#include "tiltfive.h"
#include <bb/blitz3d/blitz3d.h>
#include <TiltFiveNative.hpp>

#include <chrono>
#include <vector>
#include <iostream>

using Client = std::shared_ptr<tiltfive::Client>;
using Glasses = std::shared_ptr<tiltfive::Glasses>;
using Wand = std::shared_ptr<tiltfive::Wand>;

struct GlassesConfig{
	std::string ident;
	Glasses handle;
};

static Client client;
static Box gameboard;
static std::vector<GlassesConfig> glasses;

// Shim C++14 chrono_literals ms
constexpr std::chrono::milliseconds operator""_ms(unsigned long long ms) {
    return std::chrono::milliseconds(ms);
}

/// Find the first pair of available glasses
//
/// \param[in] client - std::unique_ptr to a ::Client
/// \return ::Glasses representing the first found glasses or an error
/// [WaitForGlasses]
auto waitForGlasses(Client& client) -> tiltfive::Result<Glasses> {
    std::cout << "Looking for glasses..." << std::flush;

    // Loop until we find glasses
    auto glassesList = client->listGlasses();
    if (!glassesList) {
        return glassesList.error();
    }
    while (glassesList->empty()) {
        std::cout << "." << std::flush;
        std::this_thread::sleep_for(100_ms);

        // Request a list of the available glasses
        glassesList = client->listGlasses();
        if (!glassesList) {
            return glassesList.error();
        }
    }

    // Print out the found glasses
    for (auto& glassesInstance : *glassesList) {
        std::cout << "Found : " << glassesInstance << std::endl;
    }

    // Return the first found glasses
    return tiltfive::obtainGlasses(glassesList->front(), client);
}
/// [WaitForGlasses]

auto doThingsWithWands(const Wand& wand) -> tiltfive::Result<void> {
    std::cout << "Doing something with wand : " << wand << std::endl;

    auto start = std::chrono::steady_clock::now();
    do {
        auto report = wand->getLatestReport();
        std::cout << "\r" << report;
    } while ((std::chrono::steady_clock::now() - start) < 10000_ms);

    std::cout << std::endl << "Done with wand" << std::endl;

    return tiltfive::kSuccess;
}

auto waitForWand(Glasses& glasses) -> tiltfive::Result<Wand> {
    std::cout << "Looking for wand..." << std::flush;

    auto wandHelper = glasses->getWandStreamHelper();

    for (;;) {
        auto wands = wandHelper->listWands();
        if (wands) {
            if (!wands->empty()) {
                std::cout << "Found : " << wands->front() << std::endl;
                return wands->front();
            }
        } else {
            std::cerr << "Error listing wands : " << wands << std::endl;
            return wands.error();
        }

        std::cout << "." << std::flush;
        std::this_thread::sleep_for(100_ms);
    }
}

/// [ExclusiveOps]
auto readPoses(Glasses& glasses) -> tiltfive::Result<void> {
    auto start = std::chrono::steady_clock::now();
    do {
        auto pose = glasses->getLatestGlassesPose(kT5_GlassesPoseUsage_GlassesPresentation);
        if (!pose) {
            if (pose.error() == tiltfive::Error::kTryAgain) {
                std::cout << "\rPose unavailable - Is gameboard visible?                         ";
            } else {
                return pose.error();
            }
        } else {
            std::cout << "\r" << pose;
        }
    } while ((std::chrono::steady_clock::now() - start) < 10000_ms);

    return tiltfive::kSuccess;
}
/// [ExclusiveOps]

auto doThingsWithGlasses(Glasses& glasses) -> tiltfive::Result<void> {
    std::cout << "Doing something with : " << glasses << std::endl;

    // Wait for a wand to connect
    auto wand = waitForWand(glasses);
    if (wand) {
        auto result = doThingsWithWands(*wand);
        if (!result) {
            std::cerr << "Error doing things with wands : " << result << std::endl;
            return result.error();
        }
    } else {
        std::cerr << "Error waiting for wand : " << wand << std::endl;
        return wand.error();
    }

    {
        /// [Connect]
        // Wait for exclusive glasses connection
        auto connectionHelper = glasses->createConnectionHelper("Awesome game - Player 1");
        auto connectionResult = connectionHelper->awaitConnection(10000_ms);
        if (connectionResult) {
            std::cout << "Glasses connected for exclusive use" << std::endl;
        } else {
            std::cerr << "Error connecting glasses for exclusive use : " << connectionResult
                      << std::endl;
            return connectionResult.error();
        }
        /// [Connect]

        // Reading poses
        auto result = readPoses(glasses);
        if (!result) {
            std::cerr << "Error reading poses : " << result << std::endl;
            return result.error();
        }
    }

    // The connectionHelper is destroyed at the end of the previous scope, but the glasses are still
    // reserved.  Let's release them, then confirm that exclusive ops such as reading poses fails.
    auto releaseResult = glasses->release();
    if (!releaseResult) {
        std::cerr << "Failed to release glasses : " << releaseResult << std::endl;
        return releaseResult.error();
    }
    auto readPosesResult = readPoses(glasses);
    if (readPosesResult) {
        std::cerr << "Reading poses unexpectedly succeeded after glasses release\n";
    } else if (readPosesResult.error() != tiltfive::Error::kNotConnected) {
        std::cerr << "Unexpected pose read error: " << readPosesResult << "\n";
    }

    std::cout << std::endl << "Done with glasses" << std::endl;

    return tiltfive::kSuccess;
}


/// [WaitForService]
// Convenience function to repeatedly call another function if it returns 'service unavailable'
template <typename T>
auto waitForService(Client& client, const std::function<tiltfive::Result<T>(Client& client)>& func)
    -> tiltfive::Result<T> {

    bool waitingForService = false;
    for (;;) {
        auto result = func(client);
        if (result) {
            return result;
        } else if (result.error() != tiltfive::Error::kNoService) {
            return result.error();
        }

        std::cout << (waitingForService ? "." : "Waiting for service...") << std::flush;
        waitingForService = true;
        std::this_thread::sleep_for(100_ms);
    }
}
/// [WaitForService]

class MyParamChangeListener : public tiltfive::ParamChangeListener {
    auto onSysParamChanged(const std::vector<T5_ParamSys>& changed) -> void override {
        for (const auto& param : changed) {
            std::cout << "System Parameter changed : [" << param << "]" << std::endl;
        }
    }

    auto onGlassesParamChanged(const Glasses& glasses, const std::vector<T5_ParamGlasses>& changed)
        -> void override {
        for (const auto& param : changed) {
            std::cout << "Glasses Parameter changed : " << glasses << " => [" << param << "]"
                      << std::endl;
        }
    }
};

BBLIB bb_int_t BBCALL bbT5_Initialize(){
	{
		// TODO: expose params...
		auto res=tiltfive::obtainClient( "com.tiltfive.test","0.1.0",nullptr );
		if( res.error() ) return 0;

		client=*res;
	}

	{
		auto res=client->getGameboardSize( kT5_GameboardType_XE_Raised );
		if( !res ){
			return 0;
		}

		gameboard=Box(
			Vector( -res->viewableExtentNegativeX,-res->viewableExtentNegativeY,0.0 ),
			Vector( res->viewableExtentPositiveX,res->viewableExtentPositiveY,res->	viewableExtentPositiveZ )
		);
	}

	return 1;
}

BBLIB bb_float_t BBCALL bbT5_GameboardX(){
	return gameboard.a.x;
}

BBLIB bb_float_t BBCALL bbT5_GameboardY(){
	return gameboard.a.y;
}

BBLIB bb_float_t BBCALL bbT5_GameboardZ(){
	return gameboard.a.z;
}

BBLIB bb_float_t BBCALL bbT5_GameboardWidth(){
	return gameboard.width();
}

BBLIB bb_float_t BBCALL bbT5_GameboardHeight(){
	return gameboard.height();
}

BBLIB bb_float_t BBCALL bbT5_GameboardDepth(){
	return gameboard.depth();
}

BBLIB BBStr* BBCALL bbT5_ServiceVersion(){
	auto res = client->getServiceVersion();
	if( !res ){
		return d_new BBStr();
	}

	return d_new BBStr( *res );
}

BBLIB bb_int_t BBCALL bbT5_UIRequestingAttention(){
	auto res=client->isTiltFiveUiRequestingAttention();
	if( res.error() ){
		return 0;
	}

	return *res;
}

BBLIB bb_int_t BBCALL bbT5_CountGlasses(){
	return glasses.size();
}

BBLIB BBStr* BBCALL bbT5_GlassesIdentifier( bb_int_t i ){
	auto id=glasses[i].handle->getIdentifier();

	return d_new BBStr( id );
}

BBLIB BBStr* BBCALL bbT5_GlassesFriendlyName( bb_int_t i ){
	auto name=glasses[i].handle->getFriendlyName();
	if( name.error() ){
		return d_new BBStr();
	}

	return d_new BBStr( *name );
}

BBLIB bb_float_t BBCALL bbT5_GlassesIPD( bb_int_t i ){
	auto ipd=glasses[i].handle->getIpd();
	if( ipd.error() ){
		return 0.0;
	}else{
		return *ipd;
	}
}

BBLIB bb_int_t BBCALL bbT5_Poll(){
	auto list = client->listGlasses();
	if( !list ){
		return 0;
	}

	for( auto& id:*list ){
		auto handle=tiltfive::obtainGlasses( id,client );
		if( handle.error() ) continue;

		int idx=-1;
		for( int i=0;i<glasses.size();i++ ){
			if( glasses[i].ident==id ){
				idx=i;
				break;
			}
		}
		if( idx==-1 ){
			glasses.push_back({ id,*handle });
		}else{
			glasses[idx].handle=*handle;
		}
	}

	return 1;
}


void BBCALL bbT5_RunSample() {
    /// [CreateClient]
    // Create the client
    if( !bbT5_Initialize() ){
        std::cerr << "Failed to create client : " << client << std::endl;
        std::exit(EXIT_FAILURE);
    }
    std::cout << "Obtained client : " << client << std::endl;
    /// [CreateClient]

    // Create a parameter change helper
    std::shared_ptr<MyParamChangeListener> paramChangeListener(new MyParamChangeListener());
    auto paramChangeHelper = client->createParamChangedHelper(paramChangeListener);

    // Wait for glasses
    {
        auto glasses = waitForService<Glasses>(client, waitForGlasses);
        if (!glasses) {
            std::cerr << "Failed to wait for glasses : " << glasses << std::endl;
            std::exit(EXIT_FAILURE);
        }

        // Add the glasses to the parameter change listener
        paramChangeHelper->registerGlasses(*glasses);

        // Do things with the glasses
        auto result = doThingsWithGlasses(*glasses);
        if (!result) {
            std::cerr << "Failed to do things with glasses : " << result << std::endl;
        }
    }
}
/// [Main]

BBMODULE_CREATE( tiltfive ){
	client=0;
	gameboard=Box();
	return true;
}

BBMODULE_DESTROY( tiltfive ){
	return true;
}

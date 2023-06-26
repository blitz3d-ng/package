#ifndef BB_TILTFIVE_COMMANDS_H
#define BB_TILTFIVE_COMMANDS_H

#include <bb/blitz/module.h>
#include <bb/tiltfive/tiltfive.h>

#ifdef __cplusplus
extern "C" {
#endif

// AUTOGENERATED. DO NOT EDIT.
// RUN `make` TO UPDATE.
bb_int_t BBCALL bbT5_Initialize(  );
bb_float_t BBCALL bbT5_GameboardX(  );
bb_float_t BBCALL bbT5_GameboardY(  );
bb_float_t BBCALL bbT5_GameboardZ(  );
bb_float_t BBCALL bbT5_GameboardWidth(  );
bb_float_t BBCALL bbT5_GameboardHeight(  );
bb_float_t BBCALL bbT5_GameboardDepth(  );
BBStr * BBCALL bbT5_ServiceVersion(  );
bb_int_t BBCALL bbT5_UIRequestingAttention(  );
bb_int_t BBCALL bbT5_CountGlasses(  );
BBStr * BBCALL bbT5_GlassesIdentifier( bb_int_t i );
BBStr * BBCALL bbT5_GlassesFriendlyName( bb_int_t i );
bb_float_t BBCALL bbT5_GlassesIPD( bb_int_t i );
bb_int_t BBCALL bbT5_Poll(  );
void BBCALL bbT5_RunSample(  );

#ifdef __cplusplus
}
#endif


#endif

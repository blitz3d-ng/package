#include "package.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

void createApk( const string &toolchain,const BundleInfo &bundle,const Target &target ){
	string tmpdir=string( tmpnam(0) );
	string manifestpath=/*tmpdir+*/"AndroidManifest.xml";

	// cout<<"path: "<<manifestpath<<endl;

	vector<string> features;
	features.push_back( "android.hardware.touchscreen" );
	features.push_back( "android.hardware.bluetooth" );
	features.push_back( "android.hardware.gamepad" );
	features.push_back( "android.hardware.usb.host" );
	features.push_back( "android.hardware.type.pc" );

	vector<string> permissions;
	permissions.push_back( "android.permission.BLUETOOTH" );
	permissions.push_back( "android.permission.BLUETOOTH_CONNECT" );
	permissions.push_back( "android.permission.VIBRATE" );
	permissions.push_back( "android.permission.HIGH_SAMPLING_RATE_SENSORS" );

	ofstream m;
	m.open( manifestpath );
	m<<"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
	m<<"<manifest xmlns:android=\"http://schemas.android.com/apk/res/android\"\n";
	m<<"  package=\""+bundle.identifier+"\"\n";
	m<<"  android:versionCode=\"1\"\n";
	m<<"  android:versionName=\"1.0\"\n";
	m<<"  android:installLocation=\"auto\">\n";
	m<<"\n";
	m<<"  <uses-sdk android:minSdkVersion=\""+target.version+"\"\n";
	m<<"            android:targetSdkVersion=\""+target.version+"\" />\n";
	m<<"\n";
	ifstream content( toolchain+"/manifest.template.xml" );
	if( content.is_open() ){
		string line;
		while( getline( content,line ) ){
			m<<"  "<<line<<'\n';
		}
		content.close();
	}
	m<<"</manifest>\n";
	m.close();
}

#include "system.ndk.h"

static jobject activity_class=0;
static JNIEnv *env=0;

void bbSetJNI( jobject klass,JNIEnv *e ){
	activity_class=klass;
	env=e;
}

jobject bbActivityClass(){
	return activity_class;
}

JNIEnv *bbJNIEnv(){
	return env;
}

#ifndef BB_SYSTEM_NDK_H
#define BB_SYSTEM_NDK_H

#include <jni.h>

void bbSetJNI( jobject klass,JNIEnv *e );
jobject bbActivityClass();
JNIEnv *bbJNIEnv();

#endif

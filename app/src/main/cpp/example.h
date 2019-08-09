//
// Created by Raining on 2019/8/9.
//

#ifndef OPENGLES30NDK_EXAMPLE_H
#define OPENGLES30NDK_EXAMPLE_H

#include <jni.h>

JNIEXPORT void JNICALL surfaceCreated(JNIEnv *env, jobject obj, jint color);

JNIEXPORT void JNICALL surfaceChanged(JNIEnv *env, jobject obj, jint width, jint height);

JNIEXPORT void JNICALL drawFrame(JNIEnv *env, jobject obj);

#endif //OPENGLES30NDK_EXAMPLE_H

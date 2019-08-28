#include <jni.h>
#include "example.h"

/**
 * 动态注册
 */
JNINativeMethod methods[] = {
        {"surfaceCreated", "(Landroid/content/Context;I)V", (void *) surfaceCreated},
        {"surfaceChanged", "(II)V",                         (void *) surfaceChanged},
        {"drawFrame",      "()V",                           (void *) drawFrame}
};

/**
 * 动态注册
 */
jint registerNativeMethod(JNIEnv *env) {
    jclass cl = env->FindClass("cn/root4/opengles30ndk/NativeRenderer");
    if ((env->RegisterNatives(cl, methods, sizeof(methods) / sizeof(methods[0]))) < 0) {
        return -1;
    }
    //env->UnregisterNatives(cl);
    return 0;
}

int gDirection = 0;

extern "C" JNIEXPORT void JNICALL
Java_cn_root4_opengles30ndk_MainActivity_onDirection(JNIEnv *env, jclass obj, jint d) {
    if (gDirection == d) {
        gDirection = 0;
    } else {
        gDirection = d;
    }
}

/**
 * 加载默认回调
 */
jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv *env = NULL;
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK) {
        return -1;
    }
    //注册方法
    if (registerNativeMethod(env) != JNI_OK) {
        return -1;
    }
    return JNI_VERSION_1_6;
}
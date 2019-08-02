#ifndef GLES_ESUTIL_H
#define GLES_ESUTIL_H

#include <GLES3/gl3.h>
#include <android/log.h>
#include <jni.h>

#ifndef LOG_TAG
#define LOG_TAG "opengles30ndk_log"
#endif
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

#include <stdlib.h>

//检查当前程序错误
bool checkGlError(const char* funcName);
//获取并编译着色器对象
GLuint createShader(GLenum shaderType, const char* src);
//使用着色器生成着色器程序对象
GLuint createProgram(const char* vtxSrc, const char* fragSrc);

#endif //GLES_ESUTIL_H

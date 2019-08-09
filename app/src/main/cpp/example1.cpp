//
// Created by Raining on 2019/8/9.
// 画三角形
//

#include <GLES3/gl3.h>
#include <jni.h>
#include "esUtils.h"
#include "example.h"

static const char *vertexShaderCode =
        "#version 300 es\n"
        "layout(location = 0) in vec3 aPos;"
        "out vec4 color;"
        "void main(){"
        "gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);"
        "color = vec4(0.5, 0.0, 0.0, 1.0);"
        "}";

static const char *fragmentShaderCode =
        "#version 300 es\n"
        "precision mediump float;"
        "out vec4 fragColor;"
        "in vec4 color;"
        "void main(){"
        "fragColor = color;"
        "}";

static const GLfloat vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f};

static GLuint program = 0;

JNIEXPORT void JNICALL surfaceCreated(JNIEnv *env, jobject obj, jint color) {
    GLfloat alphaF = ((color >> 24) & 0xFF) * 1.0f / 255;
    GLfloat redF = ((color >> 16) & 0xFF) * 1.0f / 255;
    GLfloat greenF = ((color >> 8) & 0xFF) * 1.0f / 255;
    GLfloat blueF = (color & 0xFF) * 1.0f / 255;
    glClearColor(redF, greenF, blueF, alphaF);

    program = createProgram(vertexShaderCode, fragmentShaderCode);
    if (program) {
        //准备三角形的坐标数据
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vertices);
        //启用三角形顶点的句柄
        glEnableVertexAttribArray(0);
        //禁止顶点数组的句柄
        //glDisableVertexAttribArray(0);
    }

    GLint maxVertexAttribs;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxVertexAttribs);
    ALOGE("maxVertexAttribs = %i", maxVertexAttribs);
}

JNIEXPORT void JNICALL surfaceChanged(JNIEnv *env, jobject obj, jint width, jint height) {
    glViewport(0, 0, width, height);
}

JNIEXPORT void JNICALL drawFrame(JNIEnv *env, jobject obj) {
    //把颜色缓冲区设置为我们预设的颜色
    glClear(GL_COLOR_BUFFER_BIT);

    if (program) {
        //将程序加入到OpenGLES环境
        glUseProgram(program);
        //绘制三角形
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }
}
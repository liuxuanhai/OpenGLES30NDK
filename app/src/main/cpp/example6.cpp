//
// Created by Raining on 2019/8/14.
// 纹理
//

#include <GLES3/gl3.h>
#include <jni.h>
#include "esUtils.h"
#include "example.h"
#include <cstdlib>
#include "image/ImageUtils.h"

float vertices[] = {
        //---- 位置 ----  ---- 颜色 ----     - 纹理坐标 -
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // 右上
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,   // 右下
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,   // 左下
        -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f    // 左上
};

unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
};

static GLuint program = 0;
static GLuint vao = 0;
static GLuint texture1 = 0;
static GLuint texture2 = 0;

JNIEXPORT void JNICALL surfaceCreated(JNIEnv *env, jobject obj, jobject context, jint color) {
    color = 0xffff8000;
    GLfloat alphaF = ((color >> 24) & 0xFF) * 1.0f / 255;
    GLfloat redF = ((color >> 16) & 0xFF) * 1.0f / 255;
    GLfloat greenF = ((color >> 8) & 0xFF) * 1.0f / 255;
    GLfloat blueF = (color & 0xFF) * 1.0f / 255;
    glClearColor(redF, greenF, blueF, alphaF);

    //加载纹理1
    glGenTextures(1, &texture1);
    //glDeleteTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture1);
    //glBindTexture(GL_TEXTURE_2D, 0);
    //为当前绑定的纹理对象设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //加载并生成纹理
    jobject bmp = readAssetImage(env, context, "wall.jpg");
    Image8888 img;
    img.SetImage(env, bmp);
    //如果要使用多级渐远纹理，我们必须手动设置所有不同的图像（不断递增第二个参数）
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.m_width, img.m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img.m_pDatas);
    glGenerateMipmap(GL_TEXTURE_2D);//这会为当前绑定的纹理自动生成所有需要的多级渐远纹理
    img.ClearAll();
    env->DeleteLocalRef(bmp);

    //加载纹理2
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    bmp = readAssetImage(env, context, "awesomeface.png");
    img.SetImage(env, bmp);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.m_width, img.m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img.m_pDatas);
    glGenerateMipmap(GL_TEXTURE_2D);
    img.ClearAll();
    env->DeleteLocalRef(bmp);

    char *vertexShaderCode = (char *) readAssetFile(env, context, "vertex2.glsl", true);
    char *fragmentShaderCode = (char *) readAssetFile(env, context, "fragment2.glsl", true);
    program = createProgram(vertexShaderCode, fragmentShaderCode);
    free(vertexShaderCode);
    free(fragmentShaderCode);

    if (program) {
        GLuint vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        GLuint ebo;
        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        //VAO 初始化部分
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        //开始保存状态
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        //位置属性
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        //颜色属性
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *) (3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        //纹理坐标
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        //绑定索引数组
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

        //保存结束
        glBindVertexArray(0);

        //设置常量
        glUseProgram(program);
        glUniform1i(glGetUniformLocation(program, "ourTexture1"), 0);
        glUniform1i(glGetUniformLocation(program, "ourTexture2"), 1);
    }
}

JNIEXPORT void JNICALL surfaceChanged(JNIEnv *env, jobject obj, jint width, jint height) {
    glViewport(0, 0, width, height);
}

JNIEXPORT void JNICALL drawFrame(JNIEnv *env, jobject obj) {
    //把颜色缓冲区设置为我们预设的颜色
    glClear(GL_COLOR_BUFFER_BIT);

    if (program) {
        glUseProgram(program);

        glActiveTexture(GL_TEXTURE0); // 在绑定纹理之前先激活纹理单元
        glBindTexture(GL_TEXTURE_2D, texture1);

        glActiveTexture(GL_TEXTURE0 + 1);//GL_TEXTURE1
        glBindTexture(GL_TEXTURE_2D, texture2);

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
}
//
// Created by Raining on 2019/9/12.
// #I# 材质光照
//

#include <GLES3/gl3.h>
#include <jni.h>
#include "esUtils.h"
#include "example.h"
#include <cstdlib>
#include <ctime>
#include "image/ImageUtils.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "base_Log.h"

GLfloat vertices[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f
};

static char *lightVertexShaderCode = "#version 300 es\n"
                                     "layout (location = 0) in vec3 aPos;"
                                     "uniform mat4 model;"
                                     "uniform mat4 view;"
                                     "uniform mat4 projection;"
                                     "void main() {"
                                     "gl_Position = projection * view * model * vec4(aPos, 1.0f);"
                                     "}";

static char *lightFragmentShaderCode = "#version 300 es\n"
                                       "precision mediump float;"
                                       "out vec4 FragColor;"
                                       "void main() {"
                                       "FragColor = vec4(1.0f);"
                                       "}";

static GLuint program = 0;
static GLuint vao = 0;
static GLuint lightProgram = 0;
static GLuint lightVao = 0;

JNIEXPORT void JNICALL surfaceCreated(JNIEnv *env, jobject obj, jobject context, jint color) {
    color = 0xff808000;
    GLfloat alphaF = ((color >> 24) & 0xFF) * 1.0f / 255;
    GLfloat redF = ((color >> 16) & 0xFF) * 1.0f / 255;
    GLfloat greenF = ((color >> 8) & 0xFF) * 1.0f / 255;
    GLfloat blueF = (color & 0xFF) * 1.0f / 255;
    glClearColor(redF, greenF, blueF, alphaF);
    glEnable(GL_DEPTH_TEST);

    char *vertexShaderCode = (char *) readAssetFile(env, context, "vertex5.glsl", true, nullptr);
    char *fragmentShaderCode = (char *) readAssetFile(env, context, "fragment5.glsl", true,
                                                      nullptr);
    program = createProgram(vertexShaderCode, fragmentShaderCode);
    free(vertexShaderCode);
    free(fragmentShaderCode);

    lightProgram = createProgram(lightVertexShaderCode, lightFragmentShaderCode);

    if (program && lightProgram) {
        GLuint vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) 0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                              (void *) (3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);

        glGenVertexArrays(1, &lightVao);
        glBindVertexArray(lightVao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) 0);
        glEnableVertexAttribArray(0);

        glBindVertexArray(0);

        glm::vec3 lightPos(1.2f, 1.0f, 2.0f);//世界坐标
        glUseProgram(program);
        glUniform3fv(glGetUniformLocation(program, "light.position"), 1, glm::value_ptr(lightPos));
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(-60.0f), glm::vec3(0.5f, 0.5f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE,
                           glm::value_ptr(model));
        //法线世界坐标转换矩阵
        glm::mat3 normalModel = glm::mat3(glm::transpose(glm::inverse(model)));
        //printMatrix3x3(glm::value_ptr(normalModel));
        glUniformMatrix3fv(glGetUniformLocation(program, "normalModel"), 1, GL_FALSE,
                           glm::value_ptr(normalModel));

        glUseProgram(lightProgram);
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f));
        glUniformMatrix4fv(glGetUniformLocation(lightProgram, "model"), 1, GL_FALSE,
                           glm::value_ptr(model));
    }
}

static int sWidth = 1;
static int sHeight = 1;

JNIEXPORT void JNICALL surfaceChanged(JNIEnv *env, jobject obj, jint width, jint height) {
    sWidth = width;
    sHeight = height;
    glViewport(0, 0, width, height);
}

JNIEXPORT void JNICALL drawFrame(JNIEnv *env, jobject obj) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (program && lightProgram) {

        timeval tv;
        gettimeofday(&tv, nullptr);
        float tme = tv.tv_sec - tv.tv_sec / 10000 * 10000 + tv.tv_usec / 1000000.0f;

        glm::mat4 projection;
        projection = glm::perspective(glm::radians(45.0f), (float) sWidth / (float) sHeight,
                                      0.1f, 100.0f);

        float radius = 10.0f;
        float camX = sin(tme) * radius;
        float camZ = cos(tme) * radius;
        glm::vec3 viewPos = glm::vec3(camX, 0.0, camZ);
        glm::mat4 view = glm::lookAt(viewPos, glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));

        glUseProgram(program);
        glUniform3fv(glGetUniformLocation(program, "viewPos"), 1, glm::value_ptr(viewPos));
        glUniformMatrix4fv(glGetUniformLocation(program, "projection"),
                           1, GL_FALSE,
                           glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(program, "view"),
                           1, GL_FALSE,
                           glm::value_ptr(view));
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        //light properties
        glm::vec3 lightColor;
        lightColor.x = sin(tme * 2.0f);
        lightColor.y = sin(tme * 0.7f);
        lightColor.z = sin(tme * 1.3f);
        glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f); // decrease the influence
        glm::vec3 ambientColor = diffuseColor * glm::vec3(0.3f); // low influence
        glUniform3fv(glGetUniformLocation(program, "light.ambient"), 1,
                     glm::value_ptr(ambientColor));
        glUniform3fv(glGetUniformLocation(program, "light.diffuse"), 1,
                     glm::value_ptr(diffuseColor));
        glUniform3f(glGetUniformLocation(program, "light.specular"), 1.0f, 1.0f, 1.0f);

        // material properties
        glUniform3f(glGetUniformLocation(program, "material.ambient"), 1.0f, 0.5f, 0.31f);
        glUniform3f(glGetUniformLocation(program, "material.diffuse"), 1.0f, 0.5f, 0.31f);
        glUniform3f(glGetUniformLocation(program, "material.specular"), 0.5f, 0.5f,
                    0.5f); // specular lighting doesn't have full effect on this object's material
        glUniform1f(glGetUniformLocation(program, "material.shininess"), 32.0f);

        glUseProgram(lightProgram);
        glUniformMatrix4fv(glGetUniformLocation(lightProgram, "projection"), 1, GL_FALSE,
                           glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lightProgram, "view"), 1, GL_FALSE,
                           glm::value_ptr(view));
        glBindVertexArray(lightVao);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}
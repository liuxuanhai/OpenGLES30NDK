#include <jni.h>
#include <GLES3/gl3.h>
#include <cstdlib>

#include <android/log.h>

#define LOG_TAG "opengles30ndk"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

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

bool checkGlError(const char *funcName) {
    GLint err = glGetError();
    if (err != GL_NO_ERROR) {
        LOGE("GL error after %s(): 0x%08x\n", funcName, err);
        return true;
    }
    return false;
}

GLuint loadShader30(GLenum shaderType, const char *source) {
    GLuint shader = glCreateShader(shaderType);
    if (shader) {
        glShaderSource(shader, 1, &source, NULL);
        glCompileShader(shader);
        GLint compiled = GL_FALSE;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (compiled == GL_FALSE) {
            GLint infoLogLen = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLen);
            if (infoLogLen > 0) {
                GLchar *infoLog = (GLchar *) malloc(infoLogLen * sizeof(GLchar));
                if (infoLog) {
                    glGetShaderInfoLog(shader, infoLogLen, NULL, infoLog);
                    LOGE("Could not compile %s shader:\n%s\n",
                         shaderType == GL_VERTEX_SHADER ? "vertex" : "fragment",
                         infoLog);
                    free(infoLog);
                }
            }
            glDeleteShader(shader);
            return 0;
        }
    }
    return shader;
}

GLuint createProgram(const char *vtxSrc, const char *fragSrc) {
    GLuint vtxShader = 0;
    GLuint fragShader = 0;
    GLuint program = 0;
    GLint linked = GL_FALSE;

    vtxShader = loadShader30(GL_VERTEX_SHADER, vtxSrc);
    if (!vtxShader)
        goto EXIT;

    fragShader = loadShader30(GL_FRAGMENT_SHADER, fragSrc);
    if (!fragShader)
        goto EXIT;

    program = glCreateProgram();
    if (!program) {
        checkGlError("glCreateProgram");
        goto EXIT;
    }
    glAttachShader(program, vtxShader);
    glAttachShader(program, fragShader);

    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (!linked) {
        LOGE("Could not link program");
        GLint infoLogLen = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLen);
        if (infoLogLen) {
            GLchar *infoLog = (GLchar *) malloc(infoLogLen * sizeof(GLchar));
            if (infoLog) {
                glGetProgramInfoLog(program, infoLogLen, NULL, infoLog);
                LOGE("Could not link program:\n%s\n", infoLog);
                free(infoLog);
            }
        }
        glDeleteProgram(program);
        program = 0;
    }

    EXIT:
    glDeleteShader(vtxShader);
    glDeleteShader(fragShader);
    return program;
}

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
    }
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
        //启用三角形顶点的句柄
        glEnableVertexAttribArray(0);
        //绘制三角形
        glDrawArrays(GL_TRIANGLES, 0, 3);
        //禁止顶点数组的句柄
        glDisableVertexAttribArray(0);
    }
}

/**
 * 动态注册
 */
JNINativeMethod methods[] = {
        {"surfaceCreated", "(I)V",  (void *) surfaceCreated},
        {"surfaceChanged", "(II)V", (void *) surfaceChanged},
        {"drawFrame",      "()V",   (void *) drawFrame}
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

/**
 * 加载默认回调
 * @param vm
 * @param reserved
 * @return
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
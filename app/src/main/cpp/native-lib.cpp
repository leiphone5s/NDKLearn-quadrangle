#include <jni.h>
#include <string>
#include "GLES3/gl3.h"
#include "log/XLog.h"
#include "shader/ShaderUtils.h"

AAssetManager  *g_pAssetManager = NULL;

GLint program;


extern "C"
{
JNIEXPORT jstring JNICALL
Java_com_zhidao_ndktest02_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
JNIEXPORT void JNICALL
Java_com_zhidao_ndktest02_natives_NativeOperate_glInit(JNIEnv *env, jobject thiz) {
    // TODO: implement glInit()
    char *vertexShaderSource = readAssetFile("vertex.vsh",g_pAssetManager);
    char *fragmentShaderSource = readAssetFile("fragment.fsh",g_pAssetManager);
    program = createProgram(vertexShaderSource,fragmentShaderSource);
    if(program == GL_NONE){
        LOGE("gl init failed!");
    }
    glClearColor(0.0f,0.0f,0.0f,1.0f);
}
JNIEXPORT void JNICALL
Java_com_zhidao_ndktest02_natives_NativeOperate_glResize(JNIEnv *env, jobject thiz, jint width,
                                                         jint height) {
    // TODO: implement glResize()
    glViewport(0,0,width,height);
}
JNIEXPORT void JNICALL
Java_com_zhidao_ndktest02_natives_NativeOperate_glDraw(JNIEnv *env, jobject thiz) {
    float vertices[] = {
            0.5f, 0.5f, 0.0f,  // top right
            0.5f, -0.5f, 0.0f,  // bottom right
            -0.5f, -0.5f, 0.0f,  // bottom left
            -0.5f, 0.5f, 0.0f   // top left
    };
    unsigned int indices[] = {  // note that we start from 0!
            0, 1, 3,  // first Triangle
            1, 2, 3   // second Triangle
    };
    unsigned int VBO, VAO, EBO;
    // 1. 绑定顶点数组对象
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // 2. 把我们的顶点数组复制到一个顶点缓冲中，供OpenGL使用
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //3，复制我们的索引数组到一个索引缓冲中，供OpenGL使用

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 4，设定顶点属性指针
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    // 解除绑定
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // render
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // draw our first triangle
    glUseProgram(program);
    glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}
JNIEXPORT void JNICALL
Java_com_zhidao_ndktest02_natives_NativeOperate_registerAssetManager(JNIEnv *env, jobject thiz,
                                                                     jobject asset_manager) {

    // TODO: implement registerAssetManager()
    if(asset_manager){
        g_pAssetManager = AAssetManager_fromJava(env,asset_manager);
    }else{
        LOGE("assetManager is null!");
    }
}
}
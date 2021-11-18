#include "main.hpp"
#include <iostream>
#include <algorithm>
#include "userInteraction.hpp"
#include "CreateSolid.hpp"
#include <filesystem>
#include <string>
using namespace std;
using namespace glm;

int main(int arg, const char*argv[])
{
    std::cout << "--------------------EulerOperation-zwk--------------------" << endl;
    InitGLFW();
    InitInteraction();
    glEnable(GL_DEPTH_TEST);
    glClearColor(175/255.0, 238/255.0, 238/255.0, 1.0);
    
    //读取文件并生成模型
    vector<Solid*> solidList;
    Solid *solidT;
    for(int i=1;i<arg;i++){
        solidT = CreateByInput(argv[i]);
        solidList.push_back(solidT);
        cout<<i<<" model(s) been created!"<<endl;
    }

    GLfloat deltaTime = 0, currentTime = 0;
    GLfloat startTime = glfwGetTime();
    GLfloat lastTime = glfwGetTime();

    while (!glfwWindowShouldClose(window))
    {
        Solid *drawSolid = solidList[Interaction::count % solidList.size()];
        glfwPollEvents();
        GLfloat newTime = glfwGetTime();
        Interaction::Movement(newTime - lastTime);
        lastTime = newTime;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers

        // 左侧显示实体模型
        glViewport(0, 0, SCREEN_SIZE.x / 2.0, SCREEN_SIZE.y);
        // Perspective
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45.0f, SCREEN_SIZE.x / 2.0 / SCREEN_SIZE.y, NEAR_PLANE, FAR_PLANE);
        // 实体模型显示
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity(); // Reset the model-view matrix
        glTranslatef(0.0f, 0.0f, -15.0f);
        // 绕x轴旋转 上下
        glRotatef(Interaction::xAngle, 1, 0, 0);
        // 绕y轴旋转 左右
        glRotatef(Interaction::yAngle, 0, 1, 0);
        Render(drawSolid);

        // 左侧显示线框模型
        glViewport(SCREEN_SIZE.x / 2.0, 0, SCREEN_SIZE.x / 2.0, SCREEN_SIZE.y);
        // Perspective
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45.0f, SCREEN_SIZE.x / 2.0 / SCREEN_SIZE.y, NEAR_PLANE, FAR_PLANE);
        // 显示线框模型
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity(); // Reset the model-view matrix
        glTranslatef(0.0f, 0.0f, -15.0f);
        // 绕x轴旋转 上下
        glRotatef(Interaction::xAngle, 1, 0, 0);
        // 绕y轴旋转 左右
        glRotatef(Interaction::yAngle, 0, 1, 0);
        Render(drawSolid, true);

        glfwSwapBuffers(window);
    }

    ReleaseInteraction();

    glfwTerminate();
    return 0;
}

int InitGLFW()
{
    /* Initialize the library */
    if (!glfwInit())
    {
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    // Create window
    window = glfwCreateWindow(SCREEN_SIZE.x/2, SCREEN_SIZE.y/2, "Euler Operation", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    // Windowed
    glfwMakeContextCurrent(window);

    // Print out some info about the graphics drivers
    std::cout << "--------------------------------------" << endl;
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;

    return 0;
}

void InitInteraction()
{
    //设置键盘和鼠标
    glfwSetKeyCallback(window, Interaction::KeyCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetMouseButtonCallback(window, Interaction::MouseCallback);
    glfwSetCursorPosCallback(window,Interaction::MouseMoveCallback);

}

void ReleaseInteraction()
{
    Interaction::Release();
}

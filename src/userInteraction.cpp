#include "userInteraction.hpp"
#include <iostream>

using namespace std;

float Interaction::xAngle = 0.0;
float Interaction::yAngle = 0.0;
unsigned int Interaction::count = 0;
bool Interaction::keys[1024] = {false};
bool Interaction::leftbutton=false;
double Interaction::MouseX;
double Interaction::MouseY;

void Interaction::Release()
{
}

void Interaction::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    //按下ESC关闭窗口
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    //按下N，切换模型
    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
        count++;
    }
    //捕捉上下左右键
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            keys[key] = true;
        else if (action == GLFW_RELEASE)
            keys[key] = false;
    }
}

void Interaction::MouseCallback(GLFWwindow* window, int button, int action, int mods)
{
    //按下右键，切换模型
    if (button==GLFW_MOUSE_BUTTON_RIGHT &&  action==GLFW_PRESS) {
        count++;
    }
    //按住左键拖动进行相机视角转换
    if (button==GLFW_MOUSE_BUTTON_LEFT &&  action==GLFW_PRESS) {
        leftbutton=true;
    }
    if (button==GLFW_MOUSE_BUTTON_LEFT &&  action==GLFW_RELEASE) {
        leftbutton=false;
    }
}

void Interaction::MouseMoveCallback(GLFWwindow* window, double x, double y){
    if(!leftbutton){
        MouseX=x;
        MouseY=y;
    }
    if(leftbutton){
        yAngle += 1.0f*(x - MouseX);
		xAngle += 1.0f*(y - MouseY);
		MouseX = x;
		MouseY = y;
    }
}


//根据键盘输入进行相机视角转换
void Interaction::Movement(float deltaTime)
{
    deltaTime *= 50;
    // Camera controls
    if (keys[GLFW_KEY_UP])
        xAngle -= 1.0f * deltaTime;
    if (keys[GLFW_KEY_DOWN])
        xAngle += 1.0f * deltaTime;
    if (keys[GLFW_KEY_LEFT])
        yAngle -= 1.0f * deltaTime;
    if (keys[GLFW_KEY_RIGHT])
        yAngle += 1.0f * deltaTime;

}

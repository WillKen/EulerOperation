#pragma once
// OpenGL
#include <GL/glew.h>
#include <GLFW/glfw3.h>
// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Interaction {
public:
    static float xAngle, yAngle;
    static unsigned int count;
    static void Release();
    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void MouseCallback(GLFWwindow* window, int button, int action, int mods);
    static void Movement(float deltaTime);
    static void MouseMoveCallback(GLFWwindow* window, double x, double y);

private:
    static bool keys[1024]; 
    static bool leftbutton;
    static double MouseX;
    static double MouseY;
};
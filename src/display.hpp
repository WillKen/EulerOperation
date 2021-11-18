#pragma once

#include "EulerOperation.hpp"
#include <string>
#include <vector>
// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// GLFW
#include <GLFW/glfw3.h>
// GLUT
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

# define MAX_RING_NUM 100
# define MAX_POINT_NUM 1000

using namespace std;

void Render(Solid *_solid, const bool _line = false);
void RF(Face *_face, const bool _line);
void RL_frame(Loop *_loop);
void RL_solid(Loop *_loop);

void CALLBACK tessBeginCB(GLenum which);
void CALLBACK tessEndCB();
void CALLBACK tessErrorCB(GLenum errorCode);
void CALLBACK tessVertexCB(const GLvoid *data);
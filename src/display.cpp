
#include "display.hpp"

using namespace std;

GLdouble vdata[MAX_POINT_NUM][3];//每个点的坐标


void Render(Solid *_solid, const bool _line)
{
    Face *startF = _solid->sFace;
    Face *f = startF;
    RF(f, _line);
    while(f!=nullptr){
        RF(f, _line);
        f = f->nextF;
        if(f==startF)
            break;
    }
}

void RF(Face *_face, const bool _line)
{
    Loop *startL;
    Loop *l;

    // 实体模型
    if (!_line){
        // Draw Inner Loop
        glEnable(GL_STENCIL_TEST);
        glClearStencil(0);
        glClear(GL_STENCIL_BUFFER_BIT);
        // Write 1's into stencil buffer where the hole will be
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        glDepthMask(GL_FALSE);
        glStencilFunc(GL_ALWAYS, 1, ~0);
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    }

    // Draw innerLoop
    startL = _face->fLoop;
    l = startL;
    while(l!=nullptr){
        if (l->isInnerLoop()){
            if (!_line){
                RL_solid(l);
            }
            else{
                RL_frame(l);
            }
        }
        l = l->nextL;
        if(l==startL)
            break;
    }
    
    // 实体模型
    if (!_line){
        glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
        glStencilFunc(GL_EQUAL, 0, ~0);
        glDepthMask(GL_TRUE);
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    }

    // Draw Outter Loop
    startL = _face->fLoop;
    l = _face->fLoop;
    while(l!=nullptr){
        if (!l->isInnerLoop()){
            if (!_line){
                RL_solid(l);
            }
            else{
                RL_frame(l);
            }
        }
        l = l->nextL;
        if(l==startL)
            break;
    }
    
    if (!_line){
        glDisable(GL_STENCIL_TEST);
    }
}

// loop for wire frame model
void RL_frame(Loop *_loop)
{
    glBegin(GL_LINE_LOOP); // Begin drawing the color cube with 6 quads
    HalfEdge *startHe = _loop->lHalfEdge;
    HalfEdge *he = _loop->lHalfEdge;
    Vertex *startV = he->startV;
    while(he!=nullptr){
        glm::vec3 point = he->startV->point->p;
        glColor3f(0, 0, 0);
        glVertex3f(point.x, point.y, point.z);
        he = he->nextHe;
        if(he==startHe)
            break;
    }
    glEnd();
}

// loop for solid model
void RL_solid(Loop *_loop)
{
    GLuint id = glGenLists(1); // create a display list
    if (!id)
        return; // failed to create a list, return 0

    GLUtesselator *tess = gluNewTess(); // create a tessellator
    if (!tess)
        return; // failed to create tessellation object, return 0

    // register callback functions
    gluTessCallback(tess, GLU_TESS_BEGIN, (void(CALLBACK *)())tessBeginCB);
    gluTessCallback(tess, GLU_TESS_END, (void(CALLBACK *)())tessEndCB);
    gluTessCallback(tess, GLU_TESS_VERTEX, (void(CALLBACK *)())tessVertexCB);

    glNewList(id, GL_COMPILE);
    glColor3f(1, 1, 1);
    gluTessBeginPolygon(tess, 0); // with NULL data
    gluTessBeginContour(tess);
    HalfEdge *startHe = _loop->lHalfEdge;
    HalfEdge *he = _loop->lHalfEdge;
    Vertex *startV = he->startV;
    int flag=0;
    while(he!=nullptr){
        glm::vec3 point = he->startV->point->p;
        vdata[flag][0]=point[0];
        vdata[flag][1]=point[1];
        vdata[flag][2]=point[2];
        gluTessVertex(tess, vdata[flag],vdata[flag]);
        he = he->nextHe;
        flag++;
        if(he==startHe)
            break;
    }

    gluTessEndContour(tess);
    gluTessEndPolygon(tess);
    glEndList();

    gluDeleteTess(tess); // delete after tessellation

    glCallList(id);
}

// GLuint tessellate1()
// {
// }

///////////////////////////////////////////////////////////////////////////////
// GLU_TESS CALLBACKS
///////////////////////////////////////////////////////////////////////////////
void CALLBACK tessBeginCB(GLenum which)
{
    glBegin(which);
}

void CALLBACK tessEndCB()
{
    glEnd();
}

void CALLBACK tessVertexCB(const GLvoid *data)
{
    const GLdouble *ptr = (const GLdouble *)data;
    // cout<<"ptr:"<<*ptr<<endl;
    // 显示彩虹色
    glColor3dv(ptr);
    glVertex3dv(ptr);
}

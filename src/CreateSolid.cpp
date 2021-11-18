#include "CreateSolid.hpp"
#include <string>
#include <assert.h>
#include<iostream>
#include<fstream>

std::vector<Point*> PointData[MAX_RING_NUM];
int ringNum;
double vx,vy,vz;
glm::vec3 sweep_direction = glm::vec3(0, 0, 1);
float sweep_distance = 1.0;

void readFile(string fileName) {
	for(int i=0;i<MAX_RING_NUM;i++){
		PointData[i].clear();
	}
	std::ifstream infile;
	infile.open(fileName);
	assert(infile.is_open());
	
    infile >> vx >> vy >> vz >>sweep_distance;
    sweep_direction = glm::vec3(vx, vy, vz);
	//vNum为输入结点数,loopIdx为环的索引（0表示外环，大于等于1表示内环)
	int pNum = 0,loopIdx;
	double x, y, z;//接收坐标
	
	Point* pt;
    ringNum=-1;
	while (infile  >> x >> y >> z >> loopIdx) {
		if(loopIdx>ringNum)
            ringNum=loopIdx;
		pt = new Point(x,y,z);
		if (loopIdx >= 0 && loopIdx < MAX_RING_NUM) {//有效的索引值
			PointData[loopIdx].push_back(pt);
		}
		else {
			perror("input error:invalid ring index!");
			exit(0);
		}
		pNum++;
	}
	if(pNum>MAX_POINT_NUM) {
		perror("input error:vertex num out of range!");
		exit(0);
	}
	infile.close();
}


Solid *CreateByInput(string filename){
    readFile(filename);
    Solid *newSolid;
    Vertex *vertex_temp;
    Point* startP = PointData[0][0];//以0号环（外环）的第一个结点开始构造
	Point* pt1 = startP, * pt2 = NULL;//临时变量，用于遍历
    newSolid = mvfs(*startP, vertex_temp);
    Vertex *vertex_start=vertex_temp;
    Loop *loop_holder = newSolid->sFace->fLoop;
	for (int i = 1; i < PointData[0].size(); i++) {
		pt2 = PointData[0][i];
        HalfEdge *out_h = mev(vertex_temp, *pt2, loop_holder);
		//pt1 = pt2;
        vertex_temp=out_h->endV;
	}
    mef(vertex_temp, vertex_start, loop_holder);

	//处理内环
	for (int i = 1; i <= ringNum; i++) {
		Point* innerstartP = PointData[i][0];//内环的首点
        HalfEdge *bridge = mev(vertex_start, *innerstartP, loop_holder);
        Vertex *inner_Vertex_start = bridge->endV;
        vertex_temp=inner_Vertex_start;
		for (int j = 1; j < PointData[i].size(); j++) {
            pt2 = PointData[i][j];
            HalfEdge *inner_h = mev(vertex_temp, *pt2, loop_holder);
		    //pt1 = pt2;
            vertex_temp=inner_h->endV;

		}
		//生成一个新面
        Face *inner_face=mef(vertex_temp, inner_Vertex_start, loop_holder);
		//删除边，得到内环
        Loop *inner_loop=kemr(vertex_start, inner_Vertex_start, loop_holder);
        //删除新面
		kfmrh(loop_holder->lFace, inner_face);
	}
    sweep(loop_holder->lFace, sweep_direction, sweep_distance);
	traverseSolid(newSolid,"SWEEPED Solid");
    return newSolid;
}
#include "EulerOperation.hpp"

// 构造一个点、面、体，返回solid
// 参数：point（点的几何坐标），vertex（拓扑顶点）
Solid *mvfs(Point _point, Vertex *&_vertex)
{
    // 创建新的体、面、环
    Solid *newSolid = new Solid();
    Face *newFace = new Face();
    Loop *newLoop = new Loop();

    // 建立拓扑vertex与几何point之间的关系
    _vertex = new Vertex();
    _vertex->point = new Point();
    _vertex->point->SetPoint(_point);

    // 体、面、环之间建立关系
    newSolid->sFace = newFace;
    newFace->fSolid = newSolid;

    newFace->fLoop = newLoop;
    newLoop->lFace = newFace;

    return newSolid;
}

// 构造一个新点，同时构造一条连接新点和给定点的边
// 参数：已知顶点vertex, 新点的几何坐标，当前环
HalfEdge *mev(Vertex *_vertex, Point _point, Loop *_loop)
{
    // 创建新的物理边和对应的两条半边
    Edge *newEdge = new Edge();
    HalfEdge *newHalfEdge_1 = new HalfEdge();
    HalfEdge *newHalfEdge_2 = new HalfEdge();

    // 建立边与半边之间的关系
    newEdge->firstHe = newHalfEdge_1;
    newEdge->secondHe = newHalfEdge_2;
    newHalfEdge_1->edge = newEdge;
    newHalfEdge_2->edge = newEdge;
    newHalfEdge_1->adjacentHe = newHalfEdge_2;
    newHalfEdge_2->adjacentHe = newHalfEdge_1;

    // 建立拓扑新vertex与几何point之间的关系
    Vertex *newVertex = new Vertex();
    newVertex->point = new Point();
    newVertex->point->SetPoint(_point);

    // 半边与起止点的对应
    newHalfEdge_1->startV = _vertex;
    newHalfEdge_1->endV = newVertex;
    newHalfEdge_2->startV = newVertex;
    newHalfEdge_2->endV = _vertex;

    // 建立半边与环的关系
    newHalfEdge_1->heLoop = _loop;
    newHalfEdge_2->heLoop = _loop;

    // cout<<"flag11"<<endl;
    // 建立半边之间的先后关系（参考上课笔记）
    newHalfEdge_1->nextHe = newHalfEdge_2;
    newHalfEdge_2->prevHe = newHalfEdge_1;
    // cout<<"flag12"<<endl;
    if (_loop->lHalfEdge == nullptr)
    {
        // cout<<"flag13"<<endl;
        newHalfEdge_2->nextHe = newHalfEdge_1;
        newHalfEdge_1->prevHe = newHalfEdge_2;
        _loop->lHalfEdge = newHalfEdge_1;
    }
    else
    {
        // cout<<"flag14"<<endl;
        HalfEdge *he;
        // cout<<*(_vertex->point)<<endl;
        // traverseLoop(_loop,"flag");
        // cout<<"flag115"<<endl;
        for (he = _loop->lHalfEdge; he->nextHe->startV != _vertex; he = he->nextHe){}
        // cout<<"flag15"<<endl;
        newHalfEdge_2->nextHe = he->nextHe;
        he->nextHe->prevHe = newHalfEdge_2;
        newHalfEdge_1->prevHe = he;
        he->nextHe = newHalfEdge_1;
    }
    // cout<<"flag15"<<endl;
    return newHalfEdge_1;
}


// 以两个给定点为端点，构建一条边，同时构造一个新的环，新的面
// 参数：给定顶点1，给定顶点2，当前环
Face *mef(Vertex *_vertex_1, Vertex *_vertex_2, Loop *&_largeLoop)
{
    //保存当前环的其他内环
    Loop *tempLoop=_largeLoop;
    // 创建新的物理边和对应的两条半边
    Edge *newEdge = new Edge();
    HalfEdge *newHalfEdge_1 = new HalfEdge();
    HalfEdge *newHalfEdge_2 = new HalfEdge();
    
    // 建立边与半边之间的关系
    newEdge->firstHe = newHalfEdge_1;
    newEdge->secondHe = newHalfEdge_2;
    newHalfEdge_1->edge = newEdge;
    newHalfEdge_2->edge = newEdge;
    newHalfEdge_1->adjacentHe = newHalfEdge_2;
    newHalfEdge_2->adjacentHe = newHalfEdge_1;

    // 半边与起止点的对应
    newHalfEdge_1->startV = _vertex_1;
    newHalfEdge_1->endV = _vertex_2;
    newHalfEdge_2->startV = _vertex_2;
    newHalfEdge_2->endV = _vertex_1;

    HalfEdge *he;
    HalfEdge *startHE;
    // 寻找以vertex1为起始点的半边
    startHE = he = _largeLoop->lHalfEdge;
    while(he->startV != _vertex_1){
        he = he->nextHe;
        if(he == startHE){
            cout << "Error : EulerOperation::mef cannot find vertex_1 : " << *(_vertex_1->point) << "." << endl;
            return nullptr;
        }

    }
    HalfEdge *firstHalfEdge = he;

    // cout<<"start to search for secondEdge!"<<endl;
    // traverseLoop(_largeLoop, "string _name");
    // 寻找第二条以vertex2为起始点的半边
    bool visited=false;
    startHE = he = _largeLoop->lHalfEdge;
    while(he->startV != _vertex_2){
        he = he->nextHe;
        if(he == startHE && !visited){
            cout << "Error : EulerOperation::mef cannot find vertex_2 : " << *(_vertex_2->point) << "." << endl;
            return nullptr;
        }
        if(he->startV==_vertex_2 && !visited){
            he=he->nextHe;
            visited=true;
        }
    }
    HalfEdge *secondHalfEdge = he;
    // cout<<*(secondHalfEdge->startV->point)<<*(secondHalfEdge->endV->point)<<endl;
    // 建立新半边与现存半边之间的对应关系
    newHalfEdge_2->nextHe = firstHalfEdge;
    newHalfEdge_2->prevHe = secondHalfEdge->prevHe;
    newHalfEdge_1->nextHe = secondHalfEdge;
    newHalfEdge_1->prevHe = firstHalfEdge->prevHe;
    firstHalfEdge->prevHe->nextHe = newHalfEdge_1;
    firstHalfEdge->prevHe = newHalfEdge_2;
    secondHalfEdge->prevHe->nextHe = newHalfEdge_2;
    secondHalfEdge->prevHe = newHalfEdge_1;

    // 目前产生了两个loop, 其中一个是原loop
    _largeLoop->lHalfEdge = newHalfEdge_1;
    Loop *newLoop = new Loop();
    newLoop->lHalfEdge = newHalfEdge_2;

    
    // 针对另一个loop, 构建新的对应面，并与原有体建立联系
    Solid *oldSolid = _largeLoop->lFace->fSolid;
    Face *newFace = new Face();
    newFace->fLoop = newLoop;
    newLoop->lFace = newFace;
    newFace->fSolid = oldSolid;
    
    // 建立两个faces之间的联系
    Face *f;
    for (f = oldSolid->sFace; f->nextF != nullptr; f = f->nextF){}
    f->nextF = newFace;
    newFace->prevF = f;

    return newLoop->lFace;
}

// 去掉给定环中的一条边，生成一个内环
// 参数：边所对应的两个顶点，当前环
Loop *kemr(Vertex *_vertex_1, Vertex *_vertex_2, Loop *&_largeLoop)
{   
    //保存当前环的其他内环
    Loop *tempLoop=_largeLoop;
    // 找到从vertex1指向vertex2的一条半边以及对应的物理边
    HalfEdge *he;
    for (he = _largeLoop->lHalfEdge; !(_vertex_1 == he->startV && _vertex_2 == he->endV); he = he->nextHe){}
    Edge *e = he->edge;

    // 重建半边关系
    he->nextHe->prevHe = he->adjacentHe->prevHe;
    he->adjacentHe->prevHe->nextHe = he->nextHe;
    he->prevHe->nextHe = he->adjacentHe->nextHe;
    he->adjacentHe->nextHe->prevHe = he->prevHe;

    // 目前产生了两个loop, 其中一个是原loop
    _largeLoop->lHalfEdge = he->prevHe;
    Loop *newLoop = new Loop();
    newLoop->lHalfEdge = he->nextHe;

    // 将新loop与原face的对应面face绑定
    Face *oldFace = _largeLoop->lFace->nextF;
    newLoop->SetInnerLoop(true);
    newLoop->lFace = oldFace;
    Loop *lp;
    for (lp = oldFace->fLoop; lp->nextL!=nullptr; lp = lp->nextL){}
    lp->nextL = newLoop;
    newLoop->prevL = lp;

    return newLoop;
}

// 删去一个面，将其边为另一个面的内环，进而在体中构建一个柄，或合并两个物体
// 参数： 保留面，删除面
void kfmrh(Face *face_out, Face *face_inner)
{
    //traverseFace(face_out,"out");
    //traverseFace(face_inner,"inner");
    // cout<<"begin kefmrh"<<endl;
    // 将被删除面作为保留面的一个内环。
    Loop *_inner_loop=face_inner->fLoop;
    _inner_loop->nextL=nullptr;
    Loop *lp;
    for (lp = face_out->fLoop; lp->nextL !=nullptr; lp = lp->nextL){}
    lp->nextL = _inner_loop;
    _inner_loop->prevL = lp;
    _inner_loop->lFace = face_out;
    _inner_loop->SetInnerLoop(true);
    //cout<<"start to delete"<<endl;
    //从体中删除对应面
    Solid *oldSolid = face_inner->fSolid;
    Face *f = oldSolid->sFace;
    if (f == face_inner)
    {
        oldSolid->sFace = f->nextF;
        oldSolid->sFace->prevF = f->prevF;
    }
    else
    {   
        Face* tempF = f->nextF;
		while (tempF != face_inner && tempF != NULL) tempF = tempF->nextF;
		if (tempF != NULL){
			if(tempF->nextF !=NULL)tempF->nextF->prevF = tempF->prevF;
			tempF->prevF->nextF = tempF->nextF;
		}
    }
    //cout<<"end kefmrh"<<endl;
}


Solid *sweep(Face *_face, glm::vec3 direction, float distance)
{
    // cout<<"======Begin SWEEP"<<endl;
    //solid在扫成前有两个半边面，对应一个物理底面
    Solid* _solid=_face->fSolid;
	Face* bottomFace = _solid->sFace;//底面,不动
	Face* sweepFace = _solid->sFace->nextF;//用来扫成的面，最终要"移动到"顶面
	Loop* loop = sweepFace->fLoop;
    // traverseLoop(loop,"initloop");
    //外环
	Loop* bottomLoop = bottomFace->fLoop;
	HalfEdge* he = bottomLoop->lHalfEdge;

	Vertex* firstv = he->startV;
	Point* firstupp = new Point(firstv->point->p.x+direction[0]*distance, firstv->point->p.y+direction[1]*distance, firstv->point->p.z+direction[2]*distance);

	HalfEdge* _htemp=mev(firstv, *firstupp, loop);
    Vertex* firstupv=_htemp->endV;
    Vertex* preupv=firstupv,* vtx = NULL, *upv=NULL;
    Point* upp = NULL;
	he = he->nextHe;
	vtx = he->startV;
    // cout<<"======Begin SWEEP outloop"<<endl;
	while (vtx!=firstv) {
        // cout<<"flag1"<<endl;
		upp = new Point(vtx->point->p.x+direction[0]*distance, vtx->point->p.y+direction[1]*distance, vtx->point->p.z+direction[2]*distance);
        // cout<<"flag2"<<endl;
        _htemp=mev(vtx, *upp, loop);
        // cout<<"flag3"<<endl;
        upv=_htemp->endV;
        // cout<<"======Begin SWEEP mef inwhile"<<endl;
        mef(upv,preupv,loop);
        // cout<<"flag4"<<endl;
		//更新
		preupv = upv;
		he = he->nextHe;
		vtx = he->startV;
        // cout<<"flag"<<endl;
	}
    // cout<<"======Begin SWEEP mef outwhile"<<endl;
	mef(firstupv, preupv, loop);

    // cout<<"======Begin SWEEP innerloop"<<endl;
	//处理内环
	loop = sweepFace->fLoop->nextL; 
	bottomLoop = bottomFace->fLoop->nextL; // 底面的内环
	while (loop && bottomLoop) {
		he = bottomLoop->lHalfEdge;
		//与前面的处理方法相同
		firstv = he->startV;
		firstupp = new Point(firstv->point->p.x+direction[0]*distance, firstv->point->p.y+direction[1]*distance, firstv->point->p.z+direction[2]*distance);
		
        _htemp=mev(firstv, *firstupp, loop);
		firstupv=_htemp->endV;
        preupv = firstupv, vtx = NULL, upv = NULL;
		he = he->nextHe;
		vtx = he->startV;
		while (vtx != firstv) {
			upp = new Point(vtx->point->p.x+direction[0]*distance, vtx->point->p.y+direction[1]*distance, vtx->point->p.z+direction[2]*distance);
			_htemp=mev(vtx, *upp, loop);
			upv=_htemp->endV;
            mef(upv,preupv,loop);
			//更新
			preupv = upv;
			he = he->nextHe;
			vtx = he->startV;
		}
		mef(firstupv, preupv, loop);

		loop = loop->nextL;
		bottomLoop = bottomLoop->nextL;
	}
    // cout<<"======END SWEEP outloop"<<endl;

	return _solid;
}

void swapLoop(Loop *&_loop_1, Loop *&_loop_2)
{
    Loop *temp = _loop_1;
    _loop_1 = _loop_2;
    _loop_2 = temp;
}

void traverseSolid(Solid *_solid, string _name)
{
    cout << "-------\t Solid " << _name << "\t--------" << endl;
    Face *startF = _solid->sFace;
    Face *f = _solid->sFace;
    while(f!=nullptr){
        traverseFace(f, _name);
        cout<<"-------\t One Face END \t--------"<<endl;
        f = f->nextF;
        if(f==startF)
            break;
    }
}

void traverseFace(Face *_face, string _name)
{
    cout << "-------\t Face " << _name << "\t--------" << endl;
    Loop *startL = _face->fLoop;
    Loop *l = _face->fLoop;
    while(l!=nullptr){
        traverseLoop(l, _name);
        cout<<"-------\t One Loop END \t--------"<<endl;
        l = l->nextL;
        if(l==startL)
            break;
    }
}

void traverseLoop(Loop *_loop, string _name)
{
    cout << "-------\t Loop " << _name << "\t--------" << endl;
    HalfEdge *startHe = _loop->lHalfEdge;
    HalfEdge *he = _loop->lHalfEdge;
    Vertex *startV = he->startV;
    if (_loop->isInnerLoop()){
        cout<<"Inner loop"<<endl;
    }
    while(he!=nullptr){
        cout << *(he->startV->point) << endl;
        he = he->nextHe;
        if(he==startHe)
            break;
    }
}

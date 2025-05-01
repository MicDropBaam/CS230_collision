#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

#include "WarpEngine.h" 
#include "DemoStates.h"

#define CUE_BALL_ID 0
#define OBJ_SIZE 75.f
#define BLACK_HOLE_SIZE 80.f
#define RESISTANCE 0.975f

struct GameObject
{
	unsigned int ID;
	unsigned color;
	float size;
	float radian;

	WEVec2 objPos;
	WEVec2 objVel;
};

struct ObjMgrData
{
	struct GameObject* pGameObjects;

	int maxObj;
	int deletedObj;
	int lifeObj;
};

void ObjMgrInit(void);
void ObjMgrShutDown(void);
void ObjMgrRemoveAllObjects(void);
void ObjMgrDeleteObject(struct GameObject *pObjectData, int i);

#endif

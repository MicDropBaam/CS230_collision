#include <stdio.h>   
#include <stdlib.h>  
#include <math.h>	

#include "ObjectManager.h"

#define MAX_OBJECTS 21
#define PI 3.14159f
#define BALL_TO_BALL_DISTANCE 300.f
#define BALL_TO_BALL_DEGREE 18.f
#define DECREASE_SIZE 5.f
#define MAX_ALPHA  4278190080
#define THIRD_RGB  65536 
#define MAX_RGB_VALUE 256 

struct ObjMgrData data;

extern WEVec2 blackHolePosition;
extern float blackHoleSize;

void ObjMgrInit(void)
{
	WEVec2 screenCenter;
	int red, green, blue;
	int i = 0;

	screenCenter.x = WEApplicationGetWidth() / 2.f;
	screenCenter.y = WEApplicationGetHeight() / 2.f;

	data.maxObj = MAX_OBJECTS;

	if (!data.lifeObj)
	{
		data.lifeObj = 0;
		data.pGameObjects = (struct GameObject*)malloc(data.maxObj * sizeof(struct GameObject));
	}

	//white ball
	data.pGameObjects[i].ID = (unsigned int)CUE_BALL_ID;


	red = green = blue = MAX_RGB_VALUE - 1;
	data.pGameObjects[i].color = (unsigned)(MAX_ALPHA + (blue * THIRD_RGB)
		                                              + (green * MAX_RGB_VALUE)
		                                              + red);

	data.pGameObjects[i].objPos.x = screenCenter.x;
	data.pGameObjects[i].objPos.y = screenCenter.y;
	data.pGameObjects[i].objVel.x = 0.f;
	data.pGameObjects[i].objVel.y = 0.f;
	data.pGameObjects[i].radian = 0.f;
	data.pGameObjects[i].size = OBJ_SIZE;

	data.lifeObj++;

	//balls
	for (i = data.lifeObj; i < data.maxObj; i++)
	{
		data.pGameObjects[i].ID = (unsigned int)(i + 1);

		red = rand() % MAX_RGB_VALUE;
		green = rand() % MAX_RGB_VALUE;
		blue = rand() % MAX_RGB_VALUE;
		data.pGameObjects[i].color = (unsigned)(MAX_ALPHA + (blue * THIRD_RGB)
													      + (green * MAX_RGB_VALUE)
														  + red);

		data.pGameObjects[i].objPos.x =
			(BALL_TO_BALL_DISTANCE * (float)cos((BALL_TO_BALL_DEGREE * i) * PI / 180)) + screenCenter.x;

		data.pGameObjects[i].objPos.y =
			(BALL_TO_BALL_DISTANCE * (float)sin((BALL_TO_BALL_DEGREE * i) * PI / 180)) + screenCenter.y;

		data.pGameObjects[i].objVel.x = 0.f;
		data.pGameObjects[i].objVel.y = 0.f;
		data.pGameObjects[i].radian = 0.f;
		data.pGameObjects[i].size = OBJ_SIZE;

		data.lifeObj++;
	}

}

void ObjMgrDeleteObject(struct GameObject *pObjectData, int i)
{
	if (WEIntersectionCircleCircle(&pObjectData[i].objPos, pObjectData[i].size / 2.f,
								   &blackHolePosition, blackHoleSize / 2.f))
		pObjectData[i].size -= DECREASE_SIZE;

	if (pObjectData[i].size != OBJ_SIZE)
	{
		pObjectData[i].size -= DECREASE_SIZE;

		if (pObjectData[i].size < OBJ_SIZE / 2.f)
		{
			data.deletedObj++;
			data.lifeObj--;

			for (; i < data.lifeObj; i++)
			{
				if (i == 0)
					WESMSetNextState(DS_SHUTDOWN);

				pObjectData[i] = pObjectData[i + 1];
			}
		}
	}
}

void ObjMgrShutDown(void)
{
	free(data.pGameObjects);
}

void ObjMgrRemoveAllObjects(void)
{
	int i = 0;
	for (i = 0; i < data.lifeObj; i++)
	{
		data.deletedObj++;
		data.lifeObj--;

		data.pGameObjects[data.lifeObj - 1].ID = 0;
		data.pGameObjects[data.lifeObj - 1].color = 0;

		data.pGameObjects[data.lifeObj - 1].objPos.x = 0;
		data.pGameObjects[data.lifeObj - 1].objPos.y = 0;
		data.pGameObjects[data.lifeObj - 1].objVel.x = 0;
		data.pGameObjects[data.lifeObj - 1].objVel.y = 0;

		data.pGameObjects[data.lifeObj - 1].radian = 0;
		data.pGameObjects[data.lifeObj - 1].size = 0;
	}

	data.deletedObj += data.lifeObj;
	data.lifeObj = 0;

	free(data.pGameObjects);
	data.pGameObjects = 0;

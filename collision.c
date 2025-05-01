#include "Collision.h"
#include "ObjectManager.h" 

extern struct ObjMgrData data;

CollisionData CollisionDetection(const int i)
{
	WEVec2 nextPosition;
	CollisionData collisionData;
	int j = 0;

	collisionData.ball_ID = j;

	nextPosition.x = data.pGameObjects[i].objPos.x + (data.pGameObjects[i].objVel.x * RESISTANCE);
	nextPosition.y = data.pGameObjects[i].objPos.y + (data.pGameObjects[i].objVel.y * RESISTANCE);

	// Top Wall
	if ((nextPosition.y + data.pGameObjects[i].size / 2.f) > ((float)WEApplicationGetHeight() - WALL_SIZE / 2.f))
	{
		collisionData.cType = CT_TOP_WALL;
		return collisionData;
	}

	// Bottom Wall
	else if ((nextPosition.y - data.pGameObjects[i].size / 2.f) < WALL_SIZE / 2.f)
	{
		collisionData.cType = CT_BOTTOM_WALL;
		return collisionData;
	}

	// Right Wall
	else if ((nextPosition.x + data.pGameObjects[i].size / 2.f) > ((float)WEApplicationGetWidth() - WALL_SIZE / 2.f))
	{
		collisionData.cType = CT_RIGHT_WALL;
		return collisionData;
	}

	// Left Wall
	else if ((nextPosition.x - data.pGameObjects[i].size / 2.f) < WALL_SIZE / 2.f)
	{
		collisionData.cType = CT_LEFT_WALL;

		return collisionData;
	}

	// Ball
	for (j = i + 1; j < data.lifeObj; j++)
	{
		WEVec2 nextPos;

		nextPos.x = data.pGameObjects[j].objPos.x + (data.pGameObjects[j].objVel.x * RESISTANCE);
		nextPos.y = data.pGameObjects[j].objPos.y + (data.pGameObjects[j].objVel.y * RESISTANCE);

		if (WEIntersectionCircleCircle(&nextPosition, data.pGameObjects[i].size / 2.f,
			&nextPos, data.pGameObjects[j].size / 2.f))
		{
			collisionData.cType = CT_BALL;
			collisionData.ball_ID = j;

			return collisionData;
		}
	}

	collisionData.cType = CT_NONE;

	return collisionData;
}

void CollisionResponse(const CollisionData cData, const int i)
{
	WEVec2 prePos;
	WEVec2 nextPosition;

	prePos.x = data.pGameObjects[i].objPos.x - (data.pGameObjects[i].objVel.x * (1.f + (1.f - RESISTANCE)));
	prePos.y = data.pGameObjects[i].objPos.y - (data.pGameObjects[i].objVel.y * (1.f + (1.f - RESISTANCE)));
	nextPosition.x = data.pGameObjects[i].objPos.x;
	nextPosition.y = data.pGameObjects[i].objPos.y;

	//top
	if (cData.cType == CT_TOP_WALL)
	{
		data.pGameObjects[i].objPos.y = ((float)WEApplicationGetHeight() - (WALL_SIZE / 2.f)) - (data.pGameObjects[i].size / 2.f);
		data.pGameObjects[i].objPos.x = prePos.x + (nextPosition.x - prePos.x)
			* (data.pGameObjects[i].objPos.y - prePos.y) / (nextPosition.y - prePos.y);

		data.pGameObjects[i].objVel.y *= -1.f;
	}
	//bottom
	else if (cData.cType == CT_BOTTOM_WALL)
	{
		data.pGameObjects[i].objPos.y = (WALL_SIZE / 2.f) + (data.pGameObjects[i].size / 2.f);
		data.pGameObjects[i].objPos.x = prePos.x + (nextPosition.x - prePos.x)
			* (data.pGameObjects[i].objPos.y - prePos.y) / (nextPosition.y - prePos.y);

		data.pGameObjects[i].objVel.y *= -1.f;
	}
	//right
	else if (cData.cType == CT_RIGHT_WALL)
	{
		data.pGameObjects[i].objPos.x = ((float)WEApplicationGetWidth() - (WALL_SIZE / 2.f)) - (data.pGameObjects[i].size / 2.f);
		data.pGameObjects[i].objPos.y = prePos.y + (nextPosition.y - prePos.y)
			* (data.pGameObjects[i].objPos.x - prePos.x) / (nextPosition.x - prePos.x);

		data.pGameObjects[i].objVel.x *= -1.f;
	}
	//left
	else if (cData.cType == CT_LEFT_WALL)
	{

		data.pGameObjects[i].objPos.x = (WALL_SIZE / 2.f) + (data.pGameObjects[i].size / 2.f);
		data.pGameObjects[i].objPos.y = prePos.y + (nextPosition.y - prePos.y)
			* (data.pGameObjects[i].objPos.x - prePos.x) / (nextPosition.x - prePos.x);

		data.pGameObjects[i].objVel.x *= -1.f;
	}

	if (cData.cType == CT_BALL)
	{
		WEVec2 nextPos;
		WEVec2 iF, jF;
		WEVec2 n, un, ut;
		float nextDistance;
		float iN, jN;
		float iT, jT;

		int j = cData.ball_ID;

		nextPos.x = data.pGameObjects[j].objPos.x;
		nextPos.y = data.pGameObjects[j].objPos.y;

		nextDistance = (WEDistanceCircleCircle(&nextPosition, data.pGameObjects[i].size / 2.f,
			&nextPos, data.pGameObjects[j].size / 2.f));

		data.pGameObjects[i].objPos.x += (nextDistance / 2.f);
		data.pGameObjects[i].objPos.y += (nextDistance / 2.f);
		data.pGameObjects[j].objPos.x -= (nextDistance / 2.f);
		data.pGameObjects[j].objPos.y -= (nextDistance / 2.f);

		n.x = data.pGameObjects[j].objPos.x - data.pGameObjects[i].objPos.x;
		n.y = data.pGameObjects[j].objPos.y - data.pGameObjects[i].objPos.y;
		WEVec2Normalize(&un, &n);
		ut.x = -un.y;
		ut.y = un.x;

		iN = un.x * data.pGameObjects[i].objVel.x + un.y * data.pGameObjects[i].objVel.y;
		jN = un.x * data.pGameObjects[j].objVel.x + un.y * data.pGameObjects[j].objVel.y;
		iT = ut.x * data.pGameObjects[i].objVel.x + ut.y * data.pGameObjects[i].objVel.y;
		jT = ut.x * data.pGameObjects[j].objVel.x + ut.y * data.pGameObjects[j].objVel.y;

		iF.x = jN * un.x + iT * ut.x;
		iF.y = jN * un.y + iT * ut.y;
		jF.x = iN * un.x + jT * ut.x;
		jF.y = iN * un.y + jT * ut.y;

		data.pGameObjects[i].objVel = iF;
		data.pGameObjects[j].objVel = jF;
	}
}

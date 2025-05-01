#ifndef COLLISION_H
#define COLLISION_H

#define WALL_SIZE 20.f

enum CollisionTo { CT_NONE, CT_TOP_WALL, CT_BOTTOM_WALL, CT_RIGHT_WALL, CT_LEFT_WALL, CT_BALL };

typedef struct{

	enum CollisionTo cType;
	int ball_ID;

}CollisionData;

CollisionData CollisionDetection(const int i);
void CollisionResponse(const CollisionData cData, const int i);

#endif

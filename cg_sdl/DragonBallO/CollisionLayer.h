#pragma once
#include "Entity.h"
#include <vector>
using namespace std;
class CollisionLayer
{
public:
	CollisionLayer();
	~CollisionLayer();
	void AddEntity(Entity &ent);
	void CollideWith(Entity &ent);
protected:
	vector<Entity*> mEntities;
	int mSize;

};


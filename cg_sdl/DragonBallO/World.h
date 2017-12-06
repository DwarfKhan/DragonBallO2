#pragma once
#include "MyMath.h"
#include "Camera.h"
#include "Grid.h"

const int NUM_GRID_ROWS = 4;
const int NUM_GRID_COLUMNS = 5;
const int NUM_ZONES = NUM_GRID_ROWS * NUM_GRID_COLUMNS;

//const int GRID_CELL_SIZE = 50;

//const int GRID_CELL_SIZE = 50;
//
//Grid gGrid({ (int)SCREEN_WIDTH,(int)SCREEN_HEIGHT });

using namespace MyMath;

class World
{
friend class Camera;

public:
	World() {};
	~World() {};
	
	void InitWorldGrid(RectBoundary overlap);
	RectBoundary GetZoneBoundary(int indX, int indY);
	RectBoundary GetZoneBoundary(Float2 worldPos);
	int GetIndX(float posX);
	int GetIndY(float posY);

public:
	RectBoundary worldGrid[NUM_GRID_ROWS][NUM_GRID_COLUMNS];

public:
	int mWidth{0};
	int mHeight{0};
};


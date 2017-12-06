#include "World.h"


extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

const int GRID_CELL_SIZE = 50;

Grid gGrid({
	(int)(SCREEN_WIDTH * NUM_GRID_ROWS )/GRID_CELL_SIZE,
	(int)(SCREEN_HEIGHT * NUM_GRID_COLUMNS)/GRID_CELL_SIZE
});

void World::InitWorldGrid(RectBoundary overlap)
{
	float overlapW = overlap.right - overlap.left;
	float overlapH = overlap.bottom - overlap.top;

	worldGrid[0][0].right = float(SCREEN_WIDTH);
	worldGrid[0][0].bottom = float(SCREEN_HEIGHT);

	for (int indX = 1; indX < NUM_GRID_COLUMNS; ++indX) {
		RectBoundary &nextScrn = worldGrid[0][indX];
		RectBoundary &prevScrn = worldGrid[0][indX - 1];
		nextScrn.left = prevScrn.right - overlapW;
		nextScrn.right = nextScrn.left + SCREEN_WIDTH;
		nextScrn.top = 0;
		nextScrn.bottom = float(SCREEN_HEIGHT);
	}

	for (int indY = 1; indY < NUM_GRID_ROWS; ++indY) {
		RectBoundary &nextScrn = worldGrid[indY][0];
		RectBoundary &prevScrn = worldGrid[indY - 1][0];
		nextScrn.left = 0;
		nextScrn.right = float(SCREEN_WIDTH);
		nextScrn.top = prevScrn.bottom - overlapH;
		nextScrn.bottom = nextScrn.top + SCREEN_HEIGHT;
	}

	for (int indY = 1; indY < NUM_GRID_ROWS; ++indY) {
		for (int indX = 1; indX < NUM_GRID_COLUMNS; ++indX) {
			RectBoundary &nextScrn = worldGrid[indY][indX];
			RectBoundary &prevScrn = worldGrid[indY - 1][indX - 1];
			nextScrn.left = prevScrn.right - overlapW;
			nextScrn.right = nextScrn.left + SCREEN_WIDTH;
			nextScrn.top = prevScrn.bottom - overlapH;
			nextScrn.bottom = nextScrn.top + SCREEN_HEIGHT;
		}
	}

	mWidth = int(worldGrid[0][NUM_GRID_COLUMNS - 1].right);
	mHeight = int(worldGrid[NUM_GRID_ROWS - 1][0].bottom);
}

RectBoundary World::GetZoneBoundary(int indX, int indY) {
	indX = Clamp(indX, 0, NUM_GRID_COLUMNS - 1);
	indY = Clamp(indY, 0, NUM_GRID_ROWS - 1);
	return worldGrid[indY][indX];
}

RectBoundary World::GetZoneBoundary(Float2 worldPos) {
	int indX = GetIndX(worldPos.x);
	int indY = GetIndX(worldPos.y);
	return GetZoneBoundary(indX, indY);
}

int World::GetIndX(float posX) {
	int indX = int(posX / SCREEN_WIDTH);
	return indX;
}

int World::GetIndY(float posY) {
	int indY = int(posY / SCREEN_HEIGHT);
	return indY;
}

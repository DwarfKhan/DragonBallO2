#pragma once
#include "MyMath.h"
#include <queue>
#include "Entity.h"

using namespace MyMath;
using namespace std;

extern const int GRID_CELL_SIZE;

class Grid
{
public:
	Grid(Int2 size);
	~Grid();

public:
	bool IsValid(Int2 cellPos);
	void AddBlockedCell(Int2 cellPos);
	void AddBlockedCell(const Entity &ent);
	void RemoveBlockedCell(Int2 cellPos);
	void AddSearchCell(Int2 cellPos);
	void RemoveSearchCell(Int2 cellPos);
	Int2 GetCellIndexFromWorldPos(Int2 worldPos);
	Int2 GetCellIndexFromWorldPos(Float2 worldPos);

private:
	Int2 mSize;
	int mNumCells;
	bool *mBlockedCells;
	queue<int> *mCellsBeingSearched = nullptr;

};


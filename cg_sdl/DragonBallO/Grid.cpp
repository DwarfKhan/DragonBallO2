#include "Grid.h"




Grid::Grid(Int2 size) : mSize(size)
{
	mNumCells = size.x, size.y;
	mBlockedCells = new bool[mNumCells];
	mCellsBeingSearched = new queue<int>[mNumCells];
};

Grid::~Grid()
{
	delete[] mBlockedCells;
	delete[] mCellsBeingSearched;

	mBlockedCells = nullptr;
	mCellsBeingSearched = nullptr;
}

bool Grid::IsValid(Int2 cellPos)
{
	if (cellPos.x < 0 || cellPos.x > mSize.x) {//checking if out of grid bounds
		return false;
	}
	else if (cellPos.y < 0 || cellPos.y > mSize.y) {
		return false;
	}

	//TODO: check blocked cells
	int cellIndex = (cellPos.y * mSize.x) + cellPos.x;
	if (mBlockedCells[cellIndex]) {
		return false;
	}

	return true;
}

void Grid::AddBlockedCell(Int2 cellPos)
{
	int cellIndex = (cellPos.y * mSize.x) + cellPos.x;
	mBlockedCells[cellIndex] = true;

	//TODO:: Add some kind of queue
}

void Grid::AddBlockedCell(const Entity & ent)
{
	Float2 topLeftCorner = Float2(ent.mPos.x + ent.mTopLeftCollOffset.x, ent.mPos.y + ent.mTopLeftCollOffset.y);
	Float2 topRightCorner = Float2(ent.mPos.x + ent.mSize.x - ent.mBottomRightCollOffset.x, ent.mPos.y + ent.mTopLeftCollOffset.y);
	Float2 bottomLeftCorner = Float2(ent.mPos.x + ent.mTopLeftCollOffset.x, ent.mPos.y + ent.mSize.y - ent.mBottomRightCollOffset.y );
	Float2 bottomRightCorner = Float2(ent.mPos.x + ent.mSize.x - ent.mBottomRightCollOffset.x, ent.mPos.y + ent.mSize.y - ent.mBottomRightCollOffset.y);

	Int2 topLeftIndices = GetCellIndexFromWorldPos(topLeftCorner);
	Int2 bottomRightIndices = GetCellIndexFromWorldPos(bottomRightCorner);

	for (int y = topLeftIndices.y; y < bottomRightIndices.y; ++y) {
		for (int x = topLeftIndices.x; x < bottomRightIndices.x; ++x) {
			AddBlockedCell({x,y});
		}
	}

}

void Grid::RemoveBlockedCell(Int2 cellPos)
{
	int cellIndex = (cellPos.y * mSize.x) + cellPos.x;
	mBlockedCells[cellIndex] = false;
}

void Grid::AddSearchCell(Int2 cellPos)
{
	int cellIndex = (cellPos.y * mSize.x) + cellPos.x;
	mCellsBeingSearched[cellIndex].push(0);
}

void Grid::RemoveSearchCell(Int2 cellPos)
{
	int cellIndex = (cellPos.y * mSize.x) + cellPos.x;
	mCellsBeingSearched[cellIndex].pop();
}

Int2 Grid::GetCellIndexFromWorldPos(Int2 worldPos)
{
	return worldPos / GRID_CELL_SIZE;
}

Int2 Grid::GetCellIndexFromWorldPos(Float2 worldPos)
{
	Float2 ans = worldPos / GRID_CELL_SIZE;
	return {(int)ans.x,(int)ans.y};
}


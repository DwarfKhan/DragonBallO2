#pragma once
#include "MyMath.h"
//#include <iostream>
#include <vector>

using namespace MyMath;
using namespace std;

class PathFinder
{
	struct Node {
		Node(const Node &node) {
			mParent = node.mParent;
			mH = node.mH;
			mG = node.mG;
			mF = node.mF;
			mPos = node.mPos;
		}

		Node(Node *parent, int h, int g, int f, Int2 pos) :
			mParent(parent), mH(h), mG(g), mF(f), mPos(pos) {}

		Node(Node *parent, int g, Int2 pos) :
			mParent(parent), mG(g), mPos(pos) {}

		Node(Int2 pos) : mPos(pos) {}

		Node *mParent = nullptr;
		int mH = 0;		//Dist to home
		int mG = 0;		//Dist traveled
		int mF = 0;		//h + g
		Int2 mPos;		//x and y index pos
	};

public:
	PathFinder();
	~PathFinder();

public:
	//StartSearching(CellPos start, CellPos dest, YieldInstruction instruct = null)
	void StartSearching(Int2 start, Int2 dest);

private:
	void GeneratePath(Node pathNode);
	void GeneratePathFromNode(Node node);

	void ClearPath();

private:
	vector<Node> mPath;
};


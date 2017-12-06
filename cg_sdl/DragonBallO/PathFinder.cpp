#include "PathFinder.h"
#include "Grid.h"

#define ADJ_WEIGHT 10
#define DIAG_WEIGHT 14

extern Grid gGrid;

PathFinder::PathFinder()
{
}


PathFinder::~PathFinder()
{

}

void PathFinder::ClearPath() {
	//TODO: add logic...
}

void PathFinder::GeneratePath(Node pathNode) {
	//TODO: add logic...
}

void PathFinder::GeneratePathFromNode(Node node) {
	//TODO: add logic...
}

// 1.) Initialize PathFinder (add start pos & destination pos)
void PathFinder::StartSearching(Int2 start, Int2 dest) {
	vector<Node*> openList;
	vector<Node*> closedList;

	ClearPath();

	//2.) Create first Cell for start pos, add to OpenList.
	Node node(start);
	openList.push_back(&node);

	// 3.) While OpenList is NOT empty…
	while (openList.size() > 0) {
		// a.) Find the Node with the smallest f value ( which I will refer to as the 
		//	   “parentNode” ) and remove from OpenList.

		auto parent = openList[0];
		int parentNodeIndex = 0;

		for (int i = 0; i < openList.size(); ++i) {
			if (parent->mF > openList[i]->mF) {
				parent = openList[i];
				parentNodeIndex = i;
			}
		}

		openList.erase(openList.begin() + parentNodeIndex);

		// b.) Create 8 new Nodes from the neighbors surrounding “parentNode”, calculate g, and 
		//	   make them children of “parentNode”.
		Node nodes[8] = {
			Node(parent, parent->mG + ADJ_WEIGHT, parent->mPos + Int2(1, 0)),
			Node(parent, parent->mG + DIAG_WEIGHT, parent->mPos + Int2(1, 1)),
			Node(parent, parent->mG + DIAG_WEIGHT, parent->mPos + Int2(1, -1)),
			Node(parent, parent->mG + ADJ_WEIGHT, parent->mPos + Int2(-1, 0)),
			Node(parent, parent->mG + DIAG_WEIGHT, parent->mPos + Int2(-1, 1)),
			Node(parent, parent->mG + DIAG_WEIGHT, parent->mPos + Int2(-1, -1)),
			Node(parent, parent->mG + ADJ_WEIGHT, parent->mPos + Int2(0, 1)),
			Node(parent, parent->mG + ADJ_WEIGHT, parent->mPos + Int2(0, -1))
		};

		// c.) For each “child” Node…
		for (auto childNode : nodes) {
			// i.) If Node is NOT valid (blocked, etc.) continue...
			if (!gGrid.IsValid(childNode.mPos)) {
				continue;
			}

			// ii.) If Node in ClosedList has same position as “childNode” continue...
			bool found = false;
			for (auto node : closedList) {
				if (childNode.mPos == node->mPos) {
					found = true;
					break;
				}
			}

			if (found) {
				continue;
			}

			// iii.) Calculate h. If goal has been reached (h value is zero),
			//	   stop search and set PathNode. Else, calculate f...
			childNode.mH = (Abs(dest.x - childNode.mPos.x) +
				Abs(dest.y - childNode.mPos.y)) * ADJ_WEIGHT;

			//We're home!!!
			if (childNode.mH == 0) {
				//GeneratePath(childNode);


				mPath.push_back(childNode);
				Node *parent = childNode.mParent;
				while (parent != nullptr)
				{
					mPath.push_back(*parent);
					parent = parent->mParent;
				}

				//std::reverse(mPath.begin, mPath.end);

				delete[] & openList;
				delete[] & closedList;
				return;
			}

			//Calculating f value...
			childNode.mF = childNode.mH + childNode.mG;

			// iv.) If Node in OpenList has same position as “childNode”, but has 
			//		lower f value, assign has a lower f value, continue.
			found = false;
			for (auto node : openList) {
				if (childNode.mPos == node->mPos) {
					if (childNode.mF < node->mF) {
						node->mParent = childNode.mParent;
						node->mF = childNode.mF;
					}
					found = true;
					break;
				}
			}

			Node *childNodePtr = new Node(childNode);
			openList.push_back(childNodePtr);
		}
	}

	//Clear search...
	delete[] & openList;
	delete[] & closedList;
}

#ifndef QUADTREE_H
#define QUADTREE_H

#include <stdbool.h>
#include "../Math/Rect2D.h"
#include "Array1D.h"
#define NODE_CAPACITY 4
typedef enum
{
    WestNorth,// TOP-LEFT (1)
    EastNorth,// TOP-RIGHT (2)
    WestSouth,// BOTTOM-LEFT (3)
    EastSouth,// BOTTOM-RIGHT (4)
    NodeLimit
} QuadtreeNode;

struct QuadTree
{
    Rect2D rect; // boundary
    Array1D* objects; // list of objects
    struct QuadTree* nodes[NodeLimit]; // list of nodes
};

struct QuadTree* CreateQuadTreeNode(const struct QuadTree* parent, float width, float height, int index);
int QuadTreehash(struct QuadTree* node, float x, float y); // get cell index
void QuadtreeInsert(struct QuadTree* node, void* obj, const Rect2D* objBoundary);
void QuadTreePrint(struct QuadTree* node);
void QuadTreeSearch(struct QuadTree* node, Array1D* outs, const Rect2D* objBoundary);
void QuadTreeClear(struct QuadTree* node);
void FreeQuadTree(struct QuadTree* node);
bool QuadTreeOverlap(struct QuadTree* node, const Rect2D* boundary);

#endif // QUADTREE_H

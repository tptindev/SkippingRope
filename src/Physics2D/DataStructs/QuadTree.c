#include "QuadTree.h"
#include <stdlib.h>
#include <stdio.h>

QuadTree *CreateQuadTreeNode(const QuadTree* parent, float width, float height,  int index)
{
    QuadTree* node = (QuadTree*)malloc(sizeof(QuadTree));
    if (node != NULL)
    {
        node->objects = CreateArray1D();
        if (parent == NULL) // current node is root, have not parent (NULL)
        {
            node->rect.x = 0;
            node->rect.y = 0;
            node->rect.width = width;
            node->rect.height = height;
        }
        else
        {
            node->rect.x = parent->rect.x + (float)(index % 2) * width;
            node->rect.y = parent->rect.y + (float)(index / 2) * height;
            node->rect.width = width;
            node->rect.height = height;
        }
        node->nodes[WestNorth] = NULL; // TOP-LEFT (1)
        node->nodes[EastNorth] = NULL; // TOP-RIGHT (2)
        node->nodes[WestSouth] = NULL; // BOTTOM-LEFT (3)
        node->nodes[EastSouth] = NULL; // BOTTOM-RIGHT (4)
    }
    return node;
}

void QuadtreeInsert(QuadTree *node, void *obj, const Rect2D* objBoundary)
{
    if (node == NULL) return;
    if (!QuadTreeOverlap(node, objBoundary))
    {
        return;
    }
    if (Array1DTotalSize(node->objects) < NODE_CAPACITY)
    {
        Array1DPush(node->objects, obj);
    }
    else
    {
        for (int i = WestNorth; i < NodeLimit; ++i) {
            if (node->nodes[i] == NULL)
            {
                node->nodes[i] = CreateQuadTreeNode(node, node->rect.width / 2.0f, node->rect.height / 2.0f, i);
            }
            QuadtreeInsert(node->nodes[i], obj, objBoundary);
        }
    }

}


int QuadTreehash(QuadTree *node, float x, float y)
{
    int columns = 2;
    int rows = 2;
    int hashX = (int)((x - node->rect.x) / (node->rect.width / 2.0f));
    int hashY = (int)((y - node->rect.y) / (node->rect.height / 2.0f));
    if(hashX < 0) hashX = 0;
    if(hashX > columns) hashX = columns - 1;
    if(hashY < 0) hashY = 0;
    if(hashY > rows) hashY = rows - 1;
    return ((hashY * columns) + hashX);
}

void FreeQuadTree(QuadTree *node)
{
    if (node != NULL) 
    {
        // Clear all nodes
        for (int i = WestNorth; i < NodeLimit; i++)
        {
            if (node->nodes[i] != NULL)
            {
                FreeQuadTree(node->nodes[i]);
            }
            else
            {
                break;
            }
        }
        FreeArray1D(node->objects);
        free(node);
        node = NULL;
    }
}

bool QuadTreeOverlap(QuadTree *node, const Rect2D* boundary)
{
    if (node == NULL || boundary == NULL) return false;
    if ((boundary->x > node->rect.x + node->rect.width) || (boundary->x + boundary->width < node->rect.x)) return false;
    if ((boundary->y > node->rect.y + node->rect.height) || (boundary->y + boundary->height < node->rect.y)) return false;
    return true;
}

void QuadTreePrint(QuadTree *node)
{
    if (node == NULL) return;
    for (int i = WestNorth; i < NodeLimit; i++)
    {
        QuadTreePrint(node->nodes[i]);
    }
    char buffer[250];
    sprintf(buffer, "Node %p, (%f %f %f %f), count: %llu\n", node, node->rect.x, node->rect.y, node->rect.width, node->rect.height, node->objects->size);
    printf(buffer);
}

void QuadTreeSearch(QuadTree* node, Array1D *outs, const Rect2D *objBoundary)
{
    if (node == NULL) return;
    if (node->objects == NULL) return;
    // If is a leaf node
    bool is_leaf_node = true;
    for (int i = WestNorth; i < NodeLimit; i++)
    {
        if (node->nodes[i] != NULL)
        {
            is_leaf_node = false;
            break;
        }
    }

    if (is_leaf_node && node->objects->size > 1)
    {
        Array1DPush(outs, node);
    }
    else
    {
        for (int i = WestNorth; i < NodeLimit; i++)
        {
            QuadTreeSearch(node->nodes[i], outs, objBoundary);
        }
    }
}

void QuadTreeClear(QuadTree* node)
{
    if (node == NULL) return;
    else 
    {
        for (int i = WestNorth; i < NodeLimit; i++)
        {
            if (node->nodes[i] != NULL)
            {
                FreeQuadTree(node->nodes[i]);
            }
        }
        Array1DClear(node->objects);
    }
}

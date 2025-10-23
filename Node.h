#pragma once

#ifndef NODE_H
#define NODE_H

struct Node {
    int x;
    int y;
    int value;
    Node* next;

    Node(int v, int xp, int yp) : x(xp), y(yp), value(v), next(nullptr) {}
};

#endif

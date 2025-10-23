#pragma once


#include "Node.h"

class SparseMatrix {
  private:
    Node* start;
    int maxX; 
    int maxY;
    int count; 

    Node* find(int xPos, int yPos, Node** prev);

  public:
    SparseMatrix();

    void add(int value, int xPos, int yPos);
    int get(int xPos, int yPos);
    void remove(int xPos, int yPos);
    void printStoredValues();
    int density(); 
    SparseMatrix* multiply(SparseMatrix* second);

    ~SparseMatrix();
};

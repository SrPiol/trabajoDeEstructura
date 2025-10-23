#include "SparseMatrix.h"
#include <iostream>
#include <algorithm> 

using namespace std;

SparseMatrix::SparseMatrix() {
    start = nullptr;
    maxX = 0;
    maxY = 0;
    count = 0;
}


Node* SparseMatrix::find(int xPos, int yPos, Node** prev) {
    Node* p = start;
    Node* anterior = nullptr;
    while (p != nullptr) {
        if (p->x == xPos && p->y == yPos) {
            if (prev) *prev = anterior;
            return p;
        }
        anterior = p;
        p = p->next;
    }
    if (prev) *prev = anterior;
    return nullptr;
}

void SparseMatrix::add(int value, int xPos, int yPos) {
    if (value == 0) {
        remove(xPos, yPos);
        return;
    }
    Node* prev = nullptr;
    Node* found = find(xPos, yPos, &prev);
    if (found != nullptr) {
        // reemplazar el valor
        found->value = value;
        return;
    }
    // crear nuevo nodo y agregar al final
    Node* nuevo = new Node(value, xPos, yPos);
    if (start == nullptr) {
        start = nuevo;
    } else {
        Node* p = start;
        while (p->next != nullptr) p = p->next;
        p->next = nuevo;
    }
    count++;
    maxX = max(maxX, xPos);
    maxY = max(maxY, yPos);
}

int SparseMatrix::get(int xPos, int yPos) {
    Node* prev = nullptr;
    Node* found = find(xPos, yPos, &prev);
    if (found == nullptr) return 0;
    return found->value;
}

void SparseMatrix::remove(int xPos, int yPos) {
    Node* prev = nullptr;
    Node* found = find(xPos, yPos, &prev);
    if (found == nullptr) return;
    if (prev == nullptr) {
        //primero
        start = found->next;
    } else {
        prev->next = found->next;
    }
    delete found;
    count--;
    
}

void SparseMatrix::printStoredValues() {
    Node* p = start;
    while (p != nullptr) {
        cout << "(" << p->x << ", " << p->y << ") --> " << p->value << endl;
        p = p->next;
    }
}

int SparseMatrix::density() {

    if (maxX < 0 || maxY < 0) return 0;
    long long maxSize = (long long)(maxX + 1) * (long long)(maxY + 1);
    if (maxSize == 0) return 0;
    double d = ((double)count / (double)maxSize) * 100.0;
    return (int) d; // porcentaje aproximado
}

SparseMatrix* SparseMatrix::multiply(SparseMatrix* second) {
    SparseMatrix* result = new SparseMatrix();
    for (Node* a = start; a != nullptr; a = a->next) {
        for (Node* b = second->start; b != nullptr; b = b->next) {
            if (a->y == b->x) {

                int existing = result->get(a->x, b->y);
                int added = a->value * b->value;
                int newval = existing + added;
                result->add(newval, a->x, b->y);
            }
        }
    }
    return result;
}

SparseMatrix::~SparseMatrix() {
    Node* p = start;
    while (p != nullptr) {
        Node* t = p->next;
        delete p;
        p = t;
    }
}

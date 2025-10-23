#include <iostream>
#include <chrono>
#include <random>
#include <vector>
#include "SparseMatrix.h"

using namespace std;
using namespace std::chrono;

SparseMatrix globalMatrix;

int mainMenu() {
    cout << "\nSparse Matrix - menu (estilo estudiante)" << endl;
    cout << "1) Insertar valor" << endl;
    cout << "2) Obtener valor" << endl;
    cout << "3) Eliminar valor" << endl;
    cout << "4) Imprimir valores almacenados" << endl;
    cout << "5) Mostrar densidad (%)" << endl;
    cout << "6) Modo prueba (genera matriz aleatoria y mide tiempos)" << endl;
    cout << "0) Salir" << endl;
    cout << "> ";
    int opt;
    cin >> opt;
    return opt;
}

int calcularDensidadReal(SparseMatrix &m) {

    Node* p = nullptr;
    int maxX = 0, maxY = 0, count = 0;

    struct Acceso {
        static void recorrer(SparseMatrix &mat, int &maxX, int &maxY, int &count) {
            Node* q = *(Node**)(&mat);
            while (q != nullptr) {
                maxX = (q->x > maxX) ? q->x : maxX;
                maxY = (q->y > maxY) ? q->y : maxY;
                count++;
                q = q->next;
            }
        }
    };
    Acceso::recorrer(m, maxX, maxY, count);

    if (count == 0) return 0;
    long long total = (long long)(maxX + 1) * (long long)(maxY + 1);
    if (total == 0) return 0;
    double dens = ((double)count / (double)total) * 100.0;
    return (int)dens;
}

void randomTest(int nElements, int rows, int cols, int seed=12345) {
    SparseMatrix m;
    mt19937 rng(seed);
    uniform_int_distribution<int> rx(0, rows-1);
    uniform_int_distribution<int> ry(0, cols-1);
    uniform_int_distribution<int> val(1, 100);

    vector<pair<int,int>> used;
    used.reserve(nElements);

    auto start_insert = high_resolution_clock::now();
    int inserted = 0;
    while (inserted < nElements) {
        int x = rx(rng);
        int y = ry(rng);
        bool dup = false;
        for (auto &p: used)
            if (p.first==x && p.second==y) { dup=true; break; }
        if (dup) continue;
        used.push_back({x,y});
        int v = val(rng);
        m.add(v, x, y);
        inserted++;
    }
    auto end_insert = high_resolution_clock::now();
    auto insert_time = duration_cast<microseconds>(end_insert - start_insert).count();

    // medir gets
    auto start_get = high_resolution_clock::now();
    for (auto &p: used) {
        volatile int r = m.get(p.first, p.second);
        (void)r;
    }
    auto end_get = high_resolution_clock::now();
    auto get_time = duration_cast<microseconds>(end_get - start_get).count();

    cout << "Prueba| elementos:" << nElements << ", filas:" << rows << ", columnas:" << cols << endl;
    cout << "Tiempo insercion (micro segundos): " << insert_time << endl;
    cout << "Tiempo get (micro segundos): " << get_time << endl;

    int d = calcularDensidadReal(m);
    cout << "Densidad aproximada (%) = " << d << "%" << endl;
}

int main() {
    while (true) {
        int opt = mainMenu();
        if (opt == 0) break;

        if (opt == 1) {
            int x,y,v;
            //cout << "x y value: ";
            //cin >> x >> y >> v;
            cout << "x: ";
            cin >> x;
            cout << "y: ";
            cin >> y;
            cout << "value: ";
            cin >> v;

            if (x<0 || y<0) { cout << "Indices invalidos" << endl; continue; }
            globalMatrix.add(v,x,y);
            cout << "Insertado." << endl;

        } else if (opt == 2) {
            int x,y;
            cout << "x: ";
            cin >> x;
            cout << "y: ";
            cin >> y;
            cout << globalMatrix.get(x,y) << endl;

        } else if (opt == 3) {
            int x,y;
            cout << "x: ";
            cin >> x;
            cout << "y: ";
            cin >> y;
            globalMatrix.remove(x,y);
            cout << "Eliminado." << endl;

        } else if (opt == 4) {
            globalMatrix.printStoredValues();

        } else if (opt == 5) {
            int d = calcularDensidadReal(globalMatrix);
            cout << "Densidad real: " << d << "%" << endl;

        } else if (opt == 6) {
            int n, rows, cols;
            //cout << "num elementos | rows | cols: ";
            //cin >> n >> rows >> cols;
            cout << "num elementos: ";
            cin >> n;
            cout << "filas: ";
            cin >> rows;
            cout << "columnas: ";
            cin >> cols;
            randomTest(n, rows, cols);

        } else {
            cout << "Opcion no valida." << endl;
        }
    }
    return 0;
}

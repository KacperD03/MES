#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <set>

using namespace std;

// Struktura w�z�a
struct Node {
    double x, y; // wsp�rz�dne w�z�a

    bool isBoundary; // do macierzy Hbc - czy jest na granicy

    Node(double x1 = 0.0, double y1 = 0.0) : x(x1), y(y1), isBoundary(false) {};
};

// Struktura elementu
struct Element {
    int ID[4]; // Identyfikatory w�z��w tworz�cych element
    double Hbc[4][4] = { 0 };
    double P[4] = { 0 };

    string wypisz() {
        string txt = to_string(ID[0]) + ", " + to_string(ID[1]) + ", " + to_string(ID[2]) + ", " + to_string(ID[3]);
        return txt;
    }
};

// Struktura siatki (Grid)
struct Grid {
    int nN;        // liczba w�z��w
    int nE;        // liczba element�w
    Element* elements; // wska�nik na dynamicznie alokowan� tablic� element�w
    Node* nodes;       // wska�nik na dynamicznie alokowan� tablic� w�z��w
    //double C_global[16][16] = { 0 };

    // Konstruktor
    Grid(int numNodes, int numElements) : nN(numNodes), nE(numElements) {
        elements = new Element[nE];  // alokacja element�w
        nodes = new Node[nN];        // alokacja w�z��w
    }
    // Destruktor do zwalniania pami�ci
    ~Grid() {
        delete[] elements;  // zwalnianie tablicy element�w
        delete[] nodes;     // zwalnianie tablicy w�z��w
    }
};

// Struktura danych globalnych
struct GlobalData {
    int SimulationTime;
    int SimulationStepTime;
    int Conductivity;
    int Alfa;
    int Tot;
    int InitialTemp;
    int Density;
    int SpecificHeat;
    int nN;  // liczba w�z��w
    int nE;  // liczba element�w

    GlobalData() {
        odczytaj(); // Odczytaj dane z pliku w konstruktorze
    }

    void odczytaj();
};
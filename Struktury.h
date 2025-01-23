#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <set>

using namespace std;

// Struktura wêz³a
struct Node {
    double x, y; // wspó³rzêdne wêz³a

    bool isBoundary; // do macierzy Hbc - czy jest na granicy

    Node(double x1 = 0.0, double y1 = 0.0) : x(x1), y(y1), isBoundary(false) {};
};

// Struktura elementu
struct Element {
    int ID[4]; // Identyfikatory wêz³ów tworz¹cych element
    double Hbc[4][4] = { 0 };
    double P[4] = { 0 };

    string wypisz() {
        string txt = to_string(ID[0]) + ", " + to_string(ID[1]) + ", " + to_string(ID[2]) + ", " + to_string(ID[3]);
        return txt;
    }
};

// Struktura siatki (Grid)
struct Grid {
    int nN;        // liczba wêz³ów
    int nE;        // liczba elementów
    Element* elements; // wskaŸnik na dynamicznie alokowan¹ tablicê elementów
    Node* nodes;       // wskaŸnik na dynamicznie alokowan¹ tablicê wêz³ów
    //double C_global[16][16] = { 0 };

    // Konstruktor
    Grid(int numNodes, int numElements) : nN(numNodes), nE(numElements) {
        elements = new Element[nE];  // alokacja elementów
        nodes = new Node[nN];        // alokacja wêz³ów
    }
    // Destruktor do zwalniania pamiêci
    ~Grid() {
        delete[] elements;  // zwalnianie tablicy elementów
        delete[] nodes;     // zwalnianie tablicy wêz³ów
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
    int nN;  // liczba wêz³ów
    int nE;  // liczba elementów

    GlobalData() {
        odczytaj(); // Odczytaj dane z pliku w konstruktorze
    }

    void odczytaj();
};
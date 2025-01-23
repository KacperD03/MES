#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <set>

#include "Struktury.h"
using namespace std;

// Funkcja do obliczania wspó³rzêdnych wêz³ów
void wspolrzedne(GlobalData* dane, Grid* siatka) {
    double x_start = 0.1;
    double x = x_start;
    double y = 0.005;
    double step = 0.1 / sqrt(dane->nE); // Krok zale¿ny od liczby elementów
    int licz = 0;
    for (int i = 0; i < sqrt(dane->nN); i++) {
        for (int j = 0; j < sqrt(dane->nN); j++) {
            siatka->nodes[licz].x = x;
            siatka->nodes[licz].y = y;

            x -= step;  // Zmniejszamy wspó³rzêdn¹ x
            licz++;
        }
        x = x_start;  // Resetujemy x po ka¿dym wierszu
        y -= step;  // Zmniejszamy wspó³rzêdn¹ y
    }
}

void wczytajWspolrzedne(Grid* siatka) {
    ifstream file("wezly.txt");
    //ifstream file("test.txt");
    if (!file.is_open()) {
        cerr << "Nie mozna otworzyc pliku z wezlami!" << endl;
        return;
    }

    double x, y;
    int i = 0;
    while (file >> x >> y && i < siatka->nN) {
        siatka->nodes[i].x = x;
        siatka->nodes[i].y = y;
        i++;
    }

    file.close();
}

// Funkcja do przypisywania ID elementom
void elementy(GlobalData* dane, Grid* siatka) {
    int licz = 1;
    for (int i = 0; i < dane->nE; i++) {
        siatka->elements[i].ID[0] = licz;
        siatka->elements[i].ID[1] = licz + 1;
        siatka->elements[i].ID[2] = licz + sqrt(dane->nN) + 1;
        siatka->elements[i].ID[3] = licz + sqrt(dane->nN);
        licz++;
        if (licz % int(sqrt(dane->nN)) == 0) licz++;  // Przejœcie do nowego wiersza
    }
}


// Funkcja do wypisywania i sortowania warunków brzegowych (BC)
void wypiszBC(GlobalData* dane, Grid* siatka) {
    int n = sqrt(dane->nN);  // Zak³adamy, ¿e siatka jest kwadratowa
    set<int> bcSet;          // Set do przechowywania unikalnych elementów BC

    // Dolna krawêdŸ (y = minimalne)
    for (int i = 0; i < n; i++) {
        bcSet.insert(i + 1);
    }

    // Górna krawêdŸ (y = maksymalne)
    for (int i = dane->nN - n; i < dane->nN; i++) {
        bcSet.insert(i + 1);
    }

    // Lewa krawêdŸ (x = minimalne)
    for (int i = 0; i < dane->nN; i += n) {
        bcSet.insert(i + 1);
    }

    // Prawa krawêdŸ (x = maksymalne)
    for (int i = n - 1; i < dane->nN; i += n) {
        bcSet.insert(i + 1);
    }

    // Wypisywanie unikalnych i posortowanych elementów BC
    cout << "BC: " << endl;
    for (const auto& elem : bcSet) {
        cout << elem << " ";
    }
    cout << endl;
}


struct Matrix2x2 {
    double a, b, c, d;

    double determinant() {
        return a * d - b * c;
    }

    Matrix2x2 inverse() {
        double det = determinant();
        if (det == 0) {
            cerr << "Macierz nieodwracalna!" << endl;
            return { 0, 0, 0, 0 };
        }
        return { d / det, -b / det, -c / det, a / det };
    }

    void print() {
        cout << "[" << a << ", " << b << "]" << endl;
        cout << "[" << c << ", " << d << "]" << endl;
    }
};


Matrix2x2 computeJacobian(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4, double E, double n) {
    // pochodne funkcji kszta³tu
    double N1_E = -0.25 * (1 - n);
    double N2_E = 0.25 * (1 - n);
    double N3_E = 0.25 * (1 + n);
    double N4_E = -0.25 * (1 + n);

    double N1_n = -0.25 * (1 - E);
    double N2_n = -0.25 * (1 + E);
    double N3_n = 0.25 * (1 + E);
    double N4_n = 0.25 * (1 - E);

    //cout << "dN/dE: " << N1_E << "\t" << N2_E << "\t" << N3_E << "\t" << N4_E << endl;
    //cout << "dN/dn: " << N1_n << "\t" << N2_n << "\t" << N3_n << "\t" << N4_n << endl;
    //pochodne wspó³rzêdnych w uk³adzie globalnym
    double dx_dE = (N1_E * x1) + (N2_E * x2) + (N3_E * x3) + (N4_E * x4);
    double dx_dn = (N1_n * x1) + (N2_n * x2) + (N3_n * x3) + (N4_n * x4);
    double dy_dE = (N1_E * y1) + (N2_E * y2) + (N3_E * y3) + (N4_E * y4);
    double dy_dn = (N1_n * y1) + (N2_n * y2) + (N3_n * y3) + (N4_n * y4);

    return { dx_dE, dy_dE, dx_dn, dy_dn };
}


// Implementacja funkcji odczytuj¹cej dane globalne z pliku
void GlobalData::odczytaj() {
    ifstream file("dane.txt");
    string line;
    string temp;
    int value;
    int index;
    int licznik = 0;

    string nazwy[10] = { "SimulationTime", "SimulationStepTime", "Conductivity", "Alfa", "Tot", "InitialTemp", "Density", "SpecificHeat", "liczba wezlow", "liczba elementow" };
    while (getline(file, line)) {
        index = line.rfind(" ") + 1;
        temp = line.substr(index);
        value = stoi(temp);

        switch (licznik) {
        case 0: SimulationTime = value; break;
        case 1: SimulationStepTime = value; break;
        case 2: Conductivity = value; break;
        case 3: Alfa = value; break;
        case 4: Tot = value; break;
        case 5: InitialTemp = value; break;
        case 6: Density = value; break;
        case 7: SpecificHeat = value; break;
        case 8: nN = value; break;
        case 9: nE = value; break;
        }

        cout << nazwy[licznik] << ": " << value << endl;
        licznik++;
    }
}

//void agregujH(Grid* siatka, double H_global[16][16], double H[9][4][4]) {
void agregujH(Grid* siatka, vector<vector<double>> &H_global, vector<vector<vector<double>>> &H){
    // Inicjalizacja macierzy globalnej H
    for (int i = 0; i < siatka->nN; i++) {
        for (int j = 0; j < siatka->nN; j++) {
            H_global[i][j] = 0.0;
        }
    }

    // Iteracja przez wszystkie elementy siatki
    for (int e = 0; e < siatka->nE; e++) {
        // Pobranie identyfikatorów globalnych wêz³ów dla bie¿¹cego elementu
        int* ID = siatka->elements[e].ID;

        // Iteracja przez macierz H lokaln¹ (4x4 dla elementu czworok¹tnego)
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                // Agregacja do macierzy globalnej
                H_global[ID[i] - 1][ID[j] - 1] += H[e][i][j];
            }
        }
    }
}


//void calculateHbcAndP(Grid* siatka, double alfa, double tot, double H[9][4][4], double P_global[16]) {
void calculateHbcAndP(Grid* siatka, double alfa, double tot, vector<vector<vector<double>>> &H, vector<double> &P_global) {
    for (int e = 0; e < siatka->nE; e++) {
        Element& elem = siatka->elements[e];
        double Hbc[4][4] = { 0 };
        double P[4] = { 0 };
        double ksi[2] = { 0,0 };
        double eta[2] = { 0,0 };
        double weights[2] = { 1.0, 1.0 };

        // Iterujemy po wszystkich krawêdziach elementu
        for (int edge = 0; edge < 4; edge++) {

            int node1Index = elem.ID[edge] - 1;
            int node2Index = elem.ID[(edge + 1) % 4] - 1;

            Node node1 = siatka->nodes[node1Index];
            Node node2 = siatka->nodes[node2Index];

            // Sprawdzamy, czy krawêdŸ znajduje siê na brzegu
            if (node1.isBoundary && node2.isBoundary) {

                double detJ_boku = sqrt(pow(node2.x - node1.x, 2) + pow(node2.y - node1.y, 2)) / 2.0;
                
                for (int pc = 0; pc < 2; pc++) {
                    double N[4] = { 0 };
                    
                    // Funkcje kszta³tu dla danej krawêdzi
                    switch (edge) {
                    case 0: // Dolna krawêdŸ 
                        ksi[0] = -1 / sqrt(3); ksi[1] = 1 / sqrt(3);
                        eta[0] = -1; eta[1] = -1;
                        N[0] = 0.25 * (1 - ksi[pc]) * (1 - eta[pc]);
                        N[1] = 0.25 * (1 + ksi[pc]) * (1 - eta[pc]);
                        N[2] = 0.25 * (1 + ksi[pc]) * (1 + eta[pc]);
                        N[3] = 0.25 * (1 - ksi[pc]) * (1 + eta[pc]);
                        break;
                    case 1: // Prawa krawêdŸ 
                        ksi[0] = 1; ksi[1] = 1;
                        eta[0] = -1 / sqrt(3); eta[1] = 1 / sqrt(3);
                        N[0] = 0.25 * (1 - ksi[pc]) * (1 - eta[pc]);
                        N[1] = 0.25 * (1 + ksi[pc]) * (1 - eta[pc]);
                        N[2] = 0.25 * (1 + ksi[pc]) * (1 + eta[pc]);
                        N[3] = 0.25 * (1 - ksi[pc]) * (1 + eta[pc]);
                        break;
                    case 2: // Górna krawêdŸ 
                        ksi[0] = 1 / sqrt(3); ksi[1] = -1 / sqrt(3);
                        eta[0] = 1; eta[1] = 1;
                        N[0] = 0.25 * (1 - ksi[pc]) * (1 - eta[pc]);
                        N[1] = 0.25 * (1 + ksi[pc]) * (1 - eta[pc]);
                        N[2] = 0.25 * (1 + ksi[pc]) * (1 + eta[pc]);
                        N[3] = 0.25 * (1 - ksi[pc]) * (1 + eta[pc]);
                        break;
                    case 3: // Lewa krawêdŸ 
                        ksi[0] = -1; ksi[1] = -1;
                        eta[0] = 1 / sqrt(3); eta[1] = -1 / sqrt(3);
                        N[0] = 0.25 * (1 - ksi[pc]) * (1 - eta[pc]);
                        N[1] = 0.25 * (1 + ksi[pc]) * (1 - eta[pc]);
                        N[2] = 0.25 * (1 + ksi[pc]) * (1 + eta[pc]);
                        N[3] = 0.25 * (1 - ksi[pc]) * (1 + eta[pc]);
                        break;
                    }

                    // Sumowanie do macierzy Hbc i wektora P
                    for (int i = 0; i < 4; i++) {
                        for (int j = 0; j < 4; j++) {
                            Hbc[i][j] += alfa * N[i] * N[j] * weights[pc] * detJ_boku;
                        }
                        P[i] += alfa * tot * N[i] * weights[pc] * detJ_boku;
                    }
                }

            }
        }

        // Przypisanie macierzy Hbc i wektora P do elementu

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                Hbc[i][j] += H[e][i][j];
                elem.Hbc[i][j] = Hbc[i][j];
                H[e][i][j] = elem.Hbc[i][j];
            }
            elem.P[i] = P[i];
        }

        // dodajemy P lokalne do P globalnego
        for (int i = 0; i < 4; i++) {
            int global_i = elem.ID[i] - 1;
            P_global[global_i] += P[i];
        }
    }

}




void wypiszHbcAndP(Grid* siatka) {
    cout << "Wyniki obliczen dla macierzy [Hbc] i wektora {P}:\n";

    for (int e = 0; e < siatka->nE; e++) {
        Element& elem = siatka->elements[e];

        cout << "Element " << e + 1 << ":\n";

        // Wypisywanie macierzy [Hbc]
        cout << "[Hbc]:\n";
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                cout << elem.Hbc[i][j] << "\t";
            }
            cout << endl;
        }

        // Wypisywanie wektora {P}
        cout << "{P}:\n";
        for (int i = 0; i < 4; i++) {
            cout << elem.P[i] << "\t";
        }
        cout << "\n\n";
    }
}

void Czy_isBoundary(Grid& grid) {
    // do macierzy Hbc - lab 7
    // oblicz min oraz max z koordynatow
    double minX = grid.nodes[0].x, maxX = grid.nodes[0].x;
    double minY = grid.nodes[0].y, maxY = grid.nodes[0].y;

    for (int i = 1; i < grid.nN; i++) {
        if (grid.nodes[i].x < minX) minX = grid.nodes[i].x;
        if (grid.nodes[i].x > maxX) maxX = grid.nodes[i].x;
        if (grid.nodes[i].y < minY) minY = grid.nodes[i].y;
        if (grid.nodes[i].y > maxY) maxY = grid.nodes[i].y;
    }

    // sprawdz czy jest na granicy i ustaw flage
    for (int i = 0; i < grid.nN; i++) {
        if (grid.nodes[i].x == minX || grid.nodes[i].x == maxX || grid.nodes[i].y == minY || grid.nodes[i].y == maxY) {
            grid.nodes[i].isBoundary = true;
        }
    }
}


void ObliczanieMacierzyC(double density, double specificHeat, Grid* siatka, vector<vector<double>> &C_global, int numPoints, double points[4], double weights[4]) {
    //double C_local[9][4][4] = { 0 };
    vector<vector<vector<double>>> C_local(siatka->nE, vector<vector<double>>(4, vector<double>(4, 0.0)));

    for (int e = 0; e < siatka->nE; e++) {
    Element& elem = siatka->elements[e];
    double N[4] = { 0 };

    double x1 = siatka->nodes[elem.ID[0] - 1].x;
    double y1 = siatka->nodes[elem.ID[0] - 1].y;
    double x2 = siatka->nodes[elem.ID[1] - 1].x;
    double y2 = siatka->nodes[elem.ID[1] - 1].y;
    double x3 = siatka->nodes[elem.ID[2] - 1].x;
    double y3 = siatka->nodes[elem.ID[2] - 1].y;
    double x4 = siatka->nodes[elem.ID[3] - 1].x;
    double y4 = siatka->nodes[elem.ID[3] - 1].y;

    for (int i = 0; i < numPoints; i++) {
        for (int j = 0; j < numPoints; j++) {
            double ksi = points[i];
            double eta = points[j];
            double weight = weights[i] * weights[j];

            //Wartoœci funkcji kszta³tu w punkcie ca³kowania
            N[0] = 0.25 * (1 - ksi) * (1 - eta);
            N[1] = 0.25 * (1 + ksi) * (1 - eta);
            N[2] = 0.25 * (1 + ksi) * (1 + eta);
            N[3] = 0.25 * (1 - ksi) * (1 + eta);
            double E = points[i];
            double n = points[j];

            Matrix2x2 J = computeJacobian(x1, y1, x2, y2, x3, y3, x4, y4, E, n);
            //cout << "Jakobian: " << endl;
            //J.print();
            double detJ = J.determinant();
            //cout << "Wyznacznik Jakobianu: " << detJ << endl;
            Matrix2x2 invJ = J.inverse();
            //cout << "Odwrotnosc Jakobianu: " << endl;
            //invJ.print();
            //cout << endl << endl;
            
            // Obliczanie macierzy C lokalnej
            for (int m = 0; m < 4; m++) {
                for (int n = 0; n < 4; n++) {
                    C_local[e][m][n] += N[m] * N[n] * density * specificHeat * detJ * weight;
                }
            }
        }
    }
    
        cout << "Macierz C (lokalna) dla elementu " << e + 1 << ":" << endl;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                cout << C_local[e][i][j] << " ";
            }
            cout << endl;
        }
        cout << endl;
    }

    //agregujH(siatka, siatka->C_global, C_local);
	agregujH(siatka, C_global, C_local);
}

//void eliminacjaGaussa(Grid* siatka, double A[16][16], double b[16], double x[16]) {
void eliminacjaGaussa(Grid* siatka, vector<vector<double>> &A, vector<double> &b, vector<double> &x) {

    // Eliminacja w przód
    for (int k = 0; k < siatka->nN; k++) {
        // Normalizacja elementu g³ównego
        double pivot = A[k][k];
        for (int j = k; j < siatka->nN; j++) {
            A[k][j] /= pivot;
        }
        b[k] /= pivot;

        // Zerowanie elementów poni¿ej
        for (int i = k + 1; i < siatka->nN; i++) {
            double factor = A[i][k];
            for (int j = k; j < siatka->nN; j++) {
                A[i][j] -= factor * A[k][j];
            }
            b[i] -= factor * b[k];
        }
    }
    // Podstawianie wstecz
    for (int i = siatka->nN - 1; i >= 0; i--) {
        x[i] = b[i];
        for (int j = i + 1; j < siatka->nN; j++) {
            x[i] -= A[i][j] * x[j];
        }
    }
}

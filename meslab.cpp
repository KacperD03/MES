#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <set>

#include "Funkcje.h"
using namespace std;

int main() {
    cout << fixed;
    cout.precision(10);

    // Tworzymy obiekt GlobalData, który odczyta dane z pliku
    GlobalData dane;

    // Tworzymy siatkê (grid)
    Grid siatka(dane.nN, dane.nE);

    // Obliczanie wspó³rzêdnych wêz³ów
    wspolrzedne(&dane, &siatka);

    // Wczytywanie z pliku wez³ów
    //wczytajWspolrzedne(&siatka);

    // Wyœwietlanie wêz³ów
    cout << "Nodes:\n";
    for (int i = 0; i < dane.nN; i++) {
        cout << i + 1 << ".\t" << siatka.nodes[i].x << ", \t" << siatka.nodes[i].y << endl;
    }

    // Obliczanie i przypisywanie elementów
    elementy(&dane, &siatka);

    // Wyœwietlanie elementów
    cout << "Elements:\n";
    for (int i = 0; i < dane.nE; i++) {
        cout << i + 1 << ".\t(" << siatka.elements[i].wypisz() << ")\n";
    }

    // Wypisanie warunków brzegowych (BC)
    wypiszBC(&dane, &siatka);
    cout << endl << endl << endl;


    cout << "Wybierz schemat calkowania:\n";
    cout << "1. Dwupunktowy\n";
    cout << "2. Trojpunktowy\n";
    //cout << "3. Ze scinaniem\n";
    cout << "3. Czteropunktowy\n";
    int choice;
    cin >> choice;

    double points[16][2] = { 0 };
    int numPoints = 0;
    double weights[3] = { 0, 0, 0 };
    double w[16] = { 0 };
    for (int i = 0; i < 16; i++) {
        w[i] = 1.0;
    }
    
    //Do macierzy C
    int numPointC = 0;
    double point[4] = { 0 };
    double weight[4] = { 1,1,1,1 };
    //ca³kowanie numeryczne
    if (choice == 1) { // Dwupunktowy
        numPoints = 4;
        points[0][0] = -1 / sqrt(3); points[0][1] = -1 / sqrt(3);
        points[1][0] = 1 / sqrt(3); points[1][1] = -1 / sqrt(3);
        points[2][0] = 1 / sqrt(3); points[2][1] = 1 / sqrt(3);
        points[3][0] = -1 / sqrt(3); points[3][1] = 1 / sqrt(3);
        
        //Do macierzy C
        numPointC = 2;
        point[0] = -1 / sqrt(3); point[1] = 1 / sqrt(3);
        
    }
    else if (choice == 2) { // Trójpunktowy;
        double values[3] = { -sqrt(3.0 / 5.0), 0.0, sqrt(3.0 / 5.0) };
        double weights[3] = { 5.0 / 9.0, 8.0 / 9.0, 5.0 / 9.0 };
        numPoints = 9;
        int idx = 0;

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                points[idx][0] = values[i];
                points[idx][1] = values[j];
                w[idx] = weights[i] * weights[j];
                idx++;
            }
        }
        
		//Do macierzy C
        numPointC = 3;
        point[0] = -sqrt(3.0 / 5.0); point[1] = 0.0; point[2] = sqrt(3.0 / 5.0);
		weight[0] = 5.0 / 9.0; weight[1] = 8.0 / 9.0; weight[2] = 5.0 / 9.0;
        
    }
    /*
    else if (choice == 3) { // Ze œcinaniem
        numPoints = 1; // Jeden punkt ca³kowania
        points[0][0] = 0.0; // Œrodek elementu
        points[0][1] = 0.0;
        w[0] = 1.0; // Waga punktu (przybli¿enie jednostkowe)
    }*/
    else if (choice == 3) { // Czteropunktowy
        numPoints = 16;
        // Wspó³rzêdne i wagi punktów Gaussa
        double roots[4] = { -sqrt(1.0 / 3.0), -sqrt(3.0 / 7.0), sqrt(3.0 / 7.0), sqrt(1.0 / 3.0) };
        double weights[4] = { (18.0 - sqrt(30)) / 36.0, (18.0 + sqrt(30)) / 36.0, (18.0 + sqrt(30)) / 36.0, (18.0 - sqrt(30)) / 36.0 };

        int idx = 0;
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                points[idx][0] = roots[i];
                points[idx][1] = roots[j];
                w[idx] = weights[i] * weights[j];
                idx++;
            }
        }
        
		//Do macierzy C
        numPointC = 4;
        point[0] = -sqrt(1.0 / 3.0); point[1] = -sqrt(3.0 / 7.0); point[2] = sqrt(3.0 / 7.0); point[3] = sqrt(1.0 / 3.0);
		weight[0] = (18.0 - sqrt(30)) / 36.0; weight[1] = (18.0 + sqrt(30)) / 36.0; weight[2] = (18.0 + sqrt(30)) / 36.0; weight[3] = (18.0 - sqrt(30)) / 36.0;
        
    }
    else {
        cerr << "Nieprawid³owy wybór!" << endl;
        return 1;
    }

    //double H_local[16][4][4] = { 0 };
    //double H[9][4][4] = { 0 };
    vector<vector<vector<double>>> H_local(dane.nN, vector<vector<double>>(4, vector<double>(4, 0.0)));
    vector<vector<vector<double>>> H(dane.nE, vector<vector<double>>(4, vector<double>(4, 0.0)));


    for (int i = 0; i < dane.nE; i++) {
        Element& elem = siatka.elements[i];

        // Pobieranie wspó³rzêdnych wêz³ów danego elementu
        double x1 = siatka.nodes[elem.ID[0] - 1].x;
        double y1 = siatka.nodes[elem.ID[0] - 1].y;
        double x2 = siatka.nodes[elem.ID[1] - 1].x;
        double y2 = siatka.nodes[elem.ID[1] - 1].y;
        double x3 = siatka.nodes[elem.ID[2] - 1].x;
        double y3 = siatka.nodes[elem.ID[2] - 1].y;
        double x4 = siatka.nodes[elem.ID[3] - 1].x;
        double y4 = siatka.nodes[elem.ID[3] - 1].y;
        cout << "Element " << i + 1 << ":" << endl;
        //cout << "X1: " << x1 << " X2: " << x2 << " X3: " << x3 << " X4: " << x4 << endl;
        //cout << "y1: " << y1 << " y2: " << y2 << " y3: " << y3 << " y4: " << y4 << endl;

        for (int j = 0; j < numPoints; j++) {
            double E = points[j][0];
            double n = points[j][1];
            //cout << "pc" << j + 1 << ": " << endl;

            Matrix2x2 J = computeJacobian(x1, y1, x2, y2, x3, y3, x4, y4, E, n);

            //cout << "Jakobian: " << endl;
            //J.print();
            double detJ = J.determinant();
            //cout << "Wyznacznik Jakobianu: " << detJ << endl;
            Matrix2x2 invJ = J.inverse();
            //cout << "Odwrotnosc Jakobianu: " << endl;
            //invJ.print();
            //cout << endl << endl;

            double dN_dE[4] = { -0.25 * (1 - n), 0.25 * (1 - n), 0.25 * (1 + n), -0.25 * (1 + n) };
            double dN_dn[4] = { -0.25 * (1 - E), -0.25 * (1 + E), 0.25 * (1 + E), 0.25 * (1 - E) };

            double dN_dx[4] = {0}, dN_dy[4] = { 0 };
            for (int k = 0; k < 4; k++) {
                dN_dx[k] = invJ.a * dN_dE[k] + invJ.b * dN_dn[k];
                dN_dy[k] = invJ.c * dN_dE[k] + invJ.d * dN_dn[k];
                //cout << dN_dx[k] << endl;
                //cout << dN_dy[k] << endl;
            }


            // Sumowanie wyników do macierzy H lokalnej
            for (int m = 0; m < 4; m++) {
                for (int n = 0; n < 4; n++) {
                    H_local[j][m][n] = (dN_dx[m] * dN_dx[n] + dN_dy[m] * dN_dy[n]) * w[j] * dane.Conductivity * detJ;
                    H[i][m][n] += H_local[j][m][n];
                }
            }

            // Wyœwietlanie macierzy H w pc
            /*
            cout << "[H]pc" << j + 1 << ":\n";
            for (int m = 0; m < 4; m++) {
                for (int n = 0; n < 4; n++) {
                    cout << H_local[j][m][n] << "\t";
                }
                cout << endl;
            }
            cout << endl << endl;
            */
        }

        // Wyœwietlanie macierzy H
        
        cout << "Macierz H" << ":\n";
        for (int m = 0; m < 4; m++) {
            for (int n = 0; n < 4; n++) {
                cout << H[i][m][n] << "\t";
            }
            cout << endl;
        }
        cout << endl << endl;
        

    }

    cout.precision(3);

    //double H_global[16][16] = { 0 };
    vector<vector<double>> H_global(dane.nN, vector<double>(dane.nN, 0.0));

    // Agregacja lokalnych macierzy H do globalnej
    agregujH(&siatka, H_global, H);
    
    // Wyœwietlanie macierzy globalnej H
    cout << "Macierz globalna H:" << endl;
    for (int i = 0; i < dane.nN; i++) {
        for (int j = 0; j < dane.nN; j++) {
            cout << H_global[i][j] << "  ";
        }
        cout << endl;
    }
    

    //double Hbc_global[16][16] = { 0 };
    //double P_global[16] = { 0 };
    vector<vector<double>> Hbc_global(dane.nN, vector<double>(dane.nN, 0.0));
    vector<double> P_global(dane.nN, 0.0);
    vector<vector<double>> C_global(dane.nN, vector<double>(dane.nN, 0.0));


    Czy_isBoundary(siatka);
    cout.precision(10);
    cout << endl << endl;
    cout << "Macierze Hbc dla elementow:" << endl;
    //calculateHbcAndP(&siatka, dane.Alfa, dane.Tot, H, P_global);
    calculateHbcAndP(&siatka, dane.Alfa, dane.Tot, H, P_global);

    cout.precision(3);

    // Agregacja macierzy Hbc do globalnej
    agregujH(&siatka, Hbc_global, H);
    
    // Wyœwietlanie macierzy globalnej Hbc
    cout << "Macierz globalna Hbc:" << endl;
    for (int i = 0; i < dane.nN; i++) {
        for (int j = 0; j < dane.nN; j++) {
            cout << Hbc_global[i][j] << "  ";
        }
        cout << endl;
    }
    cout << "Wektor P globalny:" << endl;
    for (int i = 0; i < dane.nN; i++) {
        cout << P_global[i] << "  ";
    }
    cout << endl << endl;
    
    ObliczanieMacierzyC(dane.Density, dane.SpecificHeat, &siatka, C_global, numPointC, point, weight);
    
    cout << endl << "Macierz C globalna:" << endl;
    for (int i = 0; i < dane.nN; ++i) {
        for (int j = 0; j < dane.nN; ++j) {
            //cout << siatka.C_global[i][j] << " ";
            cout << C_global[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
    

    vector<double> t(dane.nN, 0.0);
    vector<vector<double>> Hbc_global_Stacjonarne(dane.nN, vector<double>(dane.nN, 0.0));
    vector<double> P_global_Stacjonarne(dane.nN, 0.0);
    for (int i = 0; i < dane.nN; i++) {
        for (int j = 0; j < dane.nN; j++) {
            Hbc_global_Stacjonarne[i][j] = Hbc_global[i][j];
        }
    }
    for (int i = 0; i < dane.nN; i++) {
        P_global_Stacjonarne[i] = P_global[i];
    }

    eliminacjaGaussa(&siatka, Hbc_global_Stacjonarne, P_global_Stacjonarne, t);
    cout << endl;

    cout << "Temperatury w wezlach:" << endl;
    for (int i = 0; i < t.size(); i++) {
        cout << "T[" << i + 1 << "] = " << t[i] << endl;
    }

    // liczba kroków czasowych
    int nSteps = dane.SimulationTime / dane.SimulationStepTime;

    // tablica temperatur pocz¹tkowych T
    //double T[16] = { 0 };
    vector<double> T(dane.nN, 0.0);

    for (int i = 0; i < dane.nN; ++i) {
        T[i] = dane.InitialTemp;
    }

    // A = ( C_global/dt + H_global )
    // b = ( C_global/dt * T(n) + P_global )
    //double A[16][16];
    //double b[16];
    vector<vector<double>> A(dane.nN, vector<double>(dane.nN, 0.0));
    vector<double> b(dane.nN, 0.0);

    for (int i = 0; i < dane.nN; ++i) {
        b[i] = 0.0;
        for (int j = 0; j < dane.nN; ++j) {
            A[i][j] = 0.0;
        }
    }

    double dt = dane.SimulationStepTime;

    for (int step = 1; step <= nSteps; step++) {
        double currentTime = step * dt;
        cout << "Krok czasowy: " << step << ", czas = " << currentTime << endl;
        // A = C_global/dt + H_global
        for (int i = 0; i < dane.nN; i++) {
            for (int j = 0; j < dane.nN; j++) {
                A[i][j] = Hbc_global[i][j] + (C_global[i][j] / dt);
            }
        }
        // b = (C_global/dt)*T(n) + P_global
        // b[i] = sum( C_global[i][j]/dt * T[j] ) + P_global[i]
        for (int i = 0; i < dane.nN; i++) {
            double sumCT = 0.0;
            for (int j = 0; j < dane.nN; j++) {
                sumCT += (C_global[i][j] / dt) * T[j];
            }
            b[i] = sumCT + P_global[i];
        }
        // uklad A * T(n+1) = b - rozwiazujemy metoda eliminacji Gaussa
        for (int k = 0; k < dane.nN; k++) {
            T[k] = 0;
        }
        /*
        cout.precision(3);
        cout << endl;
        for (int i = 0; i < dane.nN; i++) {
            for (int j = 0; j < dane.nN; j++) {
				cout << A[i][j] << " ";
            }
			cout << endl;
        }
        for (int i = 0; i < dane.nN; i++) {
                cout << b[i] << " ";
        }
        cout << endl;
        */
        eliminacjaGaussa(&siatka, A, b, T);

        double minT = T[0], maxT = T[0];
        for (int i = 1; i < dane.nN; i++) {
            if (T[i] < minT) minT = T[i];
            if (T[i] > maxT) maxT = T[i];
        }
        cout.precision(10);
        cout << "Min T = " << minT << ", Max T = " << maxT << endl;
    }

    cout << endl << "Temperatury w wezlach:" << endl;
    for (int i = 0; i < dane.nN; i++) {
        cout << "T[" << i + 1 << "] = " << T[i] << endl;
    }



    return 0;
}
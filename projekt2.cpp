#include <iostream>
#include <exception>
#include "macierz.hpp"

using namespace std;

int main() try
{
    Macierz<double> M1(2, 2);
    M1.wyswietl();
    Macierz<double> M2 = M1*M1;
    M2 = M2;
    M2.wyswietl();
} catch (const exception& e) { cout << "Blad w macierzy: "<<e.what(); }

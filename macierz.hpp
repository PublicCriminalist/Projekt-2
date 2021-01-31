#pragma once
#include <iostream>
#include <exception>
#include <cmath>

using namespace std;

template<typename T> class Macierz;
//deklaracja funkcji
template<typename T> Macierz<T> operator+(const Macierz<T>& M1, const Macierz<T>& M2);
template<typename T> Macierz<T> operator-(const Macierz<T>& M1, const Macierz<T>& M2);
template<typename T> Macierz<T> operator*(const Macierz<T>& M1, const Macierz<T>& M2);
template<typename T> Macierz<T> operator*(const T& c, const Macierz<T>& M2);

template<typename T>
class Macierz
{
public:
	//konstruktor parametryczny
	Macierz<T>(int aa, int bb) try {
		if (aa < 0 || bb < 0) throw logic_error("ujemny wymiar macierzy");
		a = aa;
		b = bb;
		M=new T*[a];
		for (int i = 0; i < a; i++) M[i] = new T[b];
		cout << "Podaj wartosci:\n";
		for (int i = 0; i < a; i++) {
			for (int j = 0; j < b; j++) cin >> M[i][j];
		}
		
	} catch (...) { cout << "Ujemny wymiar macierzy!\n"; }
	//konstruktor parametryczny tworzπcy macierz zerowπ (na potrzeby iloczynu skalarnego)
	Macierz<T>(int aa, int bb, char z):a(aa),b(bb){
		M = new T * [a];
		for (int i = 0; i < a; i++) M[i] = new T[b];
		for (int i = 0; i < a; i++) {
			for (int j = 0; j < b; j++) M[i][j]=0;
		}
	}
	//konstruktor kopiujπcy
	Macierz<T>(const Macierz<T>& M1):a(M1.a),b(M1.b) {
		M = new T * [a];
		for (int i = 0; i < a; i++) M[i] = new T[b];
		for (int i = 0; i < a; i++) {
			for (int j = 0; j < b; j++) M[i][j]= M1.M[i][j];
		}
	};
	//operator kopiowania
	Macierz<T>& operator=(const Macierz<T>& M1);
	//konstruktor przenoszπcy
	Macierz<T>(Macierz<T>&& M1) noexcept:a(0),b(0),M(nullptr) {
		a = M1.a; b = M1.b;
		M = M1.M;
		M1.M = nullptr;
		M1.a = 0; M1.b = 0;
	};
	//operator przenoszenia
	Macierz<T>& operator=(Macierz<T>&& M1);
	//funkcje zaprzyjaünione
	friend Macierz<T> operator+<T>(const Macierz<T>& M1, const Macierz<T>& M2);
	friend Macierz<T> operator-<T>(const Macierz<T>& M1, const Macierz<T>& M2);
	friend Macierz<T> operator*<T>(const Macierz<T>& M1, const Macierz<T>& M2);
	friend Macierz<T> operator*<T>(const T& c, const Macierz<T>& M2);
	//wyúwietlanie macierzy
	void wyswietl() {
		cout << "Macierz:\n";
		for (int i = 0; i < a; i++) {
			for (int j = 0; j < b; j++) cout << M[i][j] << " ";
			cout << "\n";
		}
	}
	//obliczanie wyznacznika. Dopuszczalny rozmiar macierzy jest teoretycznie nieskoÒczony, praktycznie ograniczony przez g≥ÍbokoúÊ rekurencji
	T det() try {
		if (a != b) throw logic_error("niekwadratowa macierz");
		if (a == 1) return M[0][0];
		T D = 0;
		T **tem;
		tem = new T * [a];
		for (int i = 0; i < a; i++) tem[i] = new T[a];
		for (int x = 0; x < a; x++) {
			int i = 0, j = 0;
			for (int row = 0; row < a; row++) {
				for (int col = 0; col < a; col++) {
					if (row != 0 && col != x) {
						tem[i][j++] = M[row][col];
						if (j == a - 1) {
							j = 0;
							i++;
						}
					}
				}
			}
			D += pow(-1,x) * M[0][x] * deter(tem, a - 1);
		}
		for (int i = 0; i < a; i++) delete[] tem[i];
		delete[] tem;
		return D;
	}catch (...) { cout << "Niekwadratowa macierz!"; }
	//destruktor
	~Macierz(){ 
		for (int i = 0; i < a; i++) delete[] M[i];
		delete[] M;
	}
private:
	T **M; //macierz
	int a, b; //wymiary macierzy, a-liczba wierszy, b-liczba kolumn
	//obliczanie wyznacznika. Zastosowanie rekurencji w pierwszej funkcji wymaga≥oby wprowadzania wartoúci "private" w funkcji "public", dlatego uøyto jej tutaj
	T deter(T **tem, unsigned int n) {
		T D = 0;
		T **tmp;
		tmp = new T * [n];
		for (int i = 0; i < n; i++) tmp[i] = new T[n];
		if (n == 1) return tem[0][0];
		for (int x = 0; x < n; x++) {
			int i = 0, j = 0;
			for (int row = 0; row < n; row++){
				for (int col = 0; col < n; col++){
					if (row != 0 && col != x){
						tmp[i][j++] = tem[row][col];
						if (j == n - 1){
							j = 0;
							i++;
						}
					}
				}
			}
			D += pow(-1,x) * tem[0][x] * deter(tmp, n - 1);
		}
		for (int i = 0; i < n; i++) delete[] tmp[i];
		delete[] tmp;
		return D;
	}
};
//definicja operatora kopiujπcego
template<typename T> Macierz<T>& Macierz<T>::operator=(const Macierz<T>& M1)
{
	if (this == &M1) return *this;
	for (int i = 0; i < a; i++) delete[] M[i];
	delete[] M;
	a = M1.a; b = M1.b;
	M = new T * [a];
	for (int i = 0; i < a; i++) M[i] = new T[b];
	for (int i = 0; i < a; i++) {
		for (int j = 0; j < b; j++) M[i][j] = M1.M[i][j];
	}
	return *this;
}
//definicja operatora przenoszπcego
template<typename T> Macierz<T>& Macierz<T>::operator=(Macierz<T>&& M1)
{
	if (this == &M1) return *this;
	for (int i = 0; i < a; i++) delete[] M[i];
	delete[] M;
	a = M1.a; b = M1.b;
	M = M1.M;
	M1.M = nullptr;
	M1.a = 0; M1.b = 0;
	return *this;
}
//dodawanie
template<typename T> Macierz<T> operator+(const Macierz<T>& M1, const Macierz<T>& M2) try
{
	if (M1.a != M2.a || M1.b != M2.b) throw logic_error("Niezgodnosc wymiarow macierzy");
	Macierz<T> M3 = M1;
	for (int i = 0; i < M3.a; i++) {
		for (int j = 0; j < M3.b; j++) M3.M[i][j] += M2.M[i][j];
	}
	return M3;
} catch (...) { cout << "Niezgodnosc wymiarow macierzy!\n"; }
//odejmowanie
template<typename T> Macierz<T> operator-(const Macierz<T>& M1, const Macierz<T>& M2) try
{
	if (M1.a != M2.a || M1.b != M2.b) throw logic_error("Niezgodnosc wymiarow macierzy");
	Macierz<T> M3 = M1;
	for (int i = 0; i < M3.a; i++) {
		for (int j = 0; j < M3.b; j++) M3.M[i][j] -= M2.M[i][j];
	}
	return M3;
} catch (...) { cout << "Niezgodnosc wymiarow macierzy!\n"; }
//iloczyn macierzowy
template<typename T> Macierz<T> operator*(const Macierz<T>& M1, const Macierz<T>& M2) try
{
	if (M1.b != M2.a) throw logic_error("Niezgodnosc wymiarow macierzy");
	Macierz<T> M3(M1.a, M2.b,'a');
	for (int i = 0; i < M1.a; i++) {
		for (int j = 0; j < M2.b; j++) {
			for (int k = 0; k < M1.b; k++) M3.M[i][j] += (M1.M[i][k] * M2.M[k][j]);
		}
	}
	return M3;
} catch (...) { cout << "Niezgodnosc wymiarow macierzy!\n"; }
//iloczyn skalarny
template<typename T> Macierz<T> operator*(const T& c, const Macierz<T>& M1)
{
	Macierz<T> M2(M1.a, M1.b, 'a');
	for (int i = 0; i < M1.a; i++) {
		for (int j = 0; j < M1.b; j++) M2.M[i][j] = c * M1.M[i][j];
	}
	return M2;
}
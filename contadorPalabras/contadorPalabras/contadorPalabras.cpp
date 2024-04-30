#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <omp.h>

using namespace std;

string cargarTexto(const string& nombreArchivo) {
    ifstream archivo(nombreArchivo);
    string texto((istreambuf_iterator<char>(archivo)), istreambuf_iterator<char>());
    return texto;
}

int contarPalabras(string seccionTexto) {
    int contador = 0;
    string palabra;
    istringstream stream(seccionTexto);
    while (stream >> palabra) {
        contador++;
    }
    return contador;
}

int contarVocales(const string& texto) {
    int contador = 0;
    for (char caracter : texto) {
        if (caracter == 'a' || caracter == 'e' || caracter == 'i' || caracter == 'o' || caracter == 'u' ||
            caracter == 'A' || caracter == 'E' || caracter == 'I' || caracter == 'O' || caracter == 'U') {
            contador++;
        }
    }
    return contador;
}

int main() {
    string textoO = cargarTexto("C:\\Users\\fjfue\\OneDrive\\Escritorio\\a.txt");
    int palabrasSecuenciales = contarPalabras(textoO);
    int palabrasParalelas = 0;
    string texto = textoO;

#pragma omp parallel for reduction(+:palabrasParalelas)
    for (int i = 0; i < texto.size(); i++) {
        if (texto[i] == '\n') {
            string seccion = texto.substr(0, i);
            palabrasParalelas += contarPalabras(seccion);
            texto = texto.substr(i + 1);
            i = 0;
        }
    }

    if (palabrasParalelas == palabrasSecuenciales) {
        cout << "El numero de palabras calculado es correcto: " << palabrasParalelas << endl;
    }
    else {
        cout << "El numero de palabras calculado no coincide con el calculo secuencial." << endl;
    }

/*
#pragma omp parallel for reduction(+:vocales)
    for (int i = 0; i < texto.size(); i++) {
        if (texto[i] == '\n') {
            string seccion = texto.substr(0, i);
            i = 0;
        }
    }

    cout << "Cantidad de vocales : " << vocales << endl;
*/
    return 0;
}
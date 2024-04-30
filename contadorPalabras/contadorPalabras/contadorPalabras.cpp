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

int main() {
    string textoO = cargarTexto("C:\\Users\\algarci1\\OneDrive - Intel Corporation\\Documents\\a.txt");
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

    int palabrasTotales = palabrasParalelas;

    if (palabrasTotales == palabrasSecuenciales) {
        cout << "El número de palabras calculado por el programa es correcto: " << palabrasTotales << endl;
    }
    else {
        cout << "El número de palabras calculado por el programa no coincide con el cálculo secuencial." << endl;
    }

    return 0;
}
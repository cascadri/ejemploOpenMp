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
    string textoO = cargarTexto("C:\\Users\\leslymva\\OneDrive - Intel Corporation\\Documents\\UNA\\Programación paralela\\prueba.txt");
    int palabrasSecuenciales = contarPalabras(textoO);
    int palabrasParalelas = 0;
    string texto = textoO;
    // Dividir el texto en secciones por párrafos
#pragma omp parallel for reduction(+:palabrasParalelas)
    for (int i = 0; i < texto.size(); i++) {
        if (texto[i] == '\n') {
            string seccion = texto.substr(0, i);
            palabrasParalelas += contarPalabras(seccion);
            texto = texto.substr(i + 1);
            i = 0;
        }
    }

    // Suma de resultados parciales
    int palabrasTotales = palabrasParalelas;

    // Verificación de resultados
    if (palabrasTotales == palabrasSecuenciales) {
        cout << "El numero de palabras calculado por el programa es correcto: " << palabrasTotales << endl;
    }
    else {
        cout << "El numero de palabras calculado por el programa no coincide con el calculo secuencial." << endl;
    }

    int vocales = 0;
    texto = textoO;

    // Dividir el texto en secciones por párrafos
#pragma omp parallel for reduction(+:vocales)
    for (int i = 0; i < texto.size(); i++) {
        if (texto[i] == '\n') {
            string seccion = texto.substr(0, i);
            vocales += contarVocales(seccion);
            texto = texto.substr(i + 1);
            i = 0;
        }
    }

    cout << "La cantidad de vocales en el texto es: " << vocales << endl;
    cout << "Secuencial: La cantidad de palabras en el texto es: " << palabrasSecuenciales << endl;
    cout << "Paralela: La cantidad de palabras en el texto es: " << palabrasParalelas << endl;

    return 0;
}
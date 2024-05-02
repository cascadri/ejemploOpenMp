#include <iostream>  
#include <fstream>   
#include <sstream>   
#include <string>    
#include <omp.h>     // Librería para OpenMP

using namespace std;

// Función para cargar el texto desde un archivo txo
string cargarTexto(const string& nombreArchivo) {
    ifstream archivo(nombreArchivo);  
    string texto((istreambuf_iterator<char>(archivo)), istreambuf_iterator<char>()); // Guarda el contenido del archivo en una cadena de texto
    return texto; 
}

// Función para contar las palabras en una sección de texto
int contarPalabras(string seccionTexto) {
    int contador = 0;
    string palabra;            // Variable para almacenar cada palabra
    istringstream stream(seccionTexto);
    while (stream >> palabra) {  // Aumenta el contador mientras hayan más palabras
        contador++;
    }
    return contador;
}

// Función para contar vocales en una cadena de texto
int contarVocales(const string& texto) {
    int contador = 0;  
    // Para iterar sobre cada caracter del texto
    for (char caracter : texto) {
        // Verifica si el carácter es una vocal ya sea minúscula o mayúscula
        if (caracter == 'a' || caracter == 'e' || caracter == 'i' || caracter == 'o' || caracter == 'u' ||
            caracter == 'A' || caracter == 'E' || caracter == 'I' || caracter == 'O' || caracter == 'U') {
            contador++;  
        }
    }
    return contador;  
}


int main() {
    string textoO = cargarTexto("C:\\Users\\leslymva\\OneDrive - Intel Corporation\\Documents\\UNA\\Programación paralela\\prueba.txt");  // Carga el texto recibiendo una ruta
    int palabrasSecuenciales = contarPalabras(textoO);  
    int palabrasParalelas = 0; 
    string texto = textoO;  //Copia del texto original

    //Bloque para contar palabras de forma paralela
#pragma omp parallel for reduction(+:palabrasParalelas)
    for (int i = 0; i < texto.size(); i++) {
        if (texto[i] == '\n') {  //Verifica si existe un salto de linea
            string seccion = texto.substr(0, i);  
            palabrasParalelas += contarPalabras(seccion); 
            texto = texto.substr(i + 1);  //Actualiza el texto eliminando la sección procesada
            i = 0;  
        }
    }

    int palabrasTotales = palabrasParalelas;  


    if (palabrasTotales == palabrasSecuenciales) {  //Compara los totales de palabras secuencial y paralelo
        cout << "El numero de palabras calculado por el programa es correcto: " << palabrasTotales << endl;
    }
    else {
        cout << "El numero de palabras calculado por el programa no coincide con el calculo secuencial." << endl;
    }

    int vocales = 0;  
    texto = textoO;   //Vuelve al texto original

    //Bloque para contar vocales de forma paralela
#pragma omp parallel for reduction(+:vocales)
    for (int i = 0; i < texto.size(); i++) {
        if (texto[i] == '\n') {  
            string seccion = texto.substr(0, i);  
            vocales += contarVocales(seccion);     // Cuenta vocales en la sección y suma al contador paralelo
            texto = texto.substr(i + 1);          
            i = 0;  
        }
    }


    cout << "La cantidad de vocales en el texto es: " << vocales << endl;
    cout << "Secuencial: La cantidad de palabras en el texto es: " << palabrasSecuenciales << endl;
    cout << "Paralela: La cantidad de palabras en el texto es: " << palabrasParalelas << endl;

    return 0;  
}
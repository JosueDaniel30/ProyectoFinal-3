#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <cstdlib>
using namespace std;

//Función Abrir Archivo
void cargarDiccionario(map<string, string>& diccionario, const string& archivo) {
    ifstream file(archivo);
    if (!file) {
        cout << "Archivo no encontrado.\n";
        ofstream newFile(archivo);
        newFile.close();
        return;
    }

    string linea, palabra, traduccion;
    while (getline(file, linea)) {
        size_t pos = linea.find(':');
        if (pos != string::npos) {
            palabra = linea.substr(0, pos);
            traduccion = linea.substr(pos + 1);
            diccionario[palabra] = traduccion;
        }
    }
    file.close();
}

//GuardarArchivo de Traducciones
void guardarDiccionario(map<string, string>& diccionario, const string& archivo) {
    ofstream file(archivo);
    for (auto& par : diccionario) {
        file << par.first << ":" << par.second << endl;
    }
    file.close();
}

// Funcion eSpeak
void hablar(string texto, string idioma) {
    string comando = "espeak -v " + idioma + " \"" + texto + "\"";
    cout << "Ejecutando: " << comando << endl;
    system(comando.c_str());
}


// Menu para elegir idioma
string seleccionarIdioma(map<int, string>& opcionesIdioma, map<string, string>& idiomas) {
    int opcion;
    do {
        cout <<  "Menu para Seleccionar Idioma :)";
        cout << "\nSelecciona un idioma:\n";
        for (auto& par : opcionesIdioma) {
            cout << par.first << ". " << par.second << endl;
        }
        cout << "Elige una opcion: ";
        cin >> opcion;
    } while (opcionesIdioma.find(opcion) == opcionesIdioma.end());

    return idiomas[opcionesIdioma[opcion]];
}

int main() {
    map<string, string> diccionario;
    map<string, string> idiomas = {
        {"Ingles", "en"}, {"Frances", "fr"}, {"Aleman", "de"}, {"Italiano", "it"}
    };
    map<int, string> opcionesIdioma = {
        {1, "Ingles"}, {2, "Frances"}, {3, "Aleman"}, {4, "Italiano"}
    };

    string idioma = seleccionarIdioma(opcionesIdioma, idiomas);
    string archivo = "spa-" + idioma + ".txt";
    cargarDiccionario(diccionario, archivo);
    cout << "Traductor de Espanol a " + idioma + "\n";

    cin.ignore();

    int opcion;
    do {
        cout << "Menu de Opciones Traductor :)";
        cout << "\n1. Traducir palabra\n2. Agregar palabra\n3. Eliminar palabra\n4. Cambiar idioma\n5. Ver Traducciones\n6. Salir\nElige opcion: ";
        cin >> opcion;
        cin.ignore();

        if (opcion == 1) {
            string palabra;
            cout << "Palabra a traducir: ";
            getline(cin, palabra);

            if (diccionario.find(palabra) != diccionario.end()) {
                string traduccion = diccionario[palabra];
                cout << "Traduccion: " << traduccion << endl;
                hablar(traduccion, idioma);
            } else {
                cout << "Palabra no encontrada.\n";
            }
        }
        else if (opcion == 2) {
            string palabra, traduccion;
            cout << "Nueva palabra: ";
            getline(cin, palabra);
            cout << "Traduccion: ";
            getline(cin, traduccion);

            diccionario[palabra] = traduccion;
            guardarDiccionario(diccionario, archivo);
            cout << "Palabra agregada con exito.\n";
        }
        else if (opcion == 3) {
            string palabra;
            cout << "Palabra a eliminar: ";
            getline(cin, palabra);

            if (diccionario.erase(palabra)) {
                guardarDiccionario(diccionario, archivo);
                cout << "Palabra eliminada con exito.\n";
            } else {
                cout << "Palabra no encontrada.\n";
            }
        }
        else if (opcion == 4) {
            idioma = seleccionarIdioma(opcionesIdioma, idiomas);
            archivo = "spa-" + idioma + ".txt";
            diccionario.clear();
            cargarDiccionario(diccionario, archivo);
        }
        else if (opcion == 5) {
            cout << "Palabras en el diccionario:\n";
            for (auto& par : diccionario) {
                cout << par.first << " -> " << par.second << endl;
            }
        }


    } while (opcion != 6);

    return 0;
}

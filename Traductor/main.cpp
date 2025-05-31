/*#include <iostream>
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
    //cout << "Ejecutando: " << comando << endl;
    system(comando.c_str());
}


// Menu para elegir idioma
string seleccionarIdioma(map<int, string>& opcionesIdioma, map<string, string>& idiomas) {
    int opcion;
    do {
        cout <<  "Menu para Seleccionar Idioma :)\n";
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
    //cout << "Traductor de Espanol a " + idioma + "\n";

    cin.ignore();

    int opcion;
    do {
        cout << "Menu de Opciones Traductor :)\n";
        cout << "\n1. Traducir palabra\n2. Agregar palabra\n3. Eliminar palabra\n4. Cambiar idioma\n5. Ver Traducciones\n6. Salir\nElige opcion:";
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
}*/
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <map>
#include <vector>
#include <algorithm>
#include <filesystem>
#include <conio.h>
using namespace std;
namespace fs = std::filesystem;

// --- AVL ---
struct Nodo {
    string palabra, traduccion;
    Nodo* izq;
    Nodo* der;
    int altura;
    Nodo(string p, string t) : palabra(p), traduccion(t), izq(nullptr), der(nullptr), altura(1) {}
};

int altura(Nodo* n) { return n ? n->altura : 0; }
int balance(Nodo* n) { return n ? altura(n->izq) - altura(n->der) : 0; }
void actualizarAltura(Nodo* n) {
    if (n) n->altura = 1 + max(altura(n->izq), altura(n->der));
}

Nodo* rotarDerecha(Nodo* y) {
    Nodo* x = y->izq;
    Nodo* T2 = x->der;
    x->der = y;
    y->izq = T2;
    actualizarAltura(y);
    actualizarAltura(x);
    return x;
}

Nodo* rotarIzquierda(Nodo* x) {
    Nodo* y = x->der;
    Nodo* T2 = y->izq;
    y->izq = x;
    x->der = T2;
    actualizarAltura(x);
    actualizarAltura(y);
    return y;
}

Nodo* insertar(Nodo* nodo, string palabra, string traduccion) {
    if (!nodo) return new Nodo(palabra, traduccion);
    if (palabra < nodo->palabra) nodo->izq = insertar(nodo->izq, palabra, traduccion);
    else if (palabra > nodo->palabra) nodo->der = insertar(nodo->der, palabra, traduccion);
    else { nodo->traduccion = traduccion; return nodo; }

    actualizarAltura(nodo);
    int b = balance(nodo);
    if (b > 1 && palabra < nodo->izq->palabra) return rotarDerecha(nodo);
    if (b < -1 && palabra > nodo->der->palabra) return rotarIzquierda(nodo);
    if (b > 1 && palabra > nodo->izq->palabra) { nodo->izq = rotarIzquierda(nodo->izq); return rotarDerecha(nodo); }
    if (b < -1 && palabra < nodo->der->palabra) { nodo->der = rotarDerecha(nodo->der); return rotarIzquierda(nodo); }
    return nodo;
}

string buscar(Nodo* nodo, const string& palabra) {
    if (!nodo) return "";
    if (palabra == nodo->palabra) return nodo->traduccion;
    else if (palabra < nodo->palabra) return buscar(nodo->izq, palabra);
    else return buscar(nodo->der, palabra);
}

void imprimirDiccionario(Nodo* nodo) {
    if (nodo) {
        imprimirDiccionario(nodo->izq);
        cout << nodo->palabra << " -> " << nodo->traduccion << endl;
        imprimirDiccionario(nodo->der);
    }
}

void guardarDiccionario(Nodo* nodo, ofstream& file) {
    if (nodo) {
        guardarDiccionario(nodo->izq, file);
        file << nodo->palabra << ":" << nodo->traduccion << endl;
        guardarDiccionario(nodo->der, file);
    }
}

void cargarDiccionario(Nodo*& raiz, const string& archivo) {
    ifstream file(archivo);
    if (!file) { ofstream nuevo(archivo); nuevo.close(); return; }
    string linea;
    while (getline(file, linea)) {
        size_t pos = linea.find(':');
        if (pos != string::npos) {
            string palabra = linea.substr(0, pos);
            string traduccion = linea.substr(pos + 1);
            raiz = insertar(raiz, palabra, traduccion);
        }
    }
    file.close();
}

void hablar(string texto, string idioma) {
    string comando = "espeak -v " + idioma + " \"" + texto + "\"";
    system(comando.c_str());
}

string seleccionarIdioma(map<int, string>& opciones, map<string, string>& idiomas) {
    int op;
    do {
        cout << "\nSelecciona un idioma:\n";
        for (auto& p : opciones) cout << p.first << ". " << p.second << endl;
        cout << "Opcion: "; cin >> op;
    } while (opciones.find(op) == opciones.end());
    return idiomas[opciones[op]];
}

string encriptar(const string& palabra) {
    map<char, string> clave;
    string vocales = "aeiouAEIOU";
    for (int i = 0; i < 10; ++i) clave[vocales[i]] = "U" + to_string((i % 5) + 1);
    string min = "bcdfghjklmnpqrstvwxyz";
    for (int i = 0; i < min.size(); ++i) clave[min[i]] = "m" + to_string(i + 1);
    string may = "BCDFGHJKLMNPQRSTVWXYZ";
    for (int i = 0; i < may.size(); ++i) clave[may[i]] = "g" + to_string(i + 1);

    string enc;
    for (char c : palabra) enc += clave.count(c) ? clave[c] : string(1, c);
    return enc;
}

void mostrarTopPalabras(map<string, int>& historial) {
    vector<pair<string, int>> v(historial.begin(), historial.end());
    sort(v.begin(), v.end(), [](auto& a, auto& b) { return a.second > b.second; });
    cout << "\nTop de palabras buscadas:\n";
    for (int i = 0; i < min(5, (int)v.size()); ++i)
        cout << v[i].first << " - " << encriptar(v[i].first) << " - " << v[i].second << " veces\n";
}
string leerContrasenaOculta() {
    string pass = "";
    char ch;
    while ((ch = _getch()) != '\r') {
        if (ch == '\b' && !pass.empty()) {
            pass.pop_back();
            cout << "\b \b";
        } else if (isprint(ch)) {
            pass += ch;
            cout << '*';
        }
    }
    cout << endl;
    return pass;
}

bool registrarUsuario(string& usuarioActivo) {
    cout << "\n--- REGISTRO ---\nUsuario: ";
    string user; cin >> user;
    cout << "Contrasena: ";
    string pass = leerContrasenaOculta();

    string dir = "usuarios/" + user;
    if (fs::exists(dir)) {
        cout << "\nUsuario ya existe.\n";
        return false;
    }
    fs::create_directories(dir);
    ofstream datos(dir + "/usuario.txt");
    datos << user << endl;
    datos << encriptar(pass) << endl;
    datos.close();
    usuarioActivo = user;
    return true;
}

bool iniciarSesion(string& usuarioActivo) {
    cout << "\n--- INICIAR SESION ---\nUsuario: ";
    string user; cin >> user;
    string dir = "usuarios/" + user + "/usuario.txt";
    ifstream datos(dir);
    if (!datos.is_open()) {
        cout << "Usuario no encontrado.\n";
        return false;
    }
    string usuario, contraEncriptada;
    getline(datos, usuario);
    getline(datos, contraEncriptada);
    datos.close();
    cout << "Contrasena: ";
    string pass = leerContrasenaOculta();
    if (encriptar(pass) == contraEncriptada) {
        usuarioActivo = user;
        return true;
    } else {
        cout << "Contrasena incorrecta.\n";
        return false;
    }
}

void menuUsuario(string usuario) {
    Nodo* raiz = nullptr;
    map<string, string> idiomas = { {"Ingles", "en"}, {"Frances", "fr"}, {"Aleman", "de"}, {"Italiano", "it"} };
    map<int, string> opciones = { {1, "Ingles"}, {2, "Frances"}, {3, "Aleman"}, {4, "Italiano"} };
    map<string, int> historial;
    vector<string> originales, encriptadas;

    string idioma = seleccionarIdioma(opciones, idiomas);
    string archivo = "spa-" + idioma + ".txt";
    cargarDiccionario(raiz, archivo);
    cin.ignore();

    int opcion;
    do {
        cout << "\n--- MENU TRADUCTOR GRUPO #4  ---\n";
        cout << "\n1. Traducir palabra\n2. Agregar\n3. Eliminar\n4. Cambiar idioma\n5. Ver todo\n6. Ver top buscadas\n7. Salir\nOpcion: ";
        cin >> opcion; cin.ignore();

        if (opcion == 1) {
            string palabra;
            cout << "Palabra: "; getline(cin, palabra);
            string resultado = buscar(raiz, palabra);
            if (resultado.empty()) cout << "No encontrada\n";
            else {
                cout << "Traduccion: " << resultado << endl;
                hablar(resultado, idioma);
                historial[palabra]++;
                originales.push_back(palabra);
                encriptadas.push_back(encriptar(palabra));
            }
        } else if (opcion == 2) {
            string palabra, traduccion;
            cout << "Nueva palabra: "; getline(cin, palabra);
            cout << "Traduccion: "; getline(cin, traduccion);
            raiz = insertar(raiz, palabra, traduccion);
            ofstream file(archivo); guardarDiccionario(raiz, file); file.close();
        } else if (opcion == 3) {
            string palabra;
            cout << "Eliminar palabra: "; getline(cin, palabra);
            if (!buscar(raiz, palabra).empty()) {
                raiz = insertar(raiz, palabra, "");
                ofstream file(archivo); guardarDiccionario(raiz, file); file.close();
            }
        } else if (opcion == 4) {
            idioma = seleccionarIdioma(opciones, idiomas);
            archivo = "spa-" + idioma + ".txt";
            raiz = nullptr;
            cargarDiccionario(raiz, archivo);
        } else if (opcion == 5) {
            imprimirDiccionario(raiz);
        } else if (opcion == 6) {
            mostrarTopPalabras(historial);
        }
    } while (opcion != 7);

    string dir = "usuarios/" + usuario + "/";
    ofstream clave(dir + "llave_" + usuario + ".ukey"); clave << "Umg_" + usuario; clave.close();
    ofstream enc(dir + "encriptado_" + usuario + ".utr"); for (auto& e : encriptadas) enc << e << "\n"; enc.close();
    ofstream orig(dir + "original_" + usuario + ".utr"); for (auto& o : originales) orig << o << "\n"; orig.close();
}

int main() {
    fs::create_directories("usuarios");
    string usuario;
    int inicio;
    do {
        cout << "\n--- BIENVENIDO ---\n1. Registrar\n2. Iniciar Sesion\n3. Salir\nOpcion: ";
        cin >> inicio;
        if (inicio == 1 && registrarUsuario(usuario)) menuUsuario(usuario);
        else if (inicio == 2 && iniciarSesion(usuario)) menuUsuario(usuario);
    } while (inicio != 3);
    return 0;
} // FIN

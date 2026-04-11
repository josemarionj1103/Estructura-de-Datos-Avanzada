// Progra-Villa_Zombies.cpp
// Sistema de navegación para sobrevivientes en ciudad zombie

#include <iostream>
#include <fstream>
#include <string>
#include <climits>
using namespace std;

// =====================================================================
// ESTRUCTURAS BASE
// =====================================================================

struct Arista {
    int destino;
    int peligro;
    Arista* sgte;
};

struct Nodo {
    string nombre;
    bool zonaRoja;
    Arista* ady;
    Nodo* sgte;
};

typedef Nodo* TNodo;
typedef Arista* TArista;

// =====================================================================
// COLA SIMPLE (para BFS)
// =====================================================================
struct ColaNodo {
    int valor;
    ColaNodo* sgte;
};

struct Cola {
    ColaNodo* frente;
    ColaNodo* final;

    Cola() { frente = final = NULL; }

    bool estaVacia() { return frente == NULL; }

    void encolar(int v) {
        ColaNodo* nuevo = new ColaNodo;
        nuevo->valor = v;
        nuevo->sgte = NULL;
        if (final == NULL) {
            frente = final = nuevo;
        } else {
            final->sgte = nuevo;
            final = nuevo;
        }
    }

    int desencolar() {
        if (estaVacia()) return -1;
        int val = frente->valor;
        ColaNodo* tmp = frente;
        frente = frente->sgte;
        if (frente == NULL) final = NULL;
        delete tmp;
        return val;
    }
};

// =====================================================================
// PILA SIMPLE (para DFS)
// =====================================================================
struct PilaNodo {
    int valor;
    PilaNodo* sgte;
};

struct Pila {
    PilaNodo* tope;

    Pila() { tope = NULL; }

    bool estaVacia() { return tope == NULL; }

    void apilar(int v) {
        PilaNodo* nuevo = new PilaNodo;
        nuevo->valor = v;
        nuevo->sgte = tope;
        tope = nuevo;
    }

    int desapilar() {
        if (estaVacia()) return -1;
        int val = tope->valor;
        PilaNodo* tmp = tope;
        tope = tope->sgte;
        delete tmp;
        return val;
    }
};

// =====================================================================
// GRAFO DE PROGRA-VILLA
// =====================================================================

const int MAX_NODOS = 50;

class GrafoProgravilla {
private:
    TNodo nodos[MAX_NODOS];
    int totalNodos;

    int buscarIndice(string nombre) {
        for (int i = 0; i < totalNodos; i++)
            if (nodos[i]->nombre == nombre) return i;
        return -1;
    }

    void insertarNodo(string nombre, bool zonaRoja) {
        if (totalNodos >= MAX_NODOS) {
            cout << "Limite de nodos alcanzado!\n";
            return;
        }
        TNodo nuevo = new Nodo;
        nuevo->nombre = nombre;
        nuevo->zonaRoja = zonaRoja;
        nuevo->ady = NULL;
        nuevo->sgte = NULL;
        nodos[totalNodos++] = nuevo;
    }

    void agregarArista(int desde, int hasta, int peligro) {
        TArista nueva = new Arista;
        nueva->destino = hasta;
        nueva->peligro = peligro;
        nueva->sgte = NULL;
        if (nodos[desde]->ady == NULL) {
            nodos[desde]->ady = nueva;
        } else {
            TArista ar = nodos[desde]->ady;
            while (ar->sgte != NULL) ar = ar->sgte;
            ar->sgte = nueva;
        }
    }

    void dfsConexo(int v, bool visitado[]) {
        visitado[v] = true;
        TArista ar = nodos[v]->ady;
        while (ar != NULL) {
            if (!visitado[ar->destino])
                dfsConexo(ar->destino, visitado);
            ar = ar->sgte;
        }
    }

public:
    GrafoProgravilla() { totalNodos = 0; }

    // ------------------------------------------------------------------
    // LEER MAPA DESDE ARCHIVO
    // ------------------------------------------------------------------
    bool leerDesdeArchivo(string nombreArchivo) {
        ifstream archivo(nombreArchivo.c_str(), ios::in);
        if (!archivo.good()) {
            cout << "ERROR: No se pudo abrir el archivo '" << nombreArchivo << "'\n";
            return false;
        }

        string linea;
        bool leyendoNodos = false, leyendoAristas = false;

        while (getline(archivo, linea)) {
            if (linea == "NODOS") {
                leyendoNodos = true;
                leyendoAristas = false;
                continue;
            }
            if (linea == "ARISTAS") {
                leyendoAristas = true;
                leyendoNodos = false;
                continue;
            }
            if (linea.empty()) continue;

            if (leyendoNodos) {
                string nombre, tipo;
                int pos = linea.find(' ');
                if (pos != (int)string::npos) {
                    nombre = linea.substr(0, pos);
                    tipo = linea.substr(pos + 1);
                } else {
                    nombre = linea;
                    tipo = "NORMAL";
                }
                bool esRoja = (tipo == "ROJA");
                insertarNodo(nombre, esRoja);
            } else if (leyendoAristas) {
                string origen, destino;
                int peligro;
                int p1 = linea.find(' ');
                int p2 = linea.find(' ', p1 + 1);
                if (p1 == (int)string::npos || p2 == (int)string::npos) continue;
                origen  = linea.substr(0, p1);
                destino = linea.substr(p1 + 1, p2 - p1 - 1);
                peligro = atoi(linea.substr(p2 + 1).c_str());

                int idx1 = buscarIndice(origen);
                int idx2 = buscarIndice(destino);
                if (idx1 != -1 && idx2 != -1) {
                    agregarArista(idx1, idx2, peligro);
                    agregarArista(idx2, idx1, peligro);
                } else {
                    cout << "ADVERTENCIA: Nodo no encontrado en arista: "
                         << origen << " - " << destino << "\n";
                }
            }
        }
        archivo.close();
        cout << "Mapa cargado: " << totalNodos << " nodos.\n";
        return true;
    }

    // ------------------------------------------------------------------
    // TAREA 1: BFS - Exploración de suministros
    // ------------------------------------------------------------------
    void buscarSuministrosBFS(string nombreInicio, string nombreObjetivo) {
        int inicio   = buscarIndice(nombreInicio);
        int objetivo = buscarIndice(nombreObjetivo);

        if (inicio == -1 || objetivo == -1) {
            cout << "Nodo no encontrado en el mapa.\n";
            return;
        }

        bool visitado[MAX_NODOS] = {false};
        int  padre[MAX_NODOS];
        for (int i = 0; i < MAX_NODOS; i++) padre[i] = -1;

        Cola cola;
        visitado[inicio] = true;
        cola.encolar(inicio);
        bool encontrado = false;

        while (!cola.estaVacia()) {
            int actual = cola.desencolar();
            if (actual == objetivo) { encontrado = true; break; }
            TArista ar = nodos[actual]->ady;
            while (ar != NULL) {
                if (!visitado[ar->destino]) {
                    visitado[ar->destino] = true;
                    padre[ar->destino] = actual;
                    cola.encolar(ar->destino);
                }
                ar = ar->sgte;
            }
        }

        cout << "\n=== BFS: EXPLORACION DE SUMINISTROS ===\n";
        if (encontrado) {
            int camino[MAX_NODOS], longitud = 0;
            int actual = objetivo;
            while (actual != -1) { camino[longitud++] = actual; actual = padre[actual]; }
            cout << "Ruta encontrada (" << longitud - 1 << " calles):\n";
            for (int i = longitud - 1; i >= 0; i--) {
                cout << "  [" << nodos[camino[i]]->nombre << "]";
                if (i > 0) cout << " --> ";
            }
            cout << "\nLlegaste a " << nombreObjetivo << " con exito!\n";
        } else {
            cout << "No hay ruta desde " << nombreInicio << " hasta " << nombreObjetivo << "\n";
        }
    }

    // ------------------------------------------------------------------
    // TAREA 2: DFS - Ruta de rescate evitando Zonas Rojas
    // ------------------------------------------------------------------
    void esRutaSeguraDFS(string nombreInicio, string nombreFin) {
        int inicio = buscarIndice(nombreInicio);
        int fin    = buscarIndice(nombreFin);

        if (inicio == -1 || fin == -1) {
            cout << "Nodo no encontrado en el mapa.\n";
            return;
        }
        if (nodos[inicio]->zonaRoja || nodos[fin]->zonaRoja) {
            cout << "El punto de inicio o fin es Zona Roja. Imposible usar.\n";
            return;
        }

        bool visitado[MAX_NODOS] = {false};
        int  padre[MAX_NODOS];
        for (int i = 0; i < MAX_NODOS; i++) padre[i] = -1;

        Pila pila;
        pila.apilar(inicio);
        bool encontrado = false;

        while (!pila.estaVacia()) {
            int actual = pila.desapilar();
            if (visitado[actual]) continue;
            visitado[actual] = true;
            if (actual == fin) { encontrado = true; break; }
            TArista ar = nodos[actual]->ady;
            while (ar != NULL) {
                int vecino = ar->destino;
                if (!visitado[vecino] && !nodos[vecino]->zonaRoja) {
                    padre[vecino] = actual;
                    pila.apilar(vecino);
                }
                ar = ar->sgte;
            }
        }

        cout << "\n=== DFS: RUTA DE RESCATE (EVITANDO ZONAS ROJAS) ===\n";
        if (encontrado) {
            int camino[MAX_NODOS], longitud = 0;
            int actual = fin;
            while (actual != -1) { camino[longitud++] = actual; actual = padre[actual]; }
            cout << "Ruta SEGURA encontrada:\n";
            for (int i = longitud - 1; i >= 0; i--) {
                cout << "  [" << nodos[camino[i]]->nombre << "]";
                if (i > 0) cout << " --> ";
            }
            cout << "\nRuta de rescate disponible!\n";
        } else {
            cout << "NO existe ruta segura de " << nombreInicio << " a " << nombreFin << "\n";
            cout << "Los zombies han bloqueado todos los caminos.\n";
        }
    }

    // ------------------------------------------------------------------
    // TAREA 3: COMPONENTES CONEXAS
    // ------------------------------------------------------------------
    void verificarConectividad() {
        cout << "\n=== MAPA DE CONECTIVIDAD ===\n";

        bool visitado[MAX_NODOS] = {false};
        int componentes = 0;

        for (int i = 0; i < totalNodos; i++) {
            if (!visitado[i]) {
                componentes++;
                cout << "Componente " << componentes << ": ";
                dfsConexo(i, visitado);

                bool tmp[MAX_NODOS] = {false};
                for (int k = 0; k < i; k++) tmp[k] = true;

                Pila pila;
                pila.apilar(i);
                while (!pila.estaVacia()) {
                    int actual = pila.desapilar();
                    if (tmp[actual]) continue;
                    tmp[actual] = true;
                    cout << nodos[actual]->nombre << " ";
                    TArista ar = nodos[actual]->ady;
                    while (ar != NULL) {
                        if (!tmp[ar->destino]) pila.apilar(ar->destino);
                        ar = ar->sgte;
                    }
                }
                cout << "\n";
            }
        }

        if (componentes == 1)
            cout << "\nLa ciudad sigue CONECTADA. Todos los refugios son alcanzables.\n";
        else {
            cout << "\nATENCION: La ciudad tiene " << componentes << " zonas aisladas.\n";
            cout << "Algunos refugios han quedado INACCESIBLES.\n";
        }
    }

    // ------------------------------------------------------------------
    // TAREA 4: DIJKSTRA - Ruta con menor peligro
    // ------------------------------------------------------------------
    void dijkstra(string nombreInicio, string nombreObjetivo) {
        int inicio   = buscarIndice(nombreInicio);
        int objetivo = buscarIndice(nombreObjetivo);

        if (inicio == -1 || objetivo == -1) {
            cout << "Nodo no encontrado en el mapa.\n";
            return;
        }

        int  dist[MAX_NODOS];
        int  padre[MAX_NODOS];
        bool visitado[MAX_NODOS];

        for (int i = 0; i < totalNodos; i++) {
            dist[i]     = INT_MAX;
            padre[i]    = -1;
            visitado[i] = false;
        }
        dist[inicio] = 0;

        for (int iter = 0; iter < totalNodos; iter++) {
            int u = -1;
            for (int i = 0; i < totalNodos; i++)
                if (!visitado[i] && (u == -1 || dist[i] < dist[u])) u = i;
            if (u == -1 || dist[u] == INT_MAX) break;
            visitado[u] = true;

            TArista ar = nodos[u]->ady;
            while (ar != NULL) {
                int v = ar->destino;
                int nuevaDist = dist[u] + ar->peligro;
                if (nuevaDist < dist[v]) {
                    dist[v]  = nuevaDist;
                    padre[v] = u;
                }
                ar = ar->sgte;
            }
        }

        cout << "\n=== DIJKSTRA: RUTA CON MENOR PELIGRO ===\n";
        if (dist[objetivo] == INT_MAX) {
            cout << "No existe ruta de " << nombreInicio << " a " << nombreObjetivo << "\n";
        } else {
            int camino[MAX_NODOS], longitud = 0;
            int actual = objetivo;
            while (actual != -1) { camino[longitud++] = actual; actual = padre[actual]; }
            cout << "Ruta mas segura (" << dist[objetivo] << " zombies en total):\n";
            for (int i = longitud - 1; i >= 0; i--) {
                cout << "  [" << nodos[camino[i]]->nombre << "]";
                if (i > 0) cout << " --> ";
            }
            cout << "\nNivel de peligro total: " << dist[objetivo] << " zombies\n";
        }
    }

    // ------------------------------------------------------------------
    // TAREA 5: ARBOL DE CAMINOS DESDE UN NODO
    // ------------------------------------------------------------------
    void mostrarArbolCaminos(string nombreInicio) {
        int inicio = buscarIndice(nombreInicio);
        if (inicio == -1) {
            cout << "Nodo no encontrado en el mapa.\n";
            return;
        }

        bool visitado[MAX_NODOS] = {false};
        int  padre[MAX_NODOS];
        int  nivel[MAX_NODOS];
        for (int i = 0; i < MAX_NODOS; i++) { padre[i] = -1; nivel[i] = -1; }

        Cola cola;
        visitado[inicio] = true;
        nivel[inicio]    = 0;
        cola.encolar(inicio);

        while (!cola.estaVacia()) {
            int actual = cola.desencolar();
            TArista ar = nodos[actual]->ady;
            while (ar != NULL) {
                if (!visitado[ar->destino]) {
                    visitado[ar->destino]  = true;
                    padre[ar->destino]     = actual;
                    nivel[ar->destino]     = nivel[actual] + 1;
                    cola.encolar(ar->destino);
                }
                ar = ar->sgte;
            }
        }

        int maxNivel = 0;
        for (int i = 0; i < totalNodos; i++)
            if (nivel[i] > maxNivel) maxNivel = nivel[i];

        cout << "\n=== ARBOL DE CAMINOS DESDE: " << nombreInicio << " ===\n\n";

        for (int niv = 0; niv <= maxNivel; niv++) {
            if (niv == 0) {
                cout << "[" << nodos[inicio]->nombre << "]";
                if (nodos[inicio]->zonaRoja) cout << " !!ZONA ROJA!!";
                cout << "\n";
            } else {
                for (int i = 0; i < totalNodos; i++) {
                    if (nivel[i] == niv) {
                        for (int sp = 0; sp < niv * 4; sp++) cout << " ";
                        cout << "|-- [" << nodos[i]->nombre << "]";
                        if (padre[i] != -1) {
                            TArista ar = nodos[padre[i]]->ady;
                            while (ar != NULL) {
                                if (ar->destino == i) {
                                    cout << " (peligro: " << ar->peligro << ")";
                                    break;
                                }
                                ar = ar->sgte;
                            }
                        }
                        if (nodos[i]->zonaRoja) cout << " !!ZONA ROJA!!";
                        cout << "\n";
                    }
                }
            }
        }

        cout << "\n";
        bool hayAislados = false;
        for (int i = 0; i < totalNodos; i++) {
            if (!visitado[i]) {
                if (!hayAislados) {
                    cout << "Nodos NO alcanzables desde " << nombreInicio << ":\n";
                    hayAislados = true;
                }
                cout << "  [X] " << nodos[i]->nombre << "\n";
            }
        }
        if (!hayAislados)
            cout << "Todos los nodos son alcanzables desde " << nombreInicio << ".\n";
    }

    // ------------------------------------------------------------------
    // MOSTRAR MAPA COMPLETO
    // ------------------------------------------------------------------
    void mostrarMapa() {
        cout << "\n=== MAPA DE PROGRA-VILLA ===\n";
        cout << "Nodo            | Estado    | Conexiones\n";
        cout << "----------------|-----------|------------------------------\n";
        for (int i = 0; i < totalNodos; i++) {
            cout << nodos[i]->nombre;
            for (int j = nodos[i]->nombre.length(); j < 16; j++) cout << " ";
            cout << "| ";
            cout << (nodos[i]->zonaRoja ? "ZONA ROJA  | " : "Seguro     | ");
            TArista ar = nodos[i]->ady;
            if (ar == NULL) cout << "(sin conexiones)";
            while (ar != NULL) {
                cout << nodos[ar->destino]->nombre << "(p:" << ar->peligro << ") ";
                ar = ar->sgte;
            }
            cout << "\n";
        }
    }

    // ------------------------------------------------------------------
    // CREAR ARCHIVO DE EJEMPLO
    // ------------------------------------------------------------------
    void crearArchivoEjemplo(string nombre) {
        ofstream f(nombre.c_str(), ios::out);
        if (!f.good()) { cout << "Error al crear archivo de ejemplo.\n"; return; }
        f << "NODOS\n";
        f << "Refugio_Inicial NORMAL\n";
        f << "Supermercado NORMAL\n";
        f << "Farmacia NORMAL\n";
        f << "Hospital NORMAL\n";
        f << "Estacion_Policia NORMAL\n";
        f << "Zona_Norte NORMAL\n";
        f << "Zona_Sur NORMAL\n";
        f << "Calle_Peligrosa ROJA\n";
        f << "Centro NORMAL\n";
        f << "ARISTAS\n";
        f << "Refugio_Inicial Supermercado 3\n";
        f << "Refugio_Inicial Farmacia 7\n";
        f << "Supermercado Hospital 2\n";
        f << "Farmacia Hospital 5\n";
        f << "Farmacia Estacion_Policia 4\n";
        f << "Hospital Centro 1\n";
        f << "Estacion_Policia Centro 6\n";
        f << "Zona_Norte Calle_Peligrosa 2\n";
        f << "Calle_Peligrosa Zona_Sur 3\n";
        f << "Zona_Norte Centro 8\n";
        f << "Centro Zona_Sur 4\n";
        f.close();
        cout << "Archivo '" << nombre << "' creado exitosamente.\n";
    }

    int getTotalNodos() { return totalNodos; }
    string getNombre(int i) {
        if (i >= 0 && i < totalNodos) return nodos[i]->nombre;
        return "";
    }
};

// =====================================================================
// MENU
// =====================================================================
void menu() {
    cout << "\n\t=== PROGRA-VILLA: SISTEMA DE SOBREVIVENCIA ===\n\n";
    cout << " 1. Cargar mapa desde archivo\n";
    cout << " 2. Crear archivo de mapa de ejemplo\n";
    cout << " 3. Mostrar mapa de la ciudad\n";
    cout << " 4. BFS  - Ruta con menos calles (Exploracion)\n";
    cout << " 5. DFS  - Ruta segura evitando Zonas Rojas\n";
    cout << " 6. Verificar conectividad (refugios aislados)\n";
    cout << " 7. Dijkstra - Ruta con menor peligro\n";
    cout << " 8. Ver arbol de caminos desde un punto\n";
    cout << " 0. Salir\n";
    cout << "\n INGRESE OPCION: ";
}

// =====================================================================
// MAIN
// =====================================================================
int main() {
    system("color 0a");
    GrafoProgravilla ciudad;
    bool mapaListo = false;
    int  op;
    string archivo, origen, destino;

    cout << "\n\t********************************************\n";
    cout << "\t*   PROGRA-VILLA - SISTEMA ZOMBIES v1.0   *\n";
    cout << "\t*  Ayuda a los sobrevivientes a escapar!  *\n";
    cout << "\t********************************************\n";

    do {
        menu();
        cin >> op;

        switch (op) {
            case 1:
                cout << "Nombre del archivo: ";
                cin >> archivo;
                mapaListo = ciudad.leerDesdeArchivo(archivo);
                break;

            case 2:
                cout << "Nombre del archivo a crear (ej: mapa.txt): ";
                cin >> archivo;
                ciudad.crearArchivoEjemplo(archivo);
                cout << "Ahora puedes cargarlo con la opcion 1.\n";
                break;

            case 3:
                if (!mapaListo) { cout << "Primero carga el mapa (opcion 1).\n"; break; }
                ciudad.mostrarMapa();
                break;

            case 4:
                if (!mapaListo) { cout << "Primero carga el mapa (opcion 1).\n"; break; }
                cout << "Nodo de inicio: ";
                cin >> origen;
                cout << "Nodo objetivo : ";
                cin >> destino;
                ciudad.buscarSuministrosBFS(origen, destino);
                break;

            case 5:
                if (!mapaListo) { cout << "Primero carga el mapa (opcion 1).\n"; break; }
                cout << "Nodo de inicio: ";
                cin >> origen;
                cout << "Nodo destino  : ";
                cin >> destino;
                ciudad.esRutaSeguraDFS(origen, destino);
                break;

            case 6:
                if (!mapaListo) { cout << "Primero carga el mapa (opcion 1).\n"; break; }
                ciudad.verificarConectividad();
                break;

            case 7:
                if (!mapaListo) { cout << "Primero carga el mapa (opcion 1).\n"; break; }
                cout << "Nodo de inicio: ";
                cin >> origen;
                cout << "Nodo objetivo : ";
                cin >> destino;
                ciudad.dijkstra(origen, destino);
                break;

            case 8:
                if (!mapaListo) { cout << "Primero carga el mapa (opcion 1).\n"; break; }
                cout << "Nodo de inicio para el arbol: ";
                cin >> origen;
                ciudad.mostrarArbolCaminos(origen);
                break;

            case 0:
                cout << "\nBuena suerte sobreviviente. Adios!\n";
                break;

            default:
                cout << "OPCION NO VALIDA...!!!\n";
        }

        cout << "\n";
        if (op != 0) system("pause");
        if (op != 0) system("cls");

    } while (op != 0);

    return 0;
}

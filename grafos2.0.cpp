#include <iostream>
#include <conio.h>
#include <queue>
#include <map>

struct arista;

struct nodo {
    char nombre;
    struct nodo* sgte;
    struct arista* ady;
};

struct arista {
    struct nodo* destino;
    struct arista* sgte;
};

typedef struct nodo* Tnodo;
typedef struct arista* Tarista;

Tnodo p;

// PROTOTIPOS
void menu();
void insertar_nodo();
void agrega_arista(Tnodo&, Tnodo&, Tarista&);
void insertar_arista();
void mostrar_grafo();
void recorre_grafo_en_amplitud();
void recorre_grafo_en_profundidad();
void dfs(Tnodo, std::map<Tnodo, bool>&);
void dijkstra();
void warshall();
void floyd();

// MAIN
int main()
{
    p = NULL;
    int op;

    do {
        menu();
        std::cin >> op;

        switch (op)
        {
        case 1: insertar_nodo(); break;
        case 2: insertar_arista(); break;
        case 3: mostrar_grafo(); break;
        case 4: recorre_grafo_en_amplitud(); break;
        case 5: recorre_grafo_en_profundidad(); break;
        case 6: dijkstra(); break;
        case 7: warshall(); break;
        case 8: floyd(); break;
        }
        std::cout << "\n\n";
        system("pause");
        system("cls");

    } while (op != 0);

    return 0;
}

// MENU
void menu()
{
    std::cout << "\n===== GRAFOS =====\n";
    std::cout << "1. Insertar nodo\n";
    std::cout << "2. Insertar arista\n";
    std::cout << "3. Mostrar grafo\n";
    std::cout << "4. Recorrido en amplitud (BFS)\n";
    std::cout << "5. Recorrido en profundidad (DFS)\n";
    std::cout << "6. Dijkstra\n";
    std::cout << "7. Warshall - Alcanzabilidad\n";
    std::cout << "8. Floyd    - Distancias minimas\n";
    std::cout << "0. Salir\n";
    std::cout << "Opcion: ";
}

// INSERTAR NODO
void insertar_nodo()
{
    Tnodo nuevo = new nodo;

    std::cout << "Ingrese nombre del nodo: ";
    std::cin >> nuevo->nombre;

    nuevo->sgte = NULL;
    nuevo->ady = NULL;

    if (p == NULL)
    {
        p = nuevo;
    }
    else
    {
        Tnodo t = p;
        while (t->sgte != NULL)
            t = t->sgte;

        t->sgte = nuevo;
    }
}

// AGREGAR ARISTA
void agrega_arista(Tnodo& aux, Tnodo& aux2, Tarista& nuevo)
{
    if (aux->ady == NULL)
    {
        aux->ady = nuevo;
    }
    else
    {
        Tarista q = aux->ady;
        while (q->sgte != NULL)
            q = q->sgte;

        q->sgte = nuevo;
    }

    nuevo->destino = aux2;
    nuevo->sgte = NULL;
}

// INSERTAR ARISTA
void insertar_arista()
{
    char ini, fin;
    std::cout << "Nodo inicio: ";
    std::cin >> ini;
    std::cout << "Nodo final: ";
    std::cin >> fin;

    Tnodo aux = p, aux2 = p;

    while (aux != NULL && aux->nombre != ini)
        aux = aux->sgte;

    while (aux2 != NULL && aux2->nombre != fin)
        aux2 = aux2->sgte;

    if (aux == NULL || aux2 == NULL)
    {
        std::cout << "Nodo no encontrado\n";
        return;
    }

    Tarista nuevo = new arista;
    agrega_arista(aux, aux2, nuevo);
}

// MOSTRAR GRAFO
void mostrar_grafo()
{
    Tnodo ptr = p;

    std::cout << "\nNODO | ADYACENTES\n";

    while (ptr != NULL)
    {
        std::cout << ptr->nombre << " -> ";

        Tarista ar = ptr->ady;
        while (ar != NULL)
        {
            std::cout << ar->destino->nombre << " ";
            ar = ar->sgte;
        }

        std::cout << "\n";
        ptr = ptr->sgte;
    }
}

// BFS
void recorre_grafo_en_amplitud()
{
    if (p == NULL)
    {
        std::cout << "Grafo vacio\n";
        return;
    }

    std::queue<Tnodo> cola;
    std::map<Tnodo, bool> visitado;

    cola.push(p);
    visitado[p] = true;

    std::cout << "BFS: ";

    while (!cola.empty())
    {
        Tnodo actual = cola.front();
        cola.pop();

        std::cout << actual->nombre << " ";

        Tarista ar = actual->ady;
        while (ar != NULL)
        {
            if (!visitado[ar->destino])
            {
                cola.push(ar->destino);
                visitado[ar->destino] = true;
            }
            ar = ar->sgte;
        }
    }
}

// DFS AUX
void dfs(Tnodo nodo, std::map<Tnodo, bool>& visitado)
{
    visitado[nodo] = true;
    std::cout << nodo->nombre << " ";

    Tarista ar = nodo->ady;

    while (ar != NULL)
    {
        if (!visitado[ar->destino])
        {
            dfs(ar->destino, visitado);
        }
        ar = ar->sgte;
    }
}

// DFS
void recorre_grafo_en_profundidad()
{
    if (p == NULL)
    {
        std::cout << "Grafo vacio\n";
        return;
    }

    std::map<Tnodo, bool> visitado;

    std::cout << "DFS: ";
    dfs(p, visitado);
}

// DIJKSTRA SIMPLE (sin pesos)
void dijkstra()
{
    if (p == NULL)
    {
        std::cout << "Grafo vacio\n";
        return;
    }

    std::map<Tnodo, int> dist;
    std::queue<Tnodo> cola;

    Tnodo aux = p;

    while (aux != NULL)
    {
        dist[aux] = 9999;
        aux = aux->sgte;
    }

    dist[p] = 0;
    cola.push(p);

    while (!cola.empty())
    {
        Tnodo actual = cola.front();
        cola.pop();

        Tarista ar = actual->ady;

        while (ar != NULL)
        {
            if (dist[ar->destino] > dist[actual] + 1)
            {
                dist[ar->destino] = dist[actual] + 1;
                cola.push(ar->destino);
            }
            ar = ar->sgte;
        }
    }

    std::cout << "Distancias desde " << p->nombre << ":\n";

    aux = p;
    while (aux != NULL)
    {
        std::cout << aux->nombre << " -> " << dist[aux] << "\n";
        aux = aux->sgte;
    }
}

// ─────────────────────────────────────────────────────────────
//  WARSHALL  –  Clausura transitiva (alcanzabilidad)
//  Pregunta: ¿Desde qué nodo puedo llegar a qué otros nodos?
//  No usa pesos. Para cada par (i,j) responde SI o NO.
//
//  Idea: si puedo llegar de i→k  y  de k→j,
//        entonces también puedo llegar de i→j.
//  Se repite para cada nodo k como posible "puente".
// ─────────────────────────────────────────────────────────────
void warshall()
{
    if (p == NULL) { std::cout << "Grafo vacio\n"; return; }

    // Paso 1: recolectar nodos en un arreglo para indexarlos
    Tnodo nodos[100];
    int total = 0;
    Tnodo aux = p;
    while (aux != NULL) { nodos[total++] = aux; aux = aux->sgte; }

    // Mapear puntero → índice para acceso rápido
    std::map<Tnodo, int> idx;
    for (int i = 0; i < total; i++) idx[nodos[i]] = i;

    // Paso 2: inicializar matriz de alcanzabilidad en false
    bool alc[100][100] = {};
    for (int i = 0; i < total; i++) alc[i][i] = true;   // cada nodo se alcanza a sí mismo

    // Paso 3: cargar conexiones directas desde la lista de adyacencia
    for (int i = 0; i < total; i++) {
        Tarista ar = nodos[i]->ady;
        while (ar != NULL) {
            alc[i][idx[ar->destino]] = true;
            ar = ar->sgte;
        }
    }

    // Paso 4: triple bucle de Warshall
    // k = nodo intermedio que probamos como "puente"
    for (int k = 0; k < total; k++)
        for (int i = 0; i < total; i++)
            for (int j = 0; j < total; j++)
                if (alc[i][k] && alc[k][j])    // si i llega a k  Y  k llega a j...
                    alc[i][j] = true;           // ...entonces i llega a j

    // Mostrar resultado
    std::cout << "\n=== WARSHALL: ALCANZABILIDAD ===\n";
    std::cout << "(S = puede llegar,  - = sin ruta)\n\n";

    // Encabezado de columnas
    std::cout << "   ";
    for (int j = 0; j < total; j++) std::cout << "  " << nodos[j]->nombre;
    std::cout << "\n";

    for (int i = 0; i < total; i++) {
        std::cout << nodos[i]->nombre << "  ";
        for (int j = 0; j < total; j++)
            std::cout << "  " << (alc[i][j] ? 'S' : '-');
        std::cout << "\n";
    }
}

// ─────────────────────────────────────────────────────────────
//  FLOYD  –  Caminos mínimos entre TODOS los pares
//  Pregunta: ¿Cuántos saltos mínimos hay entre cada par?
//  (Este grafo no tiene pesos, así que la distancia = saltos)
//
//  Misma idea que Warshall pero guardando el costo mínimo
//  en lugar de true/false, y una tabla "next" para reconstruir
//  el camino exacto nodo a nodo.
// ─────────────────────────────────────────────────────────────
void floyd()
{
    if (p == NULL) { std::cout << "Grafo vacio\n"; return; }

    const int INF = 9999;

    // Paso 1: recolectar nodos
    Tnodo nodos[100];
    int total = 0;
    Tnodo aux = p;
    while (aux != NULL) { nodos[total++] = aux; aux = aux->sgte; }

    std::map<Tnodo, int> idx;
    for (int i = 0; i < total; i++) idx[nodos[i]] = i;

    // Paso 2: inicializar matrices
    int dist[100][100];
    int next[100][100];   // next[i][j] = siguiente nodo en el camino mínimo de i a j

    for (int i = 0; i < total; i++)
        for (int j = 0; j < total; j++) {
            dist[i][j] = (i == j) ? 0 : INF;
            next[i][j] = -1;
        }

    // Paso 3: cargar aristas directas (costo 1 por salto)
    for (int i = 0; i < total; i++) {
        Tarista ar = nodos[i]->ady;
        while (ar != NULL) {
            int j = idx[ar->destino];
            if (dist[i][j] > 1) {          // por si hay aristas duplicadas
                dist[i][j] = 1;
                next[i][j] = j;            // el siguiente paso desde i hacia j es j mismo
            }
            ar = ar->sgte;
        }
    }

    // Paso 4: triple bucle de Floyd
    // k = nodo candidato a ser puente entre i y j
    for (int k = 0; k < total; k++)
        for (int i = 0; i < total; i++)
            for (int j = 0; j < total; j++)
                if (dist[i][k] != INF && dist[k][j] != INF)
                    if (dist[i][k] + dist[k][j] < dist[i][j]) {
                        dist[i][j] = dist[i][k] + dist[k][j];
                        next[i][j] = next[i][k];   // para llegar a j, primero voy hacia k
                    }

    // Mostrar matriz de distancias
    std::cout << "\n=== FLOYD: DISTANCIAS MINIMAS (saltos) ===\n\n";

    std::cout << "   ";
    for (int j = 0; j < total; j++) std::cout << "  " << nodos[j]->nombre;
    std::cout << "\n";

    for (int i = 0; i < total; i++) {
        std::cout << nodos[i]->nombre << "  ";
        for (int j = 0; j < total; j++) {
            if (dist[i][j] == INF) std::cout << "  *";
            else                   std::cout << "  " << dist[i][j];
        }
        std::cout << "\n";
    }

    // Consulta de ruta entre dos nodos por nombre de caracter
    std::cout << "\nConsultar ruta (ingrese nombres, 0 para salir)\n";
    char ori, dst;
    while (true) {
        std::cout << "  Origen (0 para salir): ";
        std::cin >> ori;
        if (ori == '0') break;

        std::cout << "  Destino (0 para salir): ";
        std::cin >> dst;
        if (dst == '0') break;

        // Buscar índices
        int io = -1, id = -1;
        for (int i = 0; i < total; i++) {
            if (nodos[i]->nombre == ori) io = i;
            if (nodos[i]->nombre == dst) id = i;
        }

        if (io == -1 || id == -1) { std::cout << "  Nodo no encontrado\n"; continue; }
        if (io == id)             { std::cout << "  Mismo origen y destino\n"; continue; }
        if (dist[io][id] == INF)  { std::cout << "  Sin ruta posible\n"; continue; }

        std::cout << "  Saltos minimos: " << dist[io][id] << "\n";
        std::cout << "  Ruta: ";
        int cur = io;
        int pasos = 0;
        while (cur != id && cur != -1 && pasos <= total) {
            std::cout << nodos[cur]->nombre << " --> ";
            cur = next[cur][id];
            pasos++;
        }
        std::cout << nodos[id]->nombre << "\n";
    }
}

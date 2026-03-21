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

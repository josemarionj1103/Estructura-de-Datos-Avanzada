#include <iostream>
#include <stdlib.h>
using namespace std;

struct nodo {
    int nro;
    struct nodo *izq, *der;
};

typedef struct nodo* ABB;

ABB crearNodo(int x)
{
    ABB nuevoNodo = new(struct nodo);
    nuevoNodo->nro = x;
    nuevoNodo->izq = NULL;
    nuevoNodo->der = NULL;
    return nuevoNodo;
}

void insertar(ABB &arbol, int x)
{
    if (arbol == NULL)
    {
        arbol = crearNodo(x);
    }
    else if (x < arbol->nro)
        insertar(arbol->izq, x);
    else if (x > arbol->nro)
        insertar(arbol->der, x);
}

void preOrden(ABB arbol)
{
    if (arbol != NULL)
    {
        cout << arbol->nro << " ";
        preOrden(arbol->izq);
        preOrden(arbol->der);
    }
}

void enOrden(ABB arbol)
{
    if (arbol != NULL)
    {
        enOrden(arbol->izq);
        cout << arbol->nro << " ";
        enOrden(arbol->der);
    }
}

void postOrden(ABB arbol)
{
    if (arbol != NULL)
    {
        postOrden(arbol->izq);
        postOrden(arbol->der);
        cout << arbol->nro << " ";
    }
}

void verArbol(ABB arbol, int nivel)
{
    if (arbol == NULL)
        return;

    verArbol(arbol->der, nivel + 1);

    for (int i = 0; i < nivel; i++)
        cout << "      ";

    cout << arbol->nro << endl;

    verArbol(arbol->izq, nivel + 1);
}
ABB minimo(ABB arbol)
{
    while (arbol->izq != NULL)
        arbol = arbol->izq;

    return arbol;
}
ABB eliminar(ABB arbol, int x)
{
    if (arbol == NULL)
        return arbol;

    if (x < arbol->nro)
        arbol->izq = eliminar(arbol->izq, x);

    else if (x > arbol->nro)
        arbol->der = eliminar(arbol->der, x);

    else
    {
        if (arbol->izq == NULL && arbol->der == NULL)
        {
            delete arbol;
            return NULL;
        }
        else if (arbol->izq == NULL)
        {
            ABB temp = arbol;
            arbol = arbol->der;
            delete temp;
        }
        else if (arbol->der == NULL)
        {
            ABB temp = arbol;
            arbol = arbol->izq;
            delete temp;
        }
        else
        {
            ABB temp = minimo(arbol->der);
            arbol->nro = temp->nro;
            arbol->der = eliminar(arbol->der, temp->nro);
        }
    }

    return arbol;
}

int main()
{
    ABB arbol = NULL;
    int n, x;
    char opcion;

    cout << "\n\t\t ..[ ARBOL BINARIO DE BUSQUEDA ].. \n\n";

    cout << "Numero de nodos del arbol: ";
    cin >> n;
    cout << endl;

    for (int i = 0; i < n; i++)
    {
        cout << "Numero del nodo " << i + 1 << ": ";
        cin >> x;
        insertar(arbol, x);
    }

    cout << "\nMostrando ABB\n\n";
    verArbol(arbol, 0);

    cout << "\nRecorridos del ABB";
    cout << "\n\nEn orden : ";
    enOrden(arbol);

    cout << "\n\nPre Orden : ";
    preOrden(arbol);

    cout << "\n\nPost Orden : ";
    postOrden(arbol);
    do
    {
        int eliminarNodo;
        cout << "\n\nNumero a eliminar: ";
        cin >> eliminarNodo;

        arbol = eliminar(arbol, eliminarNodo);

        cout << "\n\nARBOL DESPUES DE ELIMINAR\n\n";
        verArbol(arbol, 0);

        cout << "\nRecorridos después de eliminar el nodo:";
        cout << "\nEn orden : ";
        enOrden(arbol);
        cout << "\nPre Orden : ";
        preOrden(arbol);
        cout << "\nPost Orden : ";
        postOrden(arbol);

        cout << "\n\n¿Desea eliminar otro nodo? (s/n): ";
        cin >> opcion;

    } while (opcion == 's' || opcion == 'S');

    cout << endl << endl;
    system("pause");
    return 0;
}

/*
Árbol AVL completo
Incluye:
- Inserción balanceada
- Eliminación balanceada
- Verificación de balance
- Representación gráfica
*/

#include <iostream>
using namespace std;

class NodoAVL {
public:
    int valor;
    NodoAVL* izquierdo;
    NodoAVL* derecho;
    int altura;

    NodoAVL(int val) {
        valor = val;
        izquierdo = nullptr;
        derecho = nullptr;
        altura = 1;
    }
};

class ArbolAVL {
private:
    NodoAVL* raiz;

    int maximo(int a, int b) {
        return (a > b) ? a : b;
    }

    int getAltura(NodoAVL* nodo) {
        if (nodo == nullptr) return 0;
        return nodo->altura;
    }

    int getBalance(NodoAVL* nodo) {
        if (nodo == nullptr) return 0;
        return getAltura(nodo->izquierdo) - getAltura(nodo->derecho);
    }

    // Rotaciones
    NodoAVL* rotacionDerecha(NodoAVL* y) {
        NodoAVL* x = y->izquierdo;
        NodoAVL* T2 = x->derecho;

        x->derecho = y;
        y->izquierdo = T2;

        y->altura = maximo(getAltura(y->izquierdo), getAltura(y->derecho)) + 1;
        x->altura = maximo(getAltura(x->izquierdo), getAltura(x->derecho)) + 1;

        return x;
    }

    NodoAVL* rotacionIzquierda(NodoAVL* x) {
        NodoAVL* y = x->derecho;
        NodoAVL* T2 = y->izquierdo;

        y->izquierdo = x;
        x->derecho = T2;

        x->altura = maximo(getAltura(x->izquierdo), getAltura(x->derecho)) + 1;
        y->altura = maximo(getAltura(y->izquierdo), getAltura(y->derecho)) + 1;

        return y;
    }

    // Inserción
    NodoAVL* insertarNodo(NodoAVL* nodo, int valor) {
        if (nodo == nullptr)
            return new NodoAVL(valor);

        if (valor < nodo->valor)
            nodo->izquierdo = insertarNodo(nodo->izquierdo, valor);
        else if (valor > nodo->valor)
            nodo->derecho = insertarNodo(nodo->derecho, valor);
        else
            return nodo;

        nodo->altura = 1 + maximo(getAltura(nodo->izquierdo), getAltura(nodo->derecho));

        int balance = getBalance(nodo);

        // Casos AVL
        if (balance > 1 && valor < nodo->izquierdo->valor)
            return rotacionDerecha(nodo);

        if (balance < -1 && valor > nodo->derecho->valor)
            return rotacionIzquierda(nodo);

        if (balance > 1 && valor > nodo->izquierdo->valor) {
            nodo->izquierdo = rotacionIzquierda(nodo->izquierdo);
            return rotacionDerecha(nodo);
        }

        if (balance < -1 && valor < nodo->derecho->valor) {
            nodo->derecho = rotacionDerecha(nodo->derecho);
            return rotacionIzquierda(nodo);
        }

        return nodo;
    }

    // Buscar nodo mínimo
    NodoAVL* nodoMinimo(NodoAVL* nodo) {
        NodoAVL* actual = nodo;
        while (actual->izquierdo != nullptr)
            actual = actual->izquierdo;
        return actual;
    }

    // Eliminación
    NodoAVL* eliminarNodo(NodoAVL* raiz, int valor) {
        if (raiz == nullptr)
            return raiz;

        if (valor < raiz->valor)
            raiz->izquierdo = eliminarNodo(raiz->izquierdo, valor);
        else if (valor > raiz->valor)
            raiz->derecho = eliminarNodo(raiz->derecho, valor);
        else {
            if ((raiz->izquierdo == nullptr) || (raiz->derecho == nullptr)) {
                NodoAVL* temp = raiz->izquierdo ? raiz->izquierdo : raiz->derecho;

                if (temp == nullptr) {
                    temp = raiz;
                    raiz = nullptr;
                } else {
                    *raiz = *temp;
                }
                delete temp;
            } else {
                NodoAVL* temp = nodoMinimo(raiz->derecho);
                raiz->valor = temp->valor;
                raiz->derecho = eliminarNodo(raiz->derecho, temp->valor);
            }
        }

        if (raiz == nullptr)
            return raiz;

        raiz->altura = 1 + maximo(getAltura(raiz->izquierdo), getAltura(raiz->derecho));

        int balance = getBalance(raiz);

        // Rebalanceo
        if (balance > 1 && getBalance(raiz->izquierdo) >= 0)
            return rotacionDerecha(raiz);

        if (balance > 1 && getBalance(raiz->izquierdo) < 0) {
            raiz->izquierdo = rotacionIzquierda(raiz->izquierdo);
            return rotacionDerecha(raiz);
        }

        if (balance < -1 && getBalance(raiz->derecho) <= 0)
            return rotacionIzquierda(raiz);

        if (balance < -1 && getBalance(raiz->derecho) > 0) {
            raiz->derecho = rotacionDerecha(raiz->derecho);
            return rotacionIzquierda(raiz);
        }

        return raiz;
    }

    // Verificar balance
    bool verificarBalance(NodoAVL* nodo) {
        if (nodo == nullptr)
            return true;

        int balance = getBalance(nodo);

        if (balance > 1 || balance < -1)
            return false;

        return verificarBalance(nodo->izquierdo) &&
               verificarBalance(nodo->derecho);
    }

    // Recorrido
    void inordenRec(NodoAVL* nodo) {
        if (nodo != nullptr) {
            inordenRec(nodo->izquierdo);
            cout << nodo->valor << " ";
            inordenRec(nodo->derecho);
        }
    }

    // Visualización
    int getAlturaTotalArbol(NodoAVL* nodo) {
        if (nodo == nullptr) return 0;
        return 1 + maximo(getAlturaTotalArbol(nodo->izquierdo),
                          getAlturaTotalArbol(nodo->derecho));
    }

    void imprimirEspacios(int n) {
        for (int i = 0; i < n; i++) cout << " ";
    }

    int contarDigitos(int num) {
        if (num == 0) return 1;
        int count = 0;
        if (num < 0) {
            count = 1;
            num = -num;
        }
        while (num > 0) {
            count++;
            num /= 10;
        }
        return count;
    }

    void imprimirNivel(NodoAVL* raiz, int nivel, int espaciado) {
        if (raiz == nullptr) {
            imprimirEspacios(espaciado);
            cout << "·";
            return;
        }

        if (nivel == 1) {
            imprimirEspacios(espaciado - contarDigitos(raiz->valor)/2);
            cout << raiz->valor;
            imprimirEspacios(espaciado - contarDigitos(raiz->valor)/2);
        } else {
            imprimirNivel(raiz->izquierdo, nivel-1, espaciado);
            imprimirNivel(raiz->derecho, nivel-1, espaciado);
        }
    }

public:
    ArbolAVL() {
        raiz = nullptr;
    }

    void insertar(int valor) {
        raiz = insertarNodo(raiz, valor);
    }

    void eliminar(int valor) {
        raiz = eliminarNodo(raiz, valor);
    }

    void inorden() {
        inordenRec(raiz);
        cout << "\n";
    }

    void mostrarArbol() {
        if (raiz == nullptr) {
            cout << "Árbol vacío\n";
            return;
        }

        int altura = getAlturaTotalArbol(raiz);
        int espaciado = 4;

        for (int i = 0; i < altura - 1; i++) {
            espaciado *= 2;
        }

        for (int i = 1; i <= altura; i++) {
            imprimirNivel(raiz, i, espaciado/(1 << (i-1)));
            cout << "\n\n";
        }
    }

    bool estaBalanceado() {
        return verificarBalance(raiz);
    }
};

int main() {
    ArbolAVL arbol;

    arbol.insertar(10);
    arbol.insertar(20);
    arbol.insertar(30);
    arbol.insertar(5);
    arbol.insertar(15);

    cout << "Inorden: ";
    arbol.inorden();

    cout << "\nÁrbol:\n";
    arbol.mostrarArbol();

    cout << "\n¿Está balanceado?: "
         << (arbol.estaBalanceado() ? "Sí" : "No") << endl;

    cout << "\nEliminando 20...\n";
    arbol.eliminar(20);

    arbol.mostrarArbol();

    cout << "\n¿Está balanceado?: "
         << (arbol.estaBalanceado() ? "Sí" : "No") << endl;

    return 0;
}

#include <iostream>
using namespace std;

long factorialRecursivo(int n){
    if(n == 0 || n == 1){
        return 1;
    }
    else{
        return n * factorialRecursivo(n - 1);
    }
}

long factorialIterativo(int n){
    long resultado = 1;
    for(int i = 1; i <= n; i++){
        resultado *= i;
    }
    return resultado;
}

int main(){
    
    int n, opcion;

    cout << "Ingresar un numero: ";
    cin >> n;

    if(n < 0){
        cout << "El numero debe ser mayor o igual a 0" << endl;
        return 0;
    }

    cout << "\nSeleccione el metodo:\n";
    cout << "1. Metodo Iterativo\n";
    cout << "2. Metodo Recursivo\n";
    cout << "Opcion: ";
    cin >> opcion;

    if(opcion == 1){
        cout << "\nFactorial (Iterativo) de " << n 
             << " es: " << factorialIterativo(n) << endl;
    }
    else if(opcion == 2){
        cout << "\nFactorial (Recursivo) de " << n 
             << " es: " << factorialRecursivo(n) << endl;
    }
    else{
        cout << "Opcion invalida." << endl;
    }

    return 0;
}


#include <iostream>
using namespace std;
int contador= 0;
void permutacion(char A[], int k, int n) {
    if (k == n) {
        contador++;
        
        for (int i = 0; i <= n; i++) {
            cout << A[i];
        }
        cout << endl;
    }
    else {
        for (int i = k; i <= n; i++) {
            
            char temp = A[k];
            A[k] = A[i];
            A[i] = temp;

            permutacion(A, k + 1, n);

            temp = A[k];
            A[k] = A[i];
            A[i] = temp;
        }
    }
}

int main() {
    int n;
    cout << "Ingrese la cantidad de elementos: ";
    cin >> n;
    char A[n];
    cout << "Ingrese los caracteres:\n";
    for (int i = 0; i < n; i++) {
        cin >> A[i];
    }
    cout << "\nPermutaciones:\n";
    permutacion(A, 0, n - 1);
    cout<<" Total de permutaciones: "<<contador<<endl;
    return 0;
}

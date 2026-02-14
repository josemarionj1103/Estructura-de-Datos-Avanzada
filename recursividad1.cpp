#include <iostream>
using namespace std;

long factorial(int n){
    if(n == 0 || n == 1){   
        return 1;
    }
    else{
        return n * factorial(n - 1);  
    }
}
int main(){
    
    int n;
    
    cout<<"ingresar un numero: "; cin >> n;
    
    if(n<0){
        cout<<"El numero debe ser mayor a 0"<<endl;
        
    }
    cout<<"El factorial de " <<n<< " es: "<<factorial(n)<< endl;
    
    
    return 0;
}

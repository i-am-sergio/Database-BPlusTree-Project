#include <iostream>
#include <fstream>
using namespace std;

int main(){

    fstream dictionary("disk/bloque43.bin", ios::in | ios::out | ios::binary);
    
    // Aumenta el numero de tablas
    int nTablas;
    int aux = 0;
    dictionary.seekg(aux);
    for(int j=0;j<341;j++){
        for(int i=0;i<3;i++){
          dictionary.read(reinterpret_cast<char *>(&nTablas), sizeof(int));
          cout<<nTablas<<" - ";
        }cout<<"\n";
    }
    
}

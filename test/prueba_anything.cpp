#include <iostream>
#include <cmath>
using namespace std;

int main(){
    int numSectoresPorBloque = 8;
    int numTotalBloques = 160;
    int numSectorSolicitado = 49;
    int getBloque = std::ceil(static_cast<double>(numSectorSolicitado) /numSectoresPorBloque); // 45/8 = 6   48/8 = 6 
    cout<<"getbloque: "<<getBloque;
    return 0;
}
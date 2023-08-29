#pragma once
#include "Pista.h"
#include <iostream>
#include <string>
#include <vector>
using namespace std;
  
class Superficie{
  public:
    // Info
    int idSuperficie;
    vector<Pista>pistas;

    Superficie(){}
    Superficie(int idSuperficie,int idPlato,int numSectoresPorPista, int numPistasPorSuperficie,int capacidadDelSector){//(i+1,_pista,_superf,_plat);
      this->idSuperficie = idSuperficie + (idPlato-1)*2;
        string comando = "mkdir -p HDD/Plato" + to_string(idPlato)+"/Superficie"+to_string(this->idSuperficie);
        system(comando.c_str());
      for (int i=1; i<=numPistasPorSuperficie; i++) {
        pistas.emplace_back(i,this->idSuperficie,idPlato,numPistasPorSuperficie,numSectoresPorPista,capacidadDelSector);
      }
    }
    void showInfo(){
      cout<<"idSuperficie -> "<<idSuperficie<<endl;
    }
};

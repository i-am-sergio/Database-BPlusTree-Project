#pragma once
#include "Sector.h"
#include <iostream>
#include <vector>
using namespace std;

class Pista{
  public:
    //Atributos 
    int idPista;
    vector<Sector>sectores;

    Pista(){}
    Pista(int idPista,int idSuperficie,int idPlato,int numPistasPorSuperficie,int numSectoresPorPista,int capacidadDelSector){//(i+1,_pista,_superf,_plat);
      this->idPista = idPista + (idSuperficie-1)*numPistasPorSuperficie;
        string comando = "mkdir -p HDD/Plato" + to_string(idPlato)+"/Superficie"+to_string(idSuperficie)+"/Pista"+to_string(this->idPista);
        system(comando.c_str());
      for (int i=1; i<=numSectoresPorPista; i++) {
        sectores.emplace_back(i,this->idPista,idSuperficie,idPlato,numSectoresPorPista,capacidadDelSector);
      }
    }
    void showInfo(){
      cout<<"idPista -> "<<idPista<<endl;
    }
};

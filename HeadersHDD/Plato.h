#pragma once
#include "Superficie.h"
#include <iostream>
#include <vector>
using namespace std;

class Plato{
  public:
    // Atributos 
    int idPlato;
    vector<Superficie>superficies;

    Plato(){}
    Plato(int idPlato,int numSectoresPorPista, int numPistasPorSuperficie,int capacidadDelSector){
      this->idPlato = idPlato;
        string comando = "mkdir -p HDD/Plato" + to_string(this->idPlato);
        system(comando.c_str());
      superficies.emplace_back(1,idPlato,numSectoresPorPista,numPistasPorSuperficie,capacidadDelSector);
      superficies.emplace_back(2,idPlato,numSectoresPorPista,numPistasPorSuperficie,capacidadDelSector);
    }
};

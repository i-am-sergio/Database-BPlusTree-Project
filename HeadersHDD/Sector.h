#pragma once
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class Sector{
public:
    // Atributos 
    int idSector;
    int idPista;
    int idSuperficie;
    int idPlato;

    int capacidadDeSector;
    int capacidadOcupada;
    int capacidadLibre;
    Sector(){}
    Sector(int idSector,int idPista,int idSuperficie,int idPlato,int numSectoresPorPista,int capacidadDelSector){
      this->idSector = idSector + (idPista-1) * numSectoresPorPista;
      this->capacidadDeSector = capacidadDelSector;
      this->idPista = idPista;
      this->idSuperficie = idSuperficie;
      this->idPlato = idPlato;
      this->capacidadDeSector = capacidadDelSector;
        //Comprueba si ya existen
        ifstream file("HDD/Plato" + to_string(idPlato)+"/Superficie"+to_string(idSuperficie)+"/Pista"+to_string(this->idPista)+"/Sector"+to_string(this->idSector)+".bin", std::ios::binary);
        if (file) { 
          file.seekg(0,ios::end);
          if((int)file.tellg() != 0){
            return;
          }
        }
        file.close();
        //Si no hay espacions vacios creamos los sectores
        ofstream archivo("HDD/Plato" + to_string(idPlato)+"/Superficie"+to_string(idSuperficie)+"/Pista"+to_string(this->idPista)+"/Sector"+to_string(this->idSector)+".bin", std::ios::binary);
          char * buffer = new char[capacidadDelSector];
          for (int j = 0; j < capacidadDelSector; j++) {
              buffer[j] = '*';
          }
          archivo.write(buffer, capacidadDelSector);
          
          delete [] buffer;
          
          archivo.close();

        //file.close();
    }
    
    void showInfoSector() {
        cout<<"\n Informacion del sector: \n ";
        cout << "\t" << " IdSector: " << idSector << std::endl;
        cout << "\t" << " numDePista: " << idPista << std::endl;
        cout << "\t" << " numDeSuperficie: " << idSuperficie << std::endl;
        cout << "\t" << " numDePlato: " << idPlato << std::endl;
    }
};

#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdint>
#include "Plato.h"
using namespace std;

class Disco{
    public:
        int capacidadTotalBytes;
        uint64_t capacidadTotalBits;

        int numPlatos;
        int numPistasPorSuperficie;
        int numSectoresPorPista;
        int capacidadDelSector;
        
        int sectoresPorBloque;
        int sizeBloque;

        int totalSectoresEnDisco;
        int totalPistasEnDisco;
        int totalSuperficiesEnDisco;

        vector<Plato> platos;

        int getNumBloques(){
            return (this->capacidadTotalBytes/(this->capacidadDelSector*this->sectoresPorBloque));
        }
        
        Disco(){
            std::fstream archivo("HeadersHDD/structDisk.bin", std::ios::in | std::ios::binary);
            //archivo.seekg(0, std::ios::end);
            if(archivo){
                cout<<"Se encontro una estructura de disco.\n";
                archivo.seekg(0, std::ios::beg);
                archivo.read(reinterpret_cast<char*>(&this->numPlatos), sizeof(numPlatos));
                archivo.read(reinterpret_cast<char*>(&this->numPistasPorSuperficie), sizeof(numPistasPorSuperficie));
                archivo.read(reinterpret_cast<char*>(&this->numSectoresPorPista), sizeof(numSectoresPorPista));
                archivo.read(reinterpret_cast<char*>(&this->capacidadDelSector), sizeof(capacidadDelSector));
                archivo.read(reinterpret_cast<char*>(&this->sectoresPorBloque), sizeof(sectoresPorBloque));
                cout<<this->numPlatos<<"-"<<this->numPistasPorSuperficie<<"-"<<this->numSectoresPorPista<<"-"<<this->capacidadDelSector<<"-"<<this->sectoresPorBloque<<"\n";
                archivo.close();

              buildDisk();
            } else {
                cout<<"No se encontro una estructura del disco. Ingrese los siguientes datos para crear uno nuevo.\n";
                string aux;
                std::fstream archivo("HeadersHDD/structDisk.bin", std::ios::out | std::ios::binary);
                cout<<"Ingrese el numero de Platos: ";getline(cin,aux);
                this->numPlatos = stoi(aux);
                cout<<"Ingrese el numero de Pistas por Superficie: ";getline(cin,aux);
                this->numPistasPorSuperficie = stoi(aux);
                cout<<"Ingrese el numero de Sectores por Pista: ";getline(cin,aux);
                this->numSectoresPorPista = stoi(aux);
                cout<<"Ingrese la capacidad del Sector: ";getline(cin,aux);
                this->capacidadDelSector = stoi(aux);
                cout<<"Ingrese la cantidad de sectores por bloque: ";getline(cin,aux);
                this->sectoresPorBloque = stoi(aux);
                cin.ignore();
                archivo.write(reinterpret_cast<char*>(&numPlatos), sizeof(numPlatos));
                archivo.write(reinterpret_cast<char*>(&numPistasPorSuperficie), sizeof(numPistasPorSuperficie));
                archivo.write(reinterpret_cast<char*>(&numSectoresPorPista), sizeof(numSectoresPorPista));
                archivo.write(reinterpret_cast<char*>(&capacidadDelSector), sizeof(capacidadDelSector));
                archivo.write(reinterpret_cast<char*>(&sectoresPorBloque), sizeof(sectoresPorBloque));
                cout<<this->numPlatos<<"-"<<this->numPistasPorSuperficie<<"-"<<this->numSectoresPorPista<<"-"<<this->capacidadDelSector<<"-"<<this->sectoresPorBloque<<"\n";
                buildDisk();
                ofstream file("dictionary/dictionary.bin",ios::binary);
                    int nBloques = getNumBloques();
                    file.write(reinterpret_cast<char*>(&nBloques), sizeof(int));
                    nBloques = 0;
                    file.write(reinterpret_cast<char*>(&nBloques), sizeof(int));
                    file.write(reinterpret_cast<char*>(&nBloques), sizeof(int));
                file.close();
            }
        };

        void buildDisk(){
            this->capacidadTotalBytes = capacidadDelSector * numSectoresPorPista * numPistasPorSuperficie * 2 * numPlatos; 
            this->capacidadTotalBits = this->capacidadTotalBytes * 8;
            this->totalSectoresEnDisco = capacidadTotalBytes / capacidadDelSector;
            this->totalPistasEnDisco = totalSectoresEnDisco / numSectoresPorPista;
            this->totalSuperficiesEnDisco = numPlatos * 2;
            showInfoDisco();
            for(int i=1; i<=numPlatos; i++){
                platos.emplace_back(i,numSectoresPorPista,numPistasPorSuperficie,capacidadDelSector);
            }
        } 
        //void disk(){ //5,8,20,100,10

        void showInfoDisco() {
            std::cout << "INFORMACION DEL DISCO:\n";
            std::cout << "  CapacidadTotalBytes: " << capacidadTotalBytes << std::endl;
            std::cout << "  CapacidadTotalBits: " << capacidadTotalBits << std::endl;
            std::cout << "  NumPlatos: " << numPlatos << std::endl;
            std::cout << "  NumPistasPorSuperficie: " << numPistasPorSuperficie << std::endl;
            std::cout << "  NumSectoresPorPista: " << numSectoresPorPista << std::endl;
            std::cout << "  CapacidadDelSector: " << capacidadDelSector << std::endl;
            std::cout << "  SectoresPorBloque: " << sectoresPorBloque << std::endl;
            std::cout << "  TotalSectoresEnDisco: " << totalSectoresEnDisco << std::endl;
            std::cout << "  TotalPistasEnDisco: " << totalPistasEnDisco << std::endl;
            std::cout << "  TotalSuperficiesEnDisco: " << totalSuperficiesEnDisco << std::endl<<endl;
        }

        ~Disco(){}
};

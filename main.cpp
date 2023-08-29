#include <iostream>
#include <cstring>
#include <string>
#include "DiskController.h"
#include "HeadersHDD/Disco.h"
#include "BPlusTree.h"
#include "SGDB.h"
using namespace std;


int main(){
    Disco myDisk;
    DiskController myDiskController(&myDisk);
    SGDB sistema(&myDisk,& myDiskController);

    cin.get();

    int opc;
    bool salir = false;
    while(salir==false){
        cout<<"\n-------------------| MENU |--------------------\n";
        cout<<"\nIngrese una opcion:\n";
        cout<<"[1] Menu de Disco\n";
        cout<<"[2] Menu del sistema de Base de Datos\n";
        cout<<"[3] Menu del Buffer\n";
        cout<<"[4] Menu del Arbol B+\n";
        cin>>opc;
        if(opc==1){
            int opc2;
            cout<<"[1] Imprimir Metadata Sector n\n";
            cout<<"[2] Imprimir Metadata Bloque n\n";
            cout<<"[3] Imprimir Metadata Registro n\n";
            cout<<"[4] Mostrar informacion del disco n\n";
            cout<<"[5] Mostrar diccionario n\n";
            cout<<"[6] Prueba SlottedPage\n";
            cout<<"[7] Agregar Tabla al diccionario\n";
            cout<<"[8] Setear slottedPages\n";
            cout<<"[9] Print slottedPages\n";
            cin>>opc2;
            cin.ignore();
            if(opc2==1){
                int numSector;
                cout<<"Ingrese un numero de sector => ";
                cin>>numSector;
                myDiskController.printSector(numSector);
            } else if(opc2==2){
                int numBloque;
                cout<<"Ingrese un numero de bloque => ";
                cin>>numBloque;
                myDiskController.printBloque(numBloque);
            }else if(opc2==3){
                int registro,ubi;
                cout<<"Ingrese el numero de registro => ";
                cin>>registro;
                ubi = sistema.printUbicacionRegistro(registro);
                myDiskController.printBloque(ubi);
            } else if (opc2 == 4) {
                myDiskController.disco->showInfoDisco();
            } else if (opc2 == 5) {
                myDiskController.showDictionary();
            } else if (opc2 == 6) {
                myDiskController.setBloque_SlottedPage(1);
            } else if (opc2 == 7) {
                myDiskController.AgregarTablaVariable();
                myDiskController.showDictionary();
            }
            else if(opc2==8){
                myDiskController.setearBloques();
            }
            else if(opc2==9){
                int numBloque;
                cout<<"Ingrese id Bloque: ";
                cin>>numBloque;
                myDiskController.printBloque_SlottedPage(numBloque);
            }
        } else if(opc==2){

            int opc2;
            cout<<"[1] Crear Tabla\n";
            cout<<"[2] Insert File ()\n";
            cout<<"[3] Mostrar Bloque n\n";
            cout<<"[4] Insert 1 registro n\n"; // ident si es fijo o variable de acuerdo al esquema de tablas, e insertarlo en binario
            cout<<"[5] Delete 1 registro n\n";
            cout<<"[6] SELECT * FROM titanic\n";
            cout<<"[7] SELECT * FROM titanic where atributo = n\n";
            cout<<"[8] Elegir Tabla a trabajar\n";
            cout<<"[9] insertar RegistroVariable\n";
            cout<<"[10] Crear Tabla variable\n";
            cout<<"[11] Crear tabla para Arbol B+\n";
            cin>>opc2;
            cin.ignore();

            if(opc2==1){
                cout<<" >> ";
                string prompt;
                getline(cin,prompt);
                sistema.createTable(prompt);
            } else if(opc2==2){
                string prompt, prompt2;
                getline(cin,prompt);
                getline(cin,prompt2);
                myDiskController.uploadTableToDisk(prompt,prompt2);
            } else if(opc2==3){
                cout<<" >> Ingrese el numero de bloque: ";
                string prompt;
                getline(cin,prompt);
                sistema.mostrarPage(stoi(prompt));
                sistema.bufferManager->showpageTableCLOCK();
            } else if(opc2==4){
                sistema.insertarRegistro();
                sistema.bufferManager->showpageTableCLOCK();
            } else if(opc2==5){
                string prompt;  
                cout<<"Ingrese el objetivo -> ";
                getline(cin,prompt);
                sistema.deleteRegistro(stoi(prompt));
                sistema.bufferManager->showpageTableCLOCK();
            } else if(opc2==6){
                sistema.showTable();//cambios
                //sistema.showTable("titanic");
                sistema.bufferManager->showpageTableCLOCK();
            } else if(opc2==7){
                string prompt, prompt2;
                cout<<"Ingrese el nombre del atributo -> ";
                getline(cin,prompt);
                cout<<"Ingrese el objetivo -> ";
                getline(cin,prompt2);

                sistema.search(prompt,stoi(prompt2));
                sistema.bufferManager->showpageTableCLOCK();
            } else if(opc2==8){
                string prompt;
                cout<<"Ingrese el nombre de la tabla -> ";
                getline(cin,prompt);
                myDiskController.nameTable = prompt;
            } else if(opc2==9){
                try{
                    string prompt;
                    // string prompt = "INSERT INTO titanic 1,0,3,\"Johnston, Miss. Catherine Helen Carrie\",female,,1,2,W./C. 6607,23.45,,S";
                    // string promptMovie = "INSERT INTO movie Alien,,,2,,5,4,,,4,,,,,,2,,,4,3,,,2,5,,4";
                    cout<<" >> ";
                    getline(cin,prompt);
                    // sistema.diskController->setBloque_SlottedPage(1);
                    // string prompt = "INSERT INTO titanic 892,0,3,\"Johnston, Miss. Catherine Helen Carrie\",female,,1,2,W./C. 6607,23.45,,S";
                    sistema.insertRegistroLongitudVariable(prompt);
                } catch(const exception& e){
                    cout << "Se produjo una excepcion: " << e.what() << endl;
                }
            } else if(opc2==10){
                cout<<" >> ";
                string prompt;
                getline(cin,prompt);
                sistema.createTable_Variable(prompt);
            }
            else if(opc2==11){
                cout<<" >> ";
                string prompt;
                getline(cin,prompt);
                //Ingresar tabla a convertir para tabla b+
                sistema.crearTablaBPLus(prompt);
                sistema.AgregarRegistrosBloqueATabla();
            }

        }else if(opc==3){
            cout<<"[1] Mostrar Tabla\n";
            cout<<"[2] Limpiar todo el bufferPool\n";
            cout<<"[3] Limpiar una pagina del bufferPool\n";
            cout<<"[4] PinCount a 0 \n";
            cout<<"[5] Cambiar estado \n";
            int opc3;
            cin>>opc3;
            cin.ignore();
            if(opc3==1){
                sistema.bufferManager->showpageTableCLOCK();
            } else if(opc3==2){
                sistema.bufferManager->LimpiarBufferPool();
            }
            else if(opc3==3){
                int aux;
                cout<<" Ingrese la pagina a limpiar del bufferPool: ";
                cin>>aux;
                cin.ignore();
                sistema.bufferManager->LimpiarPageBufferPool(aux);
            }
            else if(opc3==4){
                int aux;
                cout<<" Ingrese la pagina del bufferPool: ";
                cin>>aux;
                cin.ignore();
                sistema.bufferManager->PinCountACero(aux);
            }
            else if(opc3==5){
                sistema.bufferManager->cambiarEstadoPC();
            }
        }
        else if(opc==4){
            cout<<"[1] Cargar Arbol B+\n";
            cout<<"[2] Setear Orden\n";
            cout<<"[3] Imprimir Arbol\n";
            cout<<"[4] Buscar Elemento\n";
            cout<<"[5] Insertar Registro\n";
            cout<<"[6] Eliminar Registro\n";
            cout<<"[7] Imprimir Sector\n";
            int opc4;
            cin>>opc4;
            cin.ignore();
            if(opc4 == 1){
                sistema.cargarArbolBplus();
            }
            else if (opc4 == 2) {
                int aux;
                cout<<" Ingrese el orden del arbol: ";
                cin>>aux;
                cin.ignore();
                sistema.tree.setOrden(aux);
            }
            else if (opc4 == 3) {
                sistema.tree.print();
            }
            else if (opc4 == 4) {
                int aux;
                cout<<" Ingrese el elemento que desea buscar: ";
                cin>>aux;
                cin.ignore();
                sistema.buscarElemento(aux);
                // Del Arbol
                pair<int,int> direccion = sistema.tree.get(aux);
                int numBloque_Registro = direccion.first;
                int ptr_Registro = direccion.second;
                // Del Disco 
                int numSectorsPorBloque = sistema.diskController->disco->sectoresPorBloque;
                int numBytesPorSector = sistema.diskController->disco->capacidadDelSector;
                int numSector_Registro = (((numBloque_Registro-1)* numSectorsPorBloque) + (ptr_Registro/numBytesPorSector))+1;

                myDiskController.printInfoSector(numSector_Registro); 
                // myDiskController.printSector(numSector_Registro);
            }
            else if (opc4 == 5) {
                sistema.insertarRegistro();
            }
            else if (opc4 == 6) {
                int aux;
                cout<<" Ingrese el elemento que desea eliminar: ";
                cin>>aux;
                cin.ignore();
                sistema.BtreeDeleteRegistro(aux);
            }
            else if (opc4 == 7) {
                int aux;
                cout<<" Ingrese sector que desea ver: ";
                cin>>aux;
                cin.ignore();
                myDiskController.printInfoSector(aux); 
                sistema.VerSector(aux);

            }
        }

        else if(opc==0){
            salir = true;
        }
    }
    return 0;
}

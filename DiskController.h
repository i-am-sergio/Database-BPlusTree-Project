#pragma once
#include <ios>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <tuple>
#include <cmath>
#include <cstdlib>
#include "HeadersHDD/Disco.h"
#include "tipos.cpp"
#include "design/lineas.cpp"

#ifdef _WIN32 // Verificar si se está compilando en Windows

#define REMOVE_COMMAND "rmdir /s /q disk"

#else // Si no es Windows, asumir Linux o sistema similar

#define REMOVE_COMMAND "rm -r disk"

#endif

using namespace std;

class DiskController
{
public:
    Disco *disco;
    int nTotalBloques;
    int sizeBloque;
    int numSectoresPorBloque;
    int NumBLoquesEnUso;
    vector<tuple<string, string, int>> info;
    string nameTable;
    vector<int> free_espace_map;

    DiskController(Disco *disco)
    {
        this->disco = disco;
        this->nTotalBloques = disco->getNumBloques();
        this->sizeBloque = disco->capacidadDelSector * disco->sectoresPorBloque;
        this->numSectoresPorBloque = disco->sectoresPorBloque;

        int sectorActual = 1;
        if (!system("mkdir disk"))
        { // si no existen bloques, los crea.
            for (int i = 1; i <= nTotalBloques; i++)
            {
                string name_bloque = "bloque";
                name_bloque += to_string(i);
                ofstream archivo("disk/" + name_bloque + ".bin", std::ios::binary);

                char *buffer = new char[sizeBloque];
                if (!archivo)
                {
                    std::cout << "No se pudo abrir el archivo." << std::endl;
                    return;
                }
                int _pista, _superf, _plat;
                for (int i = 0; i < numSectoresPorBloque; i++)
                {
                    _pista = ((sectorActual) % disco->numSectoresPorPista == 0) ? ((sectorActual) / disco->numSectoresPorPista) : ((sectorActual) / disco->numSectoresPorPista) + 1;
                    _superf = (_pista % disco->numPistasPorSuperficie == 0) ? (_pista / disco->numPistasPorSuperficie) : (_pista / disco->numPistasPorSuperficie) + 1;
                    _plat = (_superf % 2 == 0) ? (_superf / 2) : (_superf / 2) + 1;

                    ifstream input("HDD/Plato" + to_string(_plat) + "/Superficie" + to_string(_superf) + "/Pista" + to_string(_pista) + "/Sector" + to_string(sectorActual) + ".bin", std::ios::binary);

                    input.read(buffer + disco->capacidadDelSector * i, disco->capacidadDelSector);
                    input.close();
                    sectorActual++;
                }

                archivo.write(buffer, sizeBloque);
                delete[] buffer;
                archivo.close();
            }
            std::cout << "Se han generado los bloques correctamente" << std::endl;
        }
    }

    ~DiskController()
    {
        // Al finalizar el programa los bloques virtuales desaparecen
        system(REMOVE_COMMAND);
    }

    void setearBloques()
    {
        int bloqueInicial;
        int bloqueFinal;
        fstream dictionary("dictionary/dictionary.bin", ios::in | ios::out | ios::binary);
        int pos = buscarTablaenDictionary(nameTable);
        dictionary.seekg(pos + 104);
        dictionary.read(reinterpret_cast<char *>(&bloqueInicial), sizeof(int));
        dictionary.read(reinterpret_cast<char *>(&bloqueFinal), sizeof(int));
        dictionary.close();
        for (int i = bloqueInicial; i <= bloqueFinal; i++)
        { // Seteamos 10 bloques como slottedpage
            setBloque_SlottedPage(i);
            bloqueASector(i);
        }
        cout<<"bloqueInicial => "<<bloqueInicial<<"\n";
        cout<<"bloqueFinal => "<<bloqueFinal<<"\n";
    }
    
    void printInfoSector(int nSector)
    {
        cout << "\n" << lineas::drawLinea(75) << " | SECTOR " << nSector << " | " << lineas::drawLinea(75) << "\n";

        int _pista, _superf, _plat;
        _pista = ((nSector) % disco->numSectoresPorPista == 0) ? ((nSector) / disco->numSectoresPorPista) : ((nSector) / disco->numSectoresPorPista) + 1;
        _superf = (_pista % disco->numPistasPorSuperficie == 0) ? (_pista / disco->numPistasPorSuperficie) : (_pista / disco->numPistasPorSuperficie) + 1;
        _plat = (_superf % 2 == 0) ? (_superf / 2) : (_superf / 2) + 1;
        char c;
        cout<<"\n Informacion del Sector: "<<endl;
        cout << "\t" << " IdSector: " << nSector << endl;
        cout << "\t" << " numDePista: " << _pista << endl;
        cout << "\t" << " numDeSuperficie: " << _superf << endl;
        cout << "\t" << " numDePlato: " << _plat << endl;
        //cout << "DATA:\n" << lineas::linea100 << lineas::linea50 << "\n";
        //cout << "\n" << lineas::linea100 << lineas::linea50 << "\n";
        //return {nSector,_pista,_superf,_plat};
        //input.close();
    }

    int getBloque_Of_Free_space_map()
    {
        int bloqueInicial;
        int bloqueFinal;
        fstream dictionary("dictionary/dictionary.bin", ios::in | ios::out | ios::binary);
        int pos = buscarTablaenDictionary(nameTable);
        dictionary.seekg(pos + 104);
        dictionary.read(reinterpret_cast<char *>(&bloqueInicial), sizeof(int));
        dictionary.read(reinterpret_cast<char *>(&bloqueFinal), sizeof(int));
        dictionary.close();
        int elqueTieneMasEspacio = espacioLibreBloque(bloqueInicial);
        int indice = bloqueInicial;
        // cout<< elqueTieneMasEspacio ;
        for (int i = bloqueInicial; i <= bloqueFinal; i++)
        {
            // free_espace_map.emplace_back(espacioLibreBloque(i));
            if(espacioLibreBloque(i) > elqueTieneMasEspacio){
                elqueTieneMasEspacio = espacioLibreBloque(i);
                indice = i;
            }
        }
        // int mayor = free_espace_map[0];
        // for (int i = 1; i < free_espace_map.size(); i++)
        // {
        //     if (free_espace_map[i] > mayor)
        //     {
        //         mayor = free_espace_map[i];
        //     }
        // }
        return indice;
    }

    void setBloque_SlottedPage(int bloqueNum)
    {
        fstream bloque("disk/bloque" + to_string(bloqueNum) + ".bin", ios::in | ios::out | ios::binary);
        bloque.seekp(0);

        int numEntradas = 0;
        bloque.write(reinterpret_cast<char *>(&numEntradas), sizeof(int));
        int finalDeFreeSpace = sizeBloque - 1; // con 4096 bytes el ultimo byte seria el 4095
        bloque.write(reinterpret_cast<char *>(&finalDeFreeSpace), sizeof(int));
        bloque.close();
    }

    void insertarRegistroVariableEnBloque_SlottedPage(int bloqueId, vector<char> &registroVariable)
    {
        int longitudRegistro = registroVariable.size(); // Obtener el tamaño del registro variable
        cout << "longitud REGISTRO => " << longitudRegistro << endl;
        fstream bloque("disk/bloque" + to_string(bloqueId) + ".bin", ios::in | ios::out | ios::binary);
        int ptrFinDeFreeSpace;
        int numEntradas;
        bloque.read(reinterpret_cast<char *>(&numEntradas), sizeof(int));       // Lee el numero de entradas
        bloque.read(reinterpret_cast<char *>(&ptrFinDeFreeSpace), sizeof(int)); // Lee el puntero fin de freespace

        bloque.seekp(ptrFinDeFreeSpace - longitudRegistro, std::ios::beg); // Se ubica en la posición del puntero fin de freespace
        // cout << "posCursorEscritura => " << bloque.tellp() << endl;
        bloque.write(registroVariable.data(), longitudRegistro); // Inserta registroVariable en el bloque
        // cout<<"|||"<<registroVariable.data()<<"|||"<<endl;
        bloque.seekp((8 + (numEntradas * 8)), std::ios::beg); // Se ubica en la entrada
        // cout << "posCursorEscritura => " << bloque.tellp() << endl;
        int _offset = ptrFinDeFreeSpace - longitudRegistro; // Posicion donde comienza data del atributo variable
        int _length = longitudRegistro;

        bloque.write(reinterpret_cast<char *>(&_offset), sizeof(int));
        bloque.write(reinterpret_cast<char *>(&_length), sizeof(int));

        bloque.seekp(0, std::ios::beg);
        numEntradas++;                                                     // Incrementa numero de entradas
        bloque.write(reinterpret_cast<char *>(&numEntradas), sizeof(int)); // Actualiza

        ptrFinDeFreeSpace = ptrFinDeFreeSpace - longitudRegistro;                // Reduce freespace
        bloque.write(reinterpret_cast<char *>(&ptrFinDeFreeSpace), sizeof(int)); // Actualiza
        cout << ptrFinDeFreeSpace << endl;

        if ((numEntradas - 1) == 0)
        {
            bloque.write(reinterpret_cast<char *>(&ptrFinDeFreeSpace), sizeof(int)); // Actualiza
            bloque.close();
        }
        else
        {
            for (int i = 0; i < numEntradas - 1; i++)
            {
                bloque.seekp(8, std::ios::cur);
            }
            bloque.write(reinterpret_cast<char *>(&ptrFinDeFreeSpace), sizeof(int)); // Actualiza
            bloque.close();
        }
        bloqueASector(bloqueId);
    }

    void buscarRegistroVariableEnBloque_SlottedPage(int bloqueId, vector<char> &registroVariable)
    {
    }


    vector<int> convierteBytesAInts(const std::vector<char> &bytes, int numAtributos) {
        vector<int> nullbitmap_INT;
        int contadorAtributos = 1;
        bool bandSalir = false;
        for (char byte : bytes) {
            for (int i = 7; i >= 0; i--) {
                int bit = (byte >> i) & 1;
                nullbitmap_INT.push_back(bit);
            }
        }
        return nullbitmap_INT;
    }

    bool IsAtributoVariable(string tipo){
        if (tipo == "str")
            return true;
        return false;
    }

    // convierte el registro de binario a caracteres para mostrarse por consola
    void showRegistro(vector<char>& registroBin, vector<tuple<string, string, int>> info){
        vector<char> nullbitmap_Bytes;
        int sizeEnBytesDelNullBitmap = (int)(std::ceil(((double)info.size()/8)));
        // cout<< "sizeEnBytesDelNullBitmap: "<<sizeEnBytesDelNullBitmap<<"\n";
        int contadorBytesActual = 0;
        for(int i=0; i<sizeEnBytesDelNullBitmap; i++){
            nullbitmap_Bytes.emplace_back(registroBin[i]);
            contadorBytesActual++;
        }
        
        vector<int> nullbitmap_INT = convierteBytesAInts(nullbitmap_Bytes,info.size());
        nullbitmap_INT.erase(nullbitmap_INT.begin()+info.size(), nullbitmap_INT.end());
        cout<<"(nullbitmap del registro: ";
        for(int i=0; i<nullbitmap_INT.size(); i++){
            cout<<nullbitmap_INT[i]<<" ";
        }
        cout<<") ";

        int resultado = 0;
        for (int i = 2; i <= 5; i++) {
            resultado = (resultado << 8) | static_cast<unsigned char>(registroBin[i]);
        }
        cout<<"primer atributo => "<<resultado<<endl;
        return;

        int temp = 0;
        for (int k = contadorBytesActual; k <= contadorBytesActual+(sizeof(int)-1); k++) {
            temp = (temp << 8) | static_cast<unsigned char>(registroBin[k]);
        }
        contadorBytesActual += 4;
        cout<<temp<<" ";

        return;
        for(int i=0; i<nullbitmap_INT.size();i++){
            // recorrer los 12 y de acuerdo al nullbitmap mostrar o no
            if(nullbitmap_INT[i]==1) continue;
            if (IsAtributoVariable(get<1>(info[i])) && nullbitmap_INT[i]==0){
                contadorBytesActual +=4;
                continue;
                //colaAux.emplace_back(make_tuple(contadorDeBytes, palabra)); // posicion donde empiezan los 4 bytes que guarda el par(offset, lenght)
                short int offset = (static_cast<unsigned char>(registroBin[contadorBytesActual]) << 8) | static_cast<unsigned char>(registroBin[contadorBytesActual+1]);
                short int lenght = (static_cast<unsigned char>(registroBin[contadorBytesActual+2]) << 8) | static_cast<unsigned char>(registroBin[contadorBytesActual+3]);;
                
                cout<<"[offset:"<<offset<<"]";
                cout<<"[lenght:"<<lenght<<"]";
                contadorBytesActual +=4; // par (offset 2 bytes, longitud 2 bytes)
            } else {
                if(get<1>(info[i])=="int"){
                    int temp = 0;
                    for (int k = contadorBytesActual; k <= contadorBytesActual+(sizeof(int)-1); k++) {
                        temp = (temp << 8) | static_cast<unsigned char>(registroBin[k]);
                    }
                    contadorBytesActual += 4;
                    cout<<temp<<" ";
                } else if(get<1>(info[i])=="double"){
                    double temp = 0;
                    for (int k = contadorBytesActual; k <= contadorBytesActual+(sizeof(double)-1); k++) {
                        temp = (temp * 256) + static_cast<unsigned char>(registroBin[contadorBytesActual + k]);
                        contadorBytesActual++;
                    }
                    contadorBytesActual += 8;
                    cout<<temp<<" ";
                }
            }
        }

        cout<<"\n";
        cout<<"info dentro de showRegistro Method"<<endl;
        for (const auto& item : info) {
            string nombreAtributo = get<0>(item);
            string dataAtributo = get<1>(item);
            int sizeTipo = get<2>(item);
            // Hacer algo con los datos
            cout << "nombreAtributo: " << nombreAtributo << endl;
            cout << "dataAtributo: " << dataAtributo << endl;
            cout << "sizeTipo: " << sizeTipo << endl;
            cout << "--------------------" << endl;
        }
    }

    void printBloque_SlottedPage(int bloqueId)
    {
        cout<<lineas::linea50<<"BLOQUE SLOTTEDPAGE ["<<bloqueId<<"]"<<lineas::linea50<<endl;
        cout<<"|Estructura SlottedPage|"<<endl;
        string bloqueStruct = "[numEntradas][ptrFinFreeSpace][Entradas]\n" 
        "[free space]\n.\n.\n.\n";

        cout<<bloqueStruct<<endl;

        fstream bloque("disk/bloque" + to_string(bloqueId) + ".bin", ios::in | ios::out | ios::binary);
        int numEntradas;
        int ptrFinDeFreeSpace;

        bloque.read(reinterpret_cast<char *>(&numEntradas), sizeof(int));
        bloque.read(reinterpret_cast<char *>(&ptrFinDeFreeSpace), sizeof(int));

        for(int i=numEntradas; i>=1; i--){
            cout<<"R"<<i<<"[nullbitmap|parte_fija|parte_variable]\n";       
        }
        cout<<lineas::linea100<<endl;

        cout << "[numEntradas => " << numEntradas << "]";
        cout << "[ptrFinDeFreeSpace => " << ptrFinDeFreeSpace << "]"<<endl;

        vector<vector<char>> registros;
        vector<char> bufferDinamico;

        for (int i = 0; i < numEntradas; i++)
        {
            int _offset;
            int _length;

            bloque.read(reinterpret_cast<char *>(&_offset), sizeof(int));
            bloque.read(reinterpret_cast<char *>(&_length), sizeof(int));

            cout << "[offset_" << i << " => " << _offset << "]";
            cout << "[length_" << i << " => " << _length << "]" <<endl;

            std::streampos posicion = bloque.tellg(); // guardo posicion

            bloque.seekg(_offset, ios::beg);
            
            //---------------- esto agregue, en lugar de char * vector<char>
            bufferDinamico.clear();
            bufferDinamico.resize(_length); // Utilizar vector<char> con el tamaño _length
            bloque.read(bufferDinamico.data(), _length); // Leer los datos en el vector
            registros.push_back(bufferDinamico);
            // ---------------------------

            // char *bufferDinamico = new char[_length];
            // bloque.read(bufferDinamico, _length);

            // for (int j = 0; j < _length; j++) {
            //     cout << bufferDinamico[j];
            // }
            // cout << endl;
            // delete[] bufferDinamico;

            bloque.seekg(posicion, ios::beg);
        }
        //Para imprimir los registros de binario a caracteres implementar registerBinarioAchar(vector<char>& registro)
        for (int i = 0; i < registros.size(); i++) {
            cout << "[Registro_" << registros.size()-i << " => ";
            showRegistro(registros[i],info);
            for (char ch : registros[i]) {
                cout << static_cast<int>(ch);
            }
            cout <<"]"<< endl;
        }
        // showRegistro(registros[0],info);
        bloque.close();
    }

   
    int espacioLibreBloque(int bloqueId)
    {
        fstream bloque("disk/bloque" + to_string(bloqueId) + ".bin", ios::in | ios::out | ios::binary);
        int numEntradas;
        int ptrFinDeFreeSpace;
        int espacioLibre;

        bloque.read(reinterpret_cast<char *>(&numEntradas), sizeof(int));
        bloque.read(reinterpret_cast<char *>(&ptrFinDeFreeSpace), sizeof(int));
        bloque.close();

        if (numEntradas == 0)
        {
            return sizeBloque - (2 * 4); // numEntradas y ptrFinDeFreeSpace
        }
        espacioLibre = (sizeBloque - (numEntradas * 8)) - (sizeBloque - (ptrFinDeFreeSpace - 1));
        return espacioLibre;
    }

    void AgregarTablaVariable()
    {
        cout << "Teniendo en cuenta que un bloque tiene " << sizeBloque << " bytes.\nSeleccione el numero de bloques que desea para la tabla: ";
        string aux;
        getline(cin, aux);
        int rango = stoi(aux);
        int temp = 0;

        fstream dictionary("dictionary/dictionary.bin", ios::in | ios::out | std::ios::binary);
        int nTablas, nFinal;

        dictionary.seekg(4);
        dictionary.read(reinterpret_cast<char *>(&nFinal), sizeof(int));
        dictionary.read(reinterpret_cast<char *>(&nTablas), sizeof(int));
        nTablas++;
        dictionary.seekp(8);
        dictionary.write(reinterpret_cast<char *>(&nTablas), sizeof(int));
        cout << "Ingrese el nombre de la tabla: ";
        getline(cin, aux);
        dictionary.seekp(0, ios::end);
        aux.resize(100, ' ');
        dictionary.write(aux.c_str(), aux.length());
        dictionary.write(reinterpret_cast<char *>(&temp), sizeof(int));
        nFinal++;
        rango += nFinal;
        dictionary.write(reinterpret_cast<char *>(&nFinal), sizeof(int));
        dictionary.write(reinterpret_cast<char *>(&rango), sizeof(int));
        dictionary.write(reinterpret_cast<char *>(&temp), sizeof(int));

        dictionary.seekp(0, ios::beg);
        dictionary.seekp(4);
        dictionary.write(reinterpret_cast<char *>(&rango), sizeof(int));
        dictionary.close();
    }

    void WriteDataRegistroEnBloque(int bloqueNum, int sizeRegistro, char *data)
    {
        fstream bloque("disk/bloque" + to_string(bloqueNum) + ".bin", ios::in | ios::out | ios::binary);
        bloque.seekp(0);
        bloque.seekg(0);

        int numEntradas;
        bloque.read(reinterpret_cast<char *>(&numEntradas), sizeof(int));
        numEntradas++;
        bloque.write(reinterpret_cast<char *>(&(numEntradas)), sizeof(int));

        int finalDeFreeSpace;
        bloque.read(reinterpret_cast<char *>(&finalDeFreeSpace), sizeof(int));
        finalDeFreeSpace -= sizeRegistro;
        bloque.write(reinterpret_cast<char *>(&finalDeFreeSpace), sizeof(int));

        tableToVector();
        bloque.seekp(9);
        // bloque.seekp(finalDeFreeSpace - sizeRegistro);
        bloque.write(data, sizeRegistro); // AQUI OBSERVACION
        // Llama al disco y escribe en disco
    }

    void readPrueba_SlottedPage(int bloqueNum)
    {
        fstream bloque("disk/bloque" + to_string(bloqueNum) + ".bin", ios::in | ios::out | ios::binary);
        int num;
        int num2;
        bloque.seekg(0);
        bloque.read(reinterpret_cast<char *>(&num), sizeof(num));
        bloque.read(reinterpret_cast<char *>(&num2), sizeof(num2));
        cout << "aux1 = " << num << " \t\taux2 =" << num2 << endl;
        bloque.close();
    }

    void variableTableToVector()
    {
        this->info.clear();
        ifstream data;
        data.open("dictionary/variableSchema", ios::in);
        // data.open("esquema", ios::in);
        string linea;
        while (getline(data, linea))
        {
            stringstream stream(linea);
            string word;
            getline(stream, word, '#');
            if (word == nameTable)
            {
                break;
            }
        }
        // Lleva a la memoria dinamica la ubicacion de los bytes por cada atributo
        // cout<<linea<<endl;
        stringstream stream(linea);
        string w1, w2, w3;
        getline(stream, w1, '#');
        getline(stream, w1, '#');
        while (!stream.eof())
        {
            getline(stream, w1, '#');
            getline(stream, w2, '#');
            if ((w2 == "int"))
            {
                info.push_back(make_tuple(w1, w2, mytipos::_INT));
                continue;
            }
            else if (w2 == "double")
            {
                info.push_back(make_tuple(w1, w2, mytipos::_DOUBLE));
                continue;
            }
            else if (w2 == "float")
            {
                info.push_back(make_tuple(w1, w2, mytipos::_FLOAT));
                continue;
            }
            else if (w2 == "bool")
            {
                info.push_back(make_tuple(w1, w2, mytipos::_BOOL));
                continue;
            }
            getline(stream, w3, '#');
            info.push_back(make_tuple(w1, w2, stoi(w3)));
        }
        data.close();
    }

    // Escribe el bloque virtual al disco fisico
    void bloqueASector(int nBloque)
    {
        ifstream archivo("disk/bloque" + to_string(nBloque) + ".bin", std::ios::binary);
        for (int i = 1; i <= numSectoresPorBloque; i++)
        {
            int nSector = numSectoresPorBloque * (nBloque - 1) + i;
            // cout<<"nSector -> "<<nSector<<endl;
            int _pista, _superf, _plat;
            _pista = ((nSector) % disco->numSectoresPorPista == 0) ? ((nSector) / disco->numSectoresPorPista) : ((nSector) / disco->numSectoresPorPista) + 1;
            _superf = (_pista % disco->numPistasPorSuperficie == 0) ? (_pista / disco->numPistasPorSuperficie) : (_pista / disco->numPistasPorSuperficie) + 1;
            _plat = (_superf % 2 == 0) ? (_superf / 2) : (_superf / 2) + 1;
            ofstream output("HDD/Plato" + to_string(_plat) + "/Superficie" + to_string(_superf) + "/Pista" + to_string(_pista) + "/Sector" + to_string(nSector) + ".bin", std::ios::binary);

            char *buffer = new char[disco->capacidadDelSector];
            archivo.read(buffer, disco->capacidadDelSector);
            output.write(buffer, disco->capacidadDelSector);
            delete[] buffer;
            output.close();
        }
        archivo.close();
    }

    // Transforma la informacion de una tabla en un vector para mejor manejo
    void tableToVector()
    { // cambios
        this->info.clear();
        ifstream data;
        data.open("esquema", ios::in);
        string linea;
        while (getline(data, linea))
        {
            stringstream stream(linea);
            string word;
            getline(stream, word, '#');
            if (word == nameTable)
            {
                break;
            }
        }
        // Lleva a la memoria dinamica la ubicacion de los bytes por cada atributo
        // cout<<linea<<endl;
        stringstream stream(linea);
        string w1, w2, w3;
        getline(stream, w1, '#');
        getline(stream, w1, '#');
        while (!stream.eof())
        {
            getline(stream, w1, '#');
            getline(stream, w2, '#');
            if ((w2 == "int"))
            {
                info.push_back(make_tuple(w1, w2, mytipos::_INT));
                continue;
            }
            else if (w2 == "double")
            {
                info.push_back(make_tuple(w1, w2, mytipos::_DOUBLE));
                continue;
            }
            else if (w2 == "float")
            {
                info.push_back(make_tuple(w1, w2, mytipos::_FLOAT));
                continue;
            }
            else if (w2 == "bool")
            {
                info.push_back(make_tuple(w1, w2, mytipos::_BOOL));
                continue;
            }
            getline(stream, w3, '#');
            info.push_back(make_tuple(w1, w2, stoi(w3)));
        }
        // cout<<info.size()<<"tam"<<endl;
        //    for(int i=0; i<(int)info.size(); i++){
        //      cout<<get<0>(info[i])<<"-";
        //      cout<<get<1>(info[i])<<"-";
        //      cout<<get<2>(info[i])<<"\n";
        //    }
        data.close();
    }

    // Importa la tabla al disco
    void uploadTableToDisk(string fileToImport, string tablaName)
    {
        tableToVector(); // cambios
        cout << "size -> " << info.size() << endl;
        cout << "name -> " << tablaName << endl;
        convertCSV_inTuplas(fileToImport, nameTable, (int)info.size()); // cambios

        const char *carpetaDisco = "disco\\";

        int contador = 0;

        char caracter;
        int entero;
        float decimal;
        double decimalGrande;

        ifstream archivoTupla(nameTable); // cambios
        // ifstream archivoTupla("titanic");
        ofstream salida("titanicbinario.bin", std::ios::binary);

        char c;
        char z;

        string linea;
        string palabra;
        int contadorBytes = 0;
        int numRegistrosDeLaTabla = 0;
        while (getline(archivoTupla, linea))
        {
            stringstream stream(linea);
            int indice = 0;
            while (getline(stream, palabra, '#'))
            { // palabra 5    20
                if (get<1>(info[indice]) == "int")
                {
                    if (palabra == "")
                    {
                        entero = -1;
                    }
                    else
                    {
                        entero = stoi(palabra);
                    }
                    salida.write(reinterpret_cast<char *>(&entero), sizeof(entero));
                }
                else if (get<1>(info[indice]) == "double")
                {
                    if (palabra == "")
                    {
                        decimalGrande = -1;
                    }
                    else
                    {
                        decimalGrande = stod(palabra);
                    }
                    salida.write(reinterpret_cast<char *>(&decimalGrande), sizeof(decimalGrande));
                }
                else if (get<1>(info[indice]) == "str")
                {
                    for (auto &i : palabra)
                    {
                        salida.write(reinterpret_cast<char *>(&i), sizeof(char));
                    }
                    int bytesPorCompletar = get<2>(info[indice]) - palabra.size();
                    for (auto i = 0; i < bytesPorCompletar; i++)
                    {
                        z = ' ';
                        salida.write(reinterpret_cast<char *>(&z), sizeof(char));
                    }
                }
                indice++;
            }
            numRegistrosDeLaTabla++;
        }

        // cout<<"\n-------numRegistrosDeLaTabla: "<<numRegistrosDeLaTabla<<"\n";
        fstream dictionary1("dictionary/dictionary.bin", ios::in | ios::out | std::ios::binary);
        int posicion = buscarTablaenDictionary(nameTable) + 100;
        // cout<<"Posicion -> "<<posicion<<endl;
        // cout<<"NUMERO DE REGIS -> "<<numRegistrosDeLaTabla<<endl;
        dictionary1.seekp(posicion);
        dictionary1.write(reinterpret_cast<char *>(&numRegistrosDeLaTabla), sizeof(int));
        dictionary1.close();

        // for ( auto& atributo : info ){
        //     cout<<get<0>(atributo)<<"-";
        //     cout<<get<1>(atributo)<<"-";
        //     cout<<get<2>(atributo)<<"\n";
        // }

        archivoTupla.close();
        salida.close();

        int sizeRegistro = 0;
        for (auto &i : info)
        {
            if (get<1>(i) == "int")
            {
                sizeRegistro += 4;
            }
            else if (get<1>(i) == "double")
            {
                sizeRegistro += 8;
            }
            else if (get<1>(i) == "str")
            {
                sizeRegistro += get<2>(i);
            }
        }
        setDataInBloques(sizeRegistro);
    }

    int buscarTablaenDictionary(string objetivo)
    {
        fstream dictionary("dictionary/dictionary.bin", ios::in | ios::out | std::ios::binary);
        dictionary.seekg(0, ios::end);
        int tamañoDictionary = dictionary.tellg();
        dictionary.seekg(0, ios::beg);
        int byte = 12; // Ahi inician las tablas
        while (byte <= tamañoDictionary)
        {
            dictionary.seekg(byte);
            string temp = "";
            char c;
            for (int i = 0; i < 100; i++)
            {
                dictionary.read(reinterpret_cast<char *>(&c), sizeof(c));
                temp += c;
            }
            // cout<<temp;
            string aux;
            for (char ch : temp)
            {
                if (!std::isspace(ch))
                {
                    aux += ch;
                }
            }
            if (aux == objetivo)
            {
              cout<<aux<<" -- "<<objetivo;
                dictionary.close();
                return byte;
            }
            else
            {
                byte += 116;
            }
        }
        dictionary.close();
        return 0;
    }

    void setDataInBloques(int sizeRegistro)
    {
        int contador = 1;
        int cnt = 1;
        int contadorBytesTotal;
        int contAux = 0;
        int numRegistros = 0;

        fstream numRegistrosPorTabla_dictionary("dictionary/dictionary.bin", ios::in | ios::out | std::ios::binary);
        int posicion = buscarTablaenDictionary(nameTable) + 100;
        numRegistrosPorTabla_dictionary.seekg(posicion);
        numRegistrosPorTabla_dictionary.read(reinterpret_cast<char *>(&numRegistros), sizeof(int));
        numRegistrosPorTabla_dictionary.close();

        ifstream file("titanicbinario.bin", std::ios::binary);
        // ofstream schemaBloques("dictionary/schemaBloquesFijos.bin", std::ios::binary);

        file.seekg(0, ios::end);
        int sizeFile = file.tellg();
        file.seekg(0, ios::beg);

        int almacenamientoTotal = this->nTotalBloques * this->sizeBloque;

        int contadorRegistros = 0;
        fstream _dictionary("dictionary/dictionary.bin", ios::in | ios::out | std::ios::binary);
        _dictionary.seekg(4);
        _dictionary.read(reinterpret_cast<char *>(&contador), sizeof(int));
        contador++;
        _dictionary.close();
        while ((contAux < almacenamientoTotal) && (contadorRegistros < numRegistros))
        { // sizeFile
            // cout<<"entro"<<endl;
            ofstream bloque("disk/bloque" + to_string(contador) + ".bin", std::ios::binary);
            char marcador = '-'; // puntero inicial de la freelist que se inicializara en 0
            bloque.write(reinterpret_cast<char *>(&marcador), sizeof(char));
            bloque.seekp(sizeRegistro - sizeof(int) - sizeof(char));

            int freelist = 0; // puntero inicial de la freelist que se inicializara en 0
            bloque.write(reinterpret_cast<char *>(&marcador), sizeof(char));
            bloque.write(reinterpret_cast<char *>(&freelist), sizeof(int));
            contadorBytesTotal = sizeRegistro * 2;

            while ((contadorBytesTotal <= sizeBloque) && (contadorRegistros < numRegistros))
            { // sizeRegistro <= (sizeBloque-contadorBytesTotal)
                char *buffer = new char[sizeRegistro];
                file.read(buffer, sizeRegistro);
                bloque.write(buffer, sizeRegistro);
                contadorBytesTotal += sizeRegistro;
                delete[] buffer;
                contadorRegistros++;
                // cout<<"contadorRegistros ->>>>>>>>>>> "<<contadorRegistros<<endl;
            }

            contAux += (contadorBytesTotal - sizeRegistro);
            // int numRegistrosEnBloque = (contAux / (sizeRegistro*contador)) - 1;
            // schemaBloques.write(reinterpret_cast<char*>(&contadorRegistros), sizeof(int));

            char *buffer = new char[sizeBloque - contadorBytesTotal + sizeRegistro];
            // Rellenar el buffer con datos
            for (int j = 0; j < (sizeBloque - contadorBytesTotal + sizeRegistro); j++)
            {
                buffer[j] = '*';
            }
            bloque.write(buffer, (sizeBloque - contadorBytesTotal + sizeRegistro));
            delete[] buffer;

            contAux += (sizeBloque - contadorBytesTotal + sizeRegistro);
            // cout<<"contAux: "<<contAux<<"\t\t";

            bloque.close();

            bloqueASector(contador);
            contador++;
        }
        contador--;
        fstream dictionary("dictionary/dictionary.bin", ios::in | ios::out | std::ios::binary);
        int inicioBLoque, finBLoque;
        dictionary.seekp(0, ios::beg);
        dictionary.seekg(4);
        dictionary.read(reinterpret_cast<char *>(&inicioBLoque), sizeof(int));
        dictionary.seekp(0, ios::beg);

        if (inicioBLoque == 0)
        {
            dictionary.seekp(0, ios::beg);
            int posicion = buscarTablaenDictionary(nameTable) + 104;
            dictionary.seekp(posicion);
            inicioBLoque++;
            dictionary.write(reinterpret_cast<char *>(&inicioBLoque), sizeof(int));
            dictionary.write(reinterpret_cast<char *>(&contador), sizeof(int));
            dictionary.close();
        }
        else
        {
            dictionary.seekp(0, ios::beg);
            int posicion = buscarTablaenDictionary(nameTable) + 104;
            dictionary.seekp(posicion);
            inicioBLoque++;
            dictionary.write(reinterpret_cast<char *>(&inicioBLoque), sizeof(int));
            dictionary.write(reinterpret_cast<char *>(&contador), sizeof(int));
            dictionary.close();
        }

        fstream dictionary1("dictionary/dictionary.bin", ios::in | ios::out | std::ios::binary);
        dictionary1.seekp(0, ios::beg);
        dictionary1.seekp(4);
        dictionary1.write(reinterpret_cast<char *>(&contador), sizeof(int));
        dictionary1.close();

        file.close();
        this->NumBLoquesEnUso = contador;
        // schemaBloques.close();
    }

    void showDictionary()
    {
        ifstream dictionary("dictionary/dictionary.bin", std::ios::binary);
        dictionary.seekg(0, ios::end);
        int tamañoDictionary = dictionary.tellg();
        dictionary.seekg(0, ios::beg);
        // cout<<"Size -> "<<tamañoDictionary<<endl;
        int byte = 0;
        int auxInt;
        string auxStr;
        dictionary.read(reinterpret_cast<char *>(&auxInt), sizeof(int));
        cout << "Numero total de bloques: " << auxInt << endl;
        dictionary.read(reinterpret_cast<char *>(&auxInt), sizeof(int));
        cout << "Numero de bloques en uso: " << auxInt << endl;
        dictionary.read(reinterpret_cast<char *>(&auxInt), sizeof(int));
        cout << "Numero de total de tablas: " << auxInt << endl;
        byte = 12;
        while (byte < tamañoDictionary)
        {
            string temp = "";
            char c;
            for (int i = 0; i < 100; i++)
            {
                dictionary.read(reinterpret_cast<char *>(&c), sizeof(c));
                temp += c;
            }
            // cout<<temp;
            string aux;
            for (char ch : temp)
            {
                if (!std::isspace(ch))
                {
                    aux += ch;
                }
            }
            cout << " ->Tabla : " << aux << endl;
            dictionary.read(reinterpret_cast<char *>(&auxInt), sizeof(int));
            cout << "   Numero de total de registros: " << auxInt << endl;
            dictionary.read(reinterpret_cast<char *>(&auxInt), sizeof(int));
            cout << "   Bloque inicial: " << auxInt << endl;
            dictionary.read(reinterpret_cast<char *>(&auxInt), sizeof(int));
            cout << "   Bloque final: " << auxInt << endl;
            dictionary.read(reinterpret_cast<char *>(&auxInt), sizeof(int));
            //cout << "   Fijo o Variable: " << auxInt << endl;
            byte += 116;
        }
        dictionary.close();
    }

    void convertCSV_inTuplas(string fileAimportar, string newfile, int natributos)
    {
        cout << "size -> " << natributos << endl;
        tableToVector();
        ifstream i(fileAimportar, ios::in);
        ofstream f(newfile, ios::out);
        bool reemplazarActivo = true;
        int indice = 0;
        string word = "";
        if (i && f)
        {
            char caracter;
            while (i.get(caracter))
            {
                if (((caracter == ',') || (caracter == '\n')) && reemplazarActivo == true)
                {
                    if (indice < natributos)
                    { // natributos
                        f << word;
                        f << '#';
                        word = "";
                        indice++;
                        if (indice >= natributos)
                        { // natributos//cambio
                            indice = 0;
                            f << '\n';
                        }
                    }
                }
                else if (caracter == '"')
                {
                    reemplazarActivo = (!reemplazarActivo);
                }
                else
                {
                    if (caracter != '\r')
                        word += caracter;
                }
            }
            cout << "Archivo importado exitosamente." << endl;
        }
        i.close();
        f.close();
    }

    void printSector(int nSector)
    {
        cout << "\n"
             << lineas::drawLinea(75) << " | SECTOR " << nSector << " | " << lineas::drawLinea(75) << "\n";

        int _pista, _superf, _plat;
        _pista = ((nSector) % disco->numSectoresPorPista == 0) ? ((nSector) / disco->numSectoresPorPista) : ((nSector) / disco->numSectoresPorPista) + 1;
        _superf = (_pista % disco->numPistasPorSuperficie == 0) ? (_pista / disco->numPistasPorSuperficie) : (_pista / disco->numPistasPorSuperficie) + 1;
        _plat = (_superf % 2 == 0) ? (_superf / 2) : (_superf / 2) + 1;
        char c;
        cout << "\t"
             << " IdSector: " << nSector << endl;
        cout << "\t"
             << " numDePista: " << _pista << endl;
        cout << "\t"
             << " numDeSuperficie: " << _superf << endl;
        cout << "\t"
             << " numDePlato: " << _plat << endl;
        cout << "DATA:\n"
             << lineas::linea100 << lineas::linea50 << "\n";
        ifstream input("HDD/Plato" + to_string(_plat) + "/Superficie" + to_string(_superf) + "/Pista" + to_string(_pista) + "/Sector" + to_string(nSector) + ".bin", std::ios::binary);
        for (int i = 0; i < this->disco->capacidadDelSector; i++)
        {
            input.read(static_cast<char *>(&c), sizeof(char));
            cout << c;
        }
        cout << "\n"
             << lineas::linea100 << lineas::linea50 << "\n";
        input.close();
    }

    void printBloque(int numBloque)
    {
        cout << "\n"
             << lineas::drawLinea(75) << " | BLOQUE " << numBloque << " | " << lineas::drawLinea(75) << "\n";
        // this->disco->sectores[numSector-1].showInfoSector(); // Imprime info: en que plato, superficie, pista esta

        cout << "DATA:\n"
             << lineas::linea100 << lineas::linea50 << "\n";
        ifstream bloqueUbicado("disk/bloque" + std::to_string(numBloque) + ".bin", ios::binary);
        char c;
        int contadorFila = 1;
        while (!bloqueUbicado.eof())
        {
            bloqueUbicado.read(static_cast<char *>(&c), sizeof(char));
            cout << c;
            // cout << static_cast<int>(c);
            contadorFila++;
            // if(contadorFila%125==0){ cout<<"\n"; }
        }
        bloqueUbicado.close();

        cout << "\n"
             << lineas::linea100 << lineas::linea50 << "\n";
    }
};

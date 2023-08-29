#include <iostream>
#include <cstring>
#include <fstream>

using namespace std;

void fun_int(char * frame,int & byte, int sizeBytes){
  int pru;
  pru = *reinterpret_cast<int*>(frame+byte);
  std::cout <<pru<<"|";
  byte+=sizeBytes;
}
void fun_float(char * frame,int & byte, int sizeBytes){
  int flo;
  flo = *reinterpret_cast<float*>(frame+byte);
  std::cout <<flo<<"|";
  byte+=sizeBytes;
}
void fun_double(char * frame,int & byte, int sizeBytes){
  int dou;
  dou = *reinterpret_cast<double*>(frame+byte);
  std::cout <<dou<<"|";
  byte+=sizeBytes;
}
void fun_char(char * frame, int & byte, int sizeBytes){
  char texto;
  for (int i=0; i<sizeBytes; i++) {
    texto = *reinterpret_cast<char*>(frame+byte);
    std::cout <<texto;
    byte+=1;
  }
  std::cout <<"|";
}
int main() {
    const int size_registro = 173;
    const int size_bloque = 1000; 
    char * frame = new char[size_bloque];
    ifstream file;
    file.open("disk/bloque1.bin",ios::in | ios::binary);
    file.read(frame,size_bloque);

    cout<<"aqui"<<endl;
    int byte =0;
    byte+=173-4;
    cout<<"Freelist -> ";
    fun_int(frame,byte,4);
    cout<<endl;
    for (int i=1; i<(size_bloque / size_registro); i++) {
       //1
      fun_int(frame,byte,4);
      //2
      fun_int(frame,byte,4);
      //3
      fun_int(frame,byte,4);
      //registro
      fun_char(frame,byte,90);
      //Sexo
      fun_char(frame,byte,6);
      //AGE
      fun_double(frame,byte,8);
      //SIBSb
      fun_int(frame,byte,4);
      //parch
      fun_int(frame,byte,4);
      //Ticket
      fun_char(frame,byte,20);
      //Fare
      fun_double(frame,byte,8);
      //Cabin
      fun_char(frame,byte,20);
      //Embarked
      fun_char(frame,byte,1);
      //Fin del registro
      std::cout <<"|"<< std::endl;
    }
    file.close();
  return 0;
}











/*
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

int main(){

    char * data = new char[1000];
    ifstream file("disk/bloque1.bin", ios::binary);
    file.read(data,1000);

    file.close();

    int* ptrInt;
    double* ptrDouble;
    char* ptrChar;

    string cadena;

    vector<pair<string,string>> tipos = {
        {"int","4"},
        {"int","4"},
        {"int","4"},

        {"str","90"},
        {"str","6"},
        {"double","8"},

        {"int","4"},
        {"int","4"},
        {"str","20"},

        {"double","8"},
        {"str","20"},
        {"str","1"},
    };

    vector<int> byteInicio;
    byteInicio.emplace_back(0);

    ptrInt = (reinterpret_cast<int*>(&data[173+0]));
    cout<<*ptrInt<<"|";
    ptrInt = (reinterpret_cast<int*>(&data[173+4]));
    cout<<*ptrInt<<"|";
    ptrInt = (reinterpret_cast<int*>(&data[173+8]));
    cout<<*ptrInt<<"|";
    return 0;


    int contador = 0;
    for (int k=0; k<5; k++){
        for(int i=0; i<tipos.size(); i++){
            // cout<<tipos[i].first<<endl;
            if(tipos[i].first == "int"){
                ptrInt = (reinterpret_cast<int*>(data[contador]));
                contador+=4;
                cout<<*ptrInt<<"|";
            } else if (tipos[i].first == "double"){
                ptrDouble = (reinterpret_cast<double*>(data));
                cout<<*ptrDouble<<"|";
            } else if (tipos[i].first == "str"){
                for(int j = 0; j<stoi(tipos[i].second); j++){
                    ptrChar = (reinterpret_cast<char*>(data));
                    cout<<*ptrChar;
                    //cadena += caracter;
                }
                cout<<"|";
            }
        }
        cout<<"\n";
    }

    delete[] data;
    return 0;
}
*/
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main(){
    int i;
    string s;
    double d;

    ifstream file("disk/bloque1.bin", std::ios::binary);
    file.seekg(173*2);

    file.read(reinterpret_cast<char*>(&i), sizeof(i));
    cout<<i<<endl;
    file.read(reinterpret_cast<char*>(&i), sizeof(i));
    cout<<i<<endl;
    file.read(reinterpret_cast<char*>(&i), sizeof(i));
    cout<<i<<endl;
    string temp = "";
    char c;
    for(int i = 0; i<90; i++){
        file.read(reinterpret_cast<char*>(&c), sizeof(c));
        temp += c;
    }
    cout<<temp;

    // for (auto& i : palabra){
    //     salida.write(reinterpret_cast<char*>(&i), sizeof(char));
    // }

    // file.read(reinterpret_cast<char>(&primerEliminado), sizeof(primerEliminado));
    
    file.close();
    // file.read(reinterpret_cast<char*>(buffer2), 10);
}
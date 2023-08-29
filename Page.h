#pragma once
#include <cstring>

class Page{
private:
    int valPage_AlInicio = 0; // asigna en tiempo de compilacion 
public:
    char * data;
    int sizePage;
    
    friend class BufferManager;

    Page() = default;
    Page(int sizePage){
        this->sizePage = sizePage;
        this->data = new char[this->sizePage]; 
        ResetMemory();
    }

    void ResetMemory() { 
        memset(data, valPage_AlInicio, sizePage); // (donde,que,cuanto) funciona a nivel de bytes
    }

    ~Page(){}
};

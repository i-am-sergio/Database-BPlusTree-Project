#pragma once
#include <iostream>

namespace lineas {
    std::string linea50(50, '-');
    std::string linea100(100, '-');
    std::string linea200(200, '-');
    
    std::string drawLinea(int size){
        std::string linea(size, '-');
        return linea;
    }

}

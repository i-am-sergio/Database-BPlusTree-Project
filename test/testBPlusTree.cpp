#include <iostream>
#include "../BPlusTree.h"
using namespace std;


int main() {
    int a = 4;
    BPlusTree tree(a);

    int opcion;
    while (true) {
        cout << "Menu:" << endl;
        cout << "1. Insertar elemento" << endl; // inserta al arbol y llama funcion insertar del diskcontroller
        cout << "2. Eliminar elemento" << endl; // cuando se llame a eliminar de diskcontroller luego debe llamar btree.remove(indice)
        cout << "3. Buscar elemento" << endl; // debe mostrar indice, bloque y ptr(ubicacion) // tambien deberiamos mostrar sector en disco
        cout << "4. Print Arbol" << endl;
        cout << "5. Insertar 891 registros" << endl;
        cout << "6. Salir" << endl;
        cout << "Selecciona una opcion: ";
        cin >> opcion;

        if (opcion == 1) {
            int elemento;
            cout << "Ingresa el elemento a insertar: ";
            cin >> elemento;
            tree.set(elemento, elemento*10,elemento*100);
            cout << "Elemento insertado correctamente." << endl;
            tree.print();
            cout<<endl;
        } else if (opcion == 2) {
            int indice;
            cout << "Ingresa el indice del elemento a eliminar: ";
            cin >> indice;
            // pair<int,int> clave_6 = tree.get(indice);
            tree.remove(indice);
            cout << "Elemento eliminado correctamente." << endl;
            tree.print();
            cout<<endl;
        } else if (opcion == 3) {
            int indice;
            cout << "Ingresa el indice del elemento a buscar: ";
            cin >> indice;
            // pair<int,int> clave_6 = tree.get(indice);
            // cout<<"\tpagina -> "<<clave_6.first<<endl;
            // cout<<"\tptr -> "<<clave_6.second<<endl;
            // tree.print();
            cout<<"Recorrido para llegar al indice solicitado: "<<endl;
            tree.getCamino(indice);
            cout<<endl;
            
        } else if (opcion == 4) {
            tree.print();
        } else if (opcion == 5) {
            for (int i=1; i<=891; i++) {
                //cout << "Insertando " << i << "-------------" << endl;
                tree.set(i, i*10,i*100);
            }
            cout << "Todas los indices fueron insertados ..." << endl;
            cout << "-------------------------" << endl << endl;
        } else if (opcion == 6) {
            cout << "Saliendo del menu Arbol" << endl;
            break;
        }
    }
    return 0;
}

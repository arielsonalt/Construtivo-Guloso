#ifndef INSTANCIAS_H
#define INSTANCIAS_H
#include "../include/Distancias.h"
#include "../include/Cliente.h"
#include "../include/CT.h"
#include <vector>

class Instancias
{
    public:
        Instancias();

        void lerDistancias();
        void lerCTs();
        void lerClientes();
        void imprimirDistancias();
        void imprimirCTs();
        void imprimirClientes();

    // Alexandre: Faltam as restricoes de subareas
    // Arielson: Não entendi ???

    vector <Distancias> vetorDistancias;
    vector <Cliente> vetorClientes;
    vector <CT> vetorCTs;
};

#endif // INSTANCIAS_H

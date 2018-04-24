#ifndef INSTANCIA_H
#define INSTANCIA_H

#include "../include/Distancias.h"
#include "../include/Cliente.h"
#include "../include/CT.h"
#include <vector>

class Instancia
{
public:
    Instancia();

    void lerDistancias();
    void lerCTs();
    void lerClientes();
    void imprimirDistancias();
    void imprimirCTs();
    void imprimirClientes();

    vector <Distancias> vetorDistancias;
    vector <Cliente> vetorClientes;
    vector <CT> vetorCTs;
};

#endif // INSTANCIA_H

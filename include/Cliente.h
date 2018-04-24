#ifndef CLIENTE_H
#define CLIENTE_H

#include <string>

using namespace std;

class Cliente
{
public:
    Cliente() {};

    int clienteId;
    string nomeCliente;
    string areaCliente;
    string subAreaCliente;
    string cargoAtualCliente;
    string municipioCliente;
    int municipioIdCliente;
    string estadoCliente;
    int estadoIdCliente;
    int nivelCliente;
};

#endif // CLIENTE_H

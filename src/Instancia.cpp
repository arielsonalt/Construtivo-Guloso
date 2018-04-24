#include "../include/Instancia.h"

#include "../rapidxml-1.13/rapidxml.hpp"
#include "../rapidxml-1.13/rapidxml_print.hpp"
#include "../rapidxml-1.13/rapidxml_utils.hpp"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

namespace patch
{
    template < typename T > std::string to_string( const T& n )
    {
        std::ostringstream stm;
        stm << n;
        return stm.str();
    }
}

using namespace rapidxml;
using namespace std;

Instancia::Instancia() {
    lerClientes();
    lerCTs();
    lerDistancias();
}

void Instancia::imprimirDistancias() {
    for (unsigned int i = 0; i < vetorDistancias.size(); i++){
        cout<<"ID: "<<vetorDistancias[i].cidadeId<<endl;
        for (unsigned int j = 0; j < vetorDistancias[i].distanciasCT.size(); j++){
            float valor = vetorDistancias[i].distanciasCT[j];
            cout<<"Distancia do CT"<<j+1<<": "<<valor<<endl;

        }
    }
}

void Instancia::imprimirCTs() {
    for (unsigned int i = 0;i<vetorCTs.size();i++){
        CT ct = vetorCTs[i];
        cout <<"Id: " <<ct.ctId << endl;
        cout <<"Nome: " <<ct.nomeCidadeCT  << endl;
        cout <<"Custo: " << ct.custoCT  << endl << endl;
    }
}

void Instancia::imprimirClientes() {

    Cliente cliente;
    for (unsigned int i=0; i<vetorClientes.size();i++){
        cliente = vetorClientes[i];
        cout<<"Estado: " << cliente.estadoCliente<<endl;
        cout<<"Municipio: " <<cliente.municipioCliente<<endl;
        cout<<"Municipio ID: " <<cliente.municipioIdCliente<<endl;
        cout<<"Cargo Atual: " <<cliente.cargoAtualCliente<<endl;
        cout<<"Sub Area: " <<cliente.subAreaCliente<<endl;
        cout<<"Area: " <<cliente.areaCliente<<endl;
        cout<<"Nome: " <<cliente.nomeCliente<<endl;
        cout<<"Id: " <<cliente.clienteId<<endl;
        cout<<"\n" <<endl;
    }
}

void Instancia::lerCTs(){
    xml_document<> doc;
    xml_node<> * root_node;
    ifstream theFile ("cidadeCT.xml");
    vector<char> buffer((istreambuf_iterator<char>(theFile)), istreambuf_iterator<char>());
    buffer.push_back('\0');
    doc.parse<0>(&buffer[0]);
    root_node = doc.first_node("data-set");

    for (xml_node<> * record_node = root_node->first_node("record");
        record_node;
        record_node = record_node->next_sibling())
    {
        CT ct;
        xml_node<> * id_node = record_node->first_node("id");
        xml_node<> * nome_node = record_node->first_node("nome");
        xml_node<> * custo_node = record_node->first_node("custo");

        ct.ctId = atoi(id_node->value());
        ct.nomeCidadeCT = nome_node->value();
        ct.custoCT = atoi(custo_node->value());

        vetorCTs.push_back(ct);
    }
}

void Instancia::lerDistancias(){
    xml_document<> doc;
	xml_node<> * root_node;
	ifstream theFile ("distancias_ctsn.xml");
	vector<char> buffer((istreambuf_iterator<char>(theFile)), istreambuf_iterator<char>());
	buffer.push_back('\0');
	doc.parse<0>(&buffer[0]);
	root_node = doc.first_node("distancias");

	for (xml_node<> * cidade_node = root_node->first_node("cidade");
        cidade_node;
        cidade_node = cidade_node->next_sibling()){
        Distancias tmp;
        xml_node<> * id_node = cidade_node->first_node("id");
        tmp.cidadeId = atoi(id_node->value());
        xml_node<> * distancias_cts_node = cidade_node->first_node("distancias-cts");
        xml_node<> * ct_node;

        for (unsigned int i=1; i<=52; i++) {
            std::string ct_tag = "ct" + patch::to_string(i);
            ct_node = distancias_cts_node->first_node(ct_tag.c_str());
            tmp.distanciasCT.push_back(atof(ct_node->value()));
        }
        vetorDistancias.push_back(tmp);
    }
}

void Instancia::lerClientes(){
    xml_document<> doc;
	xml_node<> * root_node;
	ifstream theFile ("intancia15.xml");
	vector<char> buffer((istreambuf_iterator<char>(theFile)), istreambuf_iterator<char>());
	buffer.push_back('\0');
	doc.parse<0>(&buffer[0]);
	root_node = doc.first_node("data-set");

    for (xml_node<> * record_node = root_node->first_node("record");
        record_node;
        record_node = record_node->next_sibling())
    {
        Cliente cliente;
        xml_node<> * id_node = record_node->first_node("id");
        cliente.clienteId = atoi(id_node->value());
        xml_node<> * nome_node = record_node->first_node("nome");
        cliente.nomeCliente = nome_node->value();
        xml_node<> * area_node = record_node->first_node("area");
        cliente.areaCliente = area_node->value();
        xml_node<> * subArea_node = record_node->first_node("subArea");
        cliente.subAreaCliente = subArea_node->value();
        xml_node<> * cargoAtual_node = record_node->first_node("cargoAtual");
        cliente.cargoAtualCliente = cargoAtual_node->value();
        xml_node<> * municipio_node = record_node->first_node("municipio");
        cliente.municipioCliente = municipio_node->value();
        xml_node<> * municipioId_node = record_node->first_node("municipioId");
        cliente.municipioIdCliente = atoi(municipioId_node->value());
        xml_node<> * estado_node = record_node->first_node("estado");
        cliente.estadoCliente = estado_node->value();
        xml_node<> * nivel_node = record_node->first_node("nivel");
        cliente.nivelCliente = atoi(nivel_node->value());
        vetorClientes.push_back(cliente);
    }
}

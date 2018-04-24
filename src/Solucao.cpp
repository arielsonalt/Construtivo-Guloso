#include "../include/Solucao.h"
#include "../include/ConstrutivoGuloso.h"
#include "../rapidxml-1.13/rapidxml.hpp"
#include "../rapidxml-1.13/rapidxml_print.hpp"
#include "../rapidxml-1.13/rapidxml_utils.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <ctime>

using namespace std;

#define NUMERO_TURMAS 16

PlanejamentoCT::PlanejamentoCT(const CT & v) :
    ct(v)
{
}

Solucao::Solucao(Instancia & i) :
    instancia(i)
{
    Idturma = 0;
    custoCTs = 0;
    custoClientes = 0;
}

void Solucao::calcularCusto() {
    float custoCT = 6000;
    float custoDeslocamento = 1000;
    float custoHospedagem = 900;
    float coffeBreak = 105;

    custoCTs = 0;
    custoClientes = 0;

    for (unsigned int i = 0; i < planejamentoCTs.size(); i++) {
        for(unsigned int k=0; k<planejamentoCTs[i].turmas.size(); k++) {
            // Contabiliza o custo da turma atual
            if(instancia.vetorCTs[planejamentoCTs[i].turmas[k].turmaCtId-1].custoCT == 1) {
                custoCTs += custoCT;
            }
            // Percorre o vetor de alunos da turma atual
            for(unsigned int j=0; j<planejamentoCTs[i].turmas[k].vetorAlunos.size(); j++) {
                // Se o ct dessa turma tem custo o coffee break custa dobrado
                if( instancia.vetorCTs[planejamentoCTs[i].turmas[k].turmaCtId-1].custoCT == 0) {
                    custoClientes += coffeBreak;
                } else {
                    custoClientes += coffeBreak*2;
                }
                float currDist = instancia.vetorDistancias[planejamentoCTs[i].turmas[k].vetorAlunos[j].municipioIdCliente-1].distanciasCT[planejamentoCTs[i].turmas[k].turmaCtId-1];

                // Contabiliza o custo de hospedagem
                if (currDist >= 60) {
                    if(planejamentoCTs[i].turmas[k].vetorAlunos[j].nivelCliente == 1) {
                        custoClientes += custoHospedagem;
                    }
                    else if(planejamentoCTs[i].turmas[k].vetorAlunos[j].nivelCliente == 2) {
                        custoClientes += custoHospedagem * 1.5;
                    }
                    else if(planejamentoCTs[i].turmas[k].vetorAlunos[j].nivelCliente == 3) {
                        custoClientes += custoHospedagem * 2;
                    }

                }
                // Contabiliza o custo de transporte
                if(currDist >= 300) {
                    if(planejamentoCTs[i].turmas[k].vetorAlunos[j].nivelCliente == 2) {
                        custoClientes += custoDeslocamento * 1.5;
                    }
                    else {
                        custoClientes += custoDeslocamento;
                    }
                }
            }
        }

    }
}

void Solucao::criarTurma() {
int ctIndice, quant;

        for(int i = 0;i < planejamentoCTs.size();i++){
            if(i==0 && planejamentoCTs[i].turmas.size() < NUMERO_TURMAS ){
                quant=planejamentoCTs[i].turmas.size();
                ctIndice=i;
            }
            if(planejamentoCTs[i].turmas.size() < quant  && planejamentoCTs[i].turmas.size() < NUMERO_TURMAS){
                quant = planejamentoCTs[i].turmas.size();
                ctIndice=i;
            }
        }

    Turma t;
    Idturma++;
    t.idTurma = Idturma;
    t.turmaCtId = ctIndice;
    planejamentoCTs[ctIndice].turmas.push_back(t);

    Solucao so(instancia);
   ConstrutivoGuloso cg(so);
   cg.realocar();
cout<<"pronto"<<endl;
}

void Solucao::criarTurma(int k, int i) {
    Turma t;
    Idturma++;
    t.idTurma = Idturma;
    t.turmaCtId = k+1;
    t.vetorAlunos.push_back(instancia.vetorClientes[i]);
    planejamentoCTs[k].turmas.push_back(t);
}

void Solucao::inserirAluno(int k,int i,int l) {
    planejamentoCTs[k].turmas[l].vetorAlunos.push_back(instancia.vetorClientes[i]);
}

// Tenta alocar o cliente i a uma turma do CT k
bool Solucao::alocarCliente(int k, int i) {
    bool clienteAlocado = false;
    bool alocar;
    int idCidade;
    string subArea;

    // Percorre as turmas do CT k
    for(unsigned int l=0; l<planejamentoCTs[k].turmas.size() && !clienteAlocado; l++) {
        // Se a turma atual não está cheia...
        if(planejamentoCTs[k].turmas[l].vetorAlunos.size() < 20) {
            subArea = instancia.vetorClientes[i].subAreaCliente;
            idCidade = instancia.vetorClientes[i].municipioIdCliente;
            alocar = true;
            // Verifica se a alocacao deste cliente nesta turma violará a restricao de SubArea
            for (unsigned int r=0; r<planejamentoCTs[k].turmas[l].vetorAlunos.size(); r++) {
                if (planejamentoCTs[k].turmas[l].vetorAlunos[r].subAreaCliente == subArea
                    && planejamentoCTs[k].turmas[l].vetorAlunos[r].municipioIdCliente == idCidade) {
                    alocar = false;
                }
            }
            // Se nao ha violacao de subArea, insere o aluno nesta turma
            if (alocar) {
                inserirAluno(k,i,l);
                clienteAlocado = true;
            }
        }
    }
    // Se o cliente ainda não foi alocado e o numero de turmas neste CT é menor que o máximo criar mais uma turma
    if (!clienteAlocado && (planejamentoCTs[k].turmas.size() < NUMERO_TURMAS)) {
        criarTurma(k,i);
        clienteAlocado = true;
    }
    return clienteAlocado;
}

void Solucao::gerarXMLSolucao(){
    ofstream file;
    file.open("solucaoXML.xml");

    file<<"<?xml version='1.0' encoding='UTF-8' standalone='yes'?>\n";
    file<<"    <Solucao>\n";

    for(unsigned int i=0; i < planejamentoCTs.size(); i++) {
        for(unsigned int j=0; j <planejamentoCTs[i].turmas.size(); j++) {
            for(unsigned int k=0; k <planejamentoCTs[i].turmas[j].vetorAlunos.size(); k++) {
                file<<"        <cliente>\n";
                file<<"            <idCliente>"<<planejamentoCTs[i].turmas[j].vetorAlunos[k].clienteId<<"</idCliente>\n";
                file<<"            <nome>"<<planejamentoCTs[i].turmas[j].vetorAlunos[k].nomeCliente<<"</nome>\n";
                file<<"            <idCt>"<<planejamentoCTs[i].ct.ctId<<"</idCt>\n";
                file<<"            <cidade>"<<planejamentoCTs[i].ct.nomeCidadeCT<<"</cidade>\n";
                file<<"        </cliente>\n";
            }
        }
    }
    file<<"</Solucao>";
    file.close();
}

int Solucao::viabilidade(){
    int cont=0,idCidadeCliente;
    string subAreaCliente;
    vector <Cliente> pesquisaCliente;

    for(unsigned int m=0; m < instancia.vetorClientes.size(); m++) {
        pesquisaCliente.push_back(instancia.vetorClientes[m]);
    }

    for (unsigned int i=0; i < planejamentoCTs.size(); i++) {

        for (unsigned int j=0; j <planejamentoCTs[i].turmas.size(); j++) {
            if ((planejamentoCTs[i].turmas[j].vetorAlunos.size() < 10) ||
                (planejamentoCTs[i].turmas[j].vetorAlunos.size() > 20)) {
                return 1;
            }
            for(unsigned int k=0; k <planejamentoCTs[i].turmas[j].vetorAlunos.size(); k++) {
                subAreaCliente = planejamentoCTs[i].turmas[j].vetorAlunos[k].subAreaCliente;
                idCidadeCliente = planejamentoCTs[i].turmas[j].vetorAlunos[k].municipioIdCliente;
                cont=0;
                for(unsigned int l=k+1; l < planejamentoCTs[i].turmas[j].vetorAlunos.size(); l++) {
                    if ((subAreaCliente == planejamentoCTs[i].turmas[j].vetorAlunos[l].subAreaCliente)
                        && (idCidadeCliente == planejamentoCTs[i].turmas[j].vetorAlunos[l].municipioIdCliente)){
                        cont++;
                    }
                }
                if (cont != 0){
                    return 2;
                }
                for(unsigned int n=0; n < pesquisaCliente.size(); n++){
                    if (planejamentoCTs[i].turmas[j].vetorAlunos[k].clienteId == pesquisaCliente[n].clienteId){
                        pesquisaCliente.erase(pesquisaCliente.begin()+n);
                    }
                }
            }
        }
    }
    if (pesquisaCliente.size() != 0){
        return 3;
    }
    return 0;
}

void Solucao::imprimir() {
    int cont=0;
    for(unsigned int i=0; i < planejamentoCTs.size(); i++) {
        cout<<"***********************************************"<<endl;
        cout<<"CT: "<<planejamentoCTs[i].ct.nomeCidadeCT<<endl;
        cout<<"ID: "<<planejamentoCTs[i].ct.ctId<<endl;
        cout<<"CUSTO: "<<planejamentoCTs[i].ct.custoCT<<endl;
        cout<<"QUANTIDADE DE TURMAS: "<<planejamentoCTs[i].turmas.size()<<endl;
        for(unsigned int j=0; j <planejamentoCTs[i].turmas.size(); j++) {
            cout<<"--------------------------------------------------"<<endl;
            cout<<"TURMA ID: "<<planejamentoCTs[i].turmas[j].idTurma<<endl;
            cout<<"QUANTIDADE DE ALUNOS: "<<planejamentoCTs[i].turmas[j].vetorAlunos.size()<<endl;
            cout<<"\n|ALUNOS|                   \n"<<endl;
            for(unsigned int k=0; k <planejamentoCTs[i].turmas[j].vetorAlunos.size(); k++) {
                cout<<"NOME: "<<planejamentoCTs[i].turmas[j].vetorAlunos[k].nomeCliente
                    <<"     CIDADE DO CLIENTE: "
                    <<planejamentoCTs[i].turmas[j].vetorAlunos[k].municipioCliente
                    <<"     SUB-AREA: "
                    <<planejamentoCTs[i].turmas[j].vetorAlunos[k].subAreaCliente<<endl;
                    cont++;
            }
        }
    }
    cout<<"Alunos Treinados: "<<cont<<endl;
}

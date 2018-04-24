#include "ConstrutivoGuloso.h"
#include <algorithm>
#include <string>
#include <iostream>
#include <ctime>

ConstrutivoGuloso::ConstrutivoGuloso(Solucao & s) :
    solucao(s)
{

}

void ConstrutivoGuloso::algoritmo(){
    bool clienteAlocado;

    // Aqui o vetor planejamentoCTs é inicializado
    for(unsigned int x=0; x<solucao.instancia.vetorCTs.size(); x++) {
        PlanejamentoCT molde(solucao.instancia.vetorCTs[x]);
        solucao.planejamentoCTs.push_back(molde);
    }
    // Aleatoriedade de clientes
    srand(unsigned(time(0)));
    random_shuffle(solucao.instancia.vetorClientes.begin(), solucao.instancia.vetorClientes.end());

    // Percorre o vetor de clientes alocando um a um a alguma turma com vaga
    for(unsigned int i=0; i<solucao.instancia.vetorClientes.size(); i++) {
        clienteAlocado = false;

        // Tenta alocar o cliente a alguma turma a menos de 60km
        for(unsigned int k=0;
            !clienteAlocado &&
            k < solucao.instancia.vetorDistancias[solucao.instancia.vetorClientes[i].municipioIdCliente-1].distanciasCT.size();
            k++) {
            if(solucao.instancia.vetorDistancias[solucao.instancia.vetorClientes[i].municipioIdCliente-1].distanciasCT[k] < 60) {
                if (solucao.alocarCliente(k,i)) {
                    clienteAlocado = true;
                }
            }
        }
        // Se ainda não foi alocado, tenta alocar o cliente a alguma turma a menos de 300km
        if (!clienteAlocado) {
            for(unsigned int k=0;
                !clienteAlocado &&
                k<solucao.instancia.vetorDistancias[solucao.instancia.vetorClientes[i].municipioIdCliente-1].distanciasCT.size();
                k++) {
                if(solucao.instancia.vetorDistancias[solucao.instancia.vetorClientes[i].municipioIdCliente-1].distanciasCT[k] >= 60
                    && solucao.instancia.vetorDistancias[solucao.instancia.vetorClientes[i].municipioIdCliente-1].distanciasCT[k] < 300) {
                    if (solucao.alocarCliente(k,i)) {
                        clienteAlocado = true;
                    }

                }
            }
        }
        // Se ainda não foi alocado, tenta alocar o cliente a alguma turma a qualquer distancia
        if (!clienteAlocado) {
            for(unsigned int k=0;
                !clienteAlocado &&
                k<solucao.instancia.vetorDistancias[solucao.instancia.vetorClientes[i].municipioIdCliente-1].distanciasCT.size();
                k++) {
                if(solucao.instancia.vetorDistancias[solucao.instancia.vetorClientes[i].municipioIdCliente-1].distanciasCT[k] >= 300) {
                    if (solucao.alocarCliente(k,i)) {
                        clienteAlocado = true;
                    }
                }
            }
        }
    }
    realocar();
}

void ConstrutivoGuloso::realocar() {
    bool alocado = false;
    bool alocar;
    bool chave = false;
    bool chave2;
    int idCidade;
    string subArea;

    // Enquanto existir (turma <=5) ou (turma > 5 && turma < 10)
    while (chave == false) {
        chave2 = false;
        // Para todo ct
        for (unsigned int a=0; a < solucao.planejamentoCTs.size(); a++) {

            // Para toda turma no ct
            for (unsigned int b=0; b<solucao.planejamentoCTs[a].turmas.size(); b++) {

                // Se turma menor ou igual a 5
                if (solucao.planejamentoCTs[a].turmas[b].vetorAlunos.size() <= 5) {
                    chave2=true;

                    //para todo aluno desta turma
                    for (unsigned int c=0; c<solucao.planejamentoCTs[a].turmas[b].vetorAlunos.size(); c++) {

                        //para todo ct
                        for (unsigned int d=0; d<solucao.planejamentoCTs.size(); d++) {
                            alocado = false;
                            // para toda turma
                            for (unsigned int e=0; e<solucao.planejamentoCTs[d].turmas.size(); e++) {
                                //se tamanho da turma >=10 && turma < 20
                                if (solucao.planejamentoCTs[d].turmas[e].vetorAlunos.size() >= 10
                                    && solucao.planejamentoCTs[d].turmas[e].vetorAlunos.size() < 20) {

                                    idCidade = solucao.planejamentoCTs[a].turmas[b].vetorAlunos[c].municipioIdCliente;
                                    subArea = solucao.planejamentoCTs[a].turmas[b].vetorAlunos[c].subAreaCliente;
                                    alocar = true;
                                    // garante que não exista cliente de mesma sub-area na turma
                                    for (unsigned int f=0; f<solucao.planejamentoCTs[d].turmas[e].vetorAlunos.size(); f++) {
                                        if(solucao.planejamentoCTs[d].turmas[e].vetorAlunos[f].municipioIdCliente == idCidade
                                                && solucao.planejamentoCTs[d].turmas[e].vetorAlunos[f].subAreaCliente == subArea) {
                                            alocar = false;
                                        }
                                    }
                                    if (alocar) {
                                        //insere o aluno na turma encontrada
                                        solucao.planejamentoCTs[d].turmas[e].vetorAlunos.push_back(solucao.planejamentoCTs[a].turmas[b].vetorAlunos[c]);

                                        //Apaga cliente da origem
                                        solucao.planejamentoCTs[a].turmas[b].vetorAlunos.erase(solucao.planejamentoCTs[a].turmas[b].vetorAlunos.begin()+c);

                                        alocado = true;
                                        break;
                                    }
                                }
                            }
                            if(alocado==true) {
                                break;
                            }
                        }
                    }
                    //se a turma de quantidade <=5 estuver vazia ela será apagada
                    if (alocado && solucao.planejamentoCTs[a].turmas[b].vetorAlunos.size() == 0) {
                        solucao.planejamentoCTs[a].turmas.erase(solucao.planejamentoCTs[a].turmas.begin()+b);
                        break;
                    }
                }
                // se turma > 5 && turma <10
                if (solucao.planejamentoCTs[a].turmas[b].vetorAlunos.size() > 5
                    && solucao.planejamentoCTs[a].turmas[b].vetorAlunos.size() < 10) {
                    //para todo aluno
                    chave2=true;
                    for(unsigned int c=0; c<solucao.planejamentoCTs[a].turmas[b].vetorAlunos.size(); c++) {

                        //para todo ct
                        for(unsigned int d=0; d<solucao.planejamentoCTs.size(); d++) {
                            alocado = false;

                            // para toda turma
                            for(unsigned int e=0; e<solucao.planejamentoCTs[d].turmas.size(); e++) {
                                //se a Turma procurada for >10 e <=20
                                if(solucao.planejamentoCTs[d].turmas[e].vetorAlunos.size()>10 && solucao.planejamentoCTs[d].turmas[e].vetorAlunos.size()<=20) {

                                    idCidade = solucao.planejamentoCTs[d].turmas[e].vetorAlunos[solucao.planejamentoCTs[d].turmas[e].vetorAlunos.size()-1].municipioIdCliente;
                                    subArea = solucao.planejamentoCTs[d].turmas[e].vetorAlunos[solucao.planejamentoCTs[d].turmas[e].vetorAlunos.size()-1].subAreaCliente;
                                    alocado = true;
                                    // garante que não exista cliente de mesma sub-area na turma
                                    for(unsigned int f=0; f<solucao.planejamentoCTs[a].turmas[b].vetorAlunos.size(); f++) {
                                        if(solucao.planejamentoCTs[a].turmas[b].vetorAlunos[f].municipioIdCliente == idCidade
                                                && solucao.planejamentoCTs[a].turmas[b].vetorAlunos[f].subAreaCliente == subArea) {
                                            alocado=false;
                                        }
                                    }
                                    if(alocado==true) {
                                        // retira um cliente
                                        solucao.planejamentoCTs[a].turmas[b].vetorAlunos.push_back(solucao.planejamentoCTs[d].turmas[e].vetorAlunos[solucao.planejamentoCTs[d].turmas[e].vetorAlunos.size()-1]);
                                        //Apaga cliente da origem
                                        solucao.planejamentoCTs[d].turmas[e].vetorAlunos.erase(solucao.planejamentoCTs[d].turmas[e].vetorAlunos.begin()+(solucao.planejamentoCTs[d].turmas[e].vetorAlunos.size()-1));
                                        break;
                                    }

                                }
                            }

                            if(alocado==true) {
                                break;
                            }

                        }
                        if(alocado==true) {
                            break;
                        }
                    }

                    if(alocado==true) {
                        break;
                    }
                }
            }
        }
        if(chave2==false) {
            chave=true;
        }
    }
}

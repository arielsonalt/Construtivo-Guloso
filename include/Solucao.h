#ifndef SOLUCAO_H
#define SOLUCAO_H

#include "../include/Instancia.h"
#include "../include/Turma.h"
#include "../include/CT.h"
#include <vector>

using namespace std;

class PlanejamentoCT {
public:
    PlanejamentoCT(const CT & v);
    CT ct;
    vector <Turma> turmas;
};

class Solucao {
public:
    Solucao(Instancia & i);
    void calcularCusto();
    void gerarXMLSolucao();
    void imprimir();
    int viabilidade();
    void criarTurma(int k,int i);
    void criarTurma();
    void inserirAluno(int k, int i, int l);
    bool alocarCliente(int k, int i);

    Instancia & instancia;
    vector <PlanejamentoCT> planejamentoCTs;
    int Idturma;
    float custoCTs;
    float custoClientes;
};

#endif // SOLUCAO_H

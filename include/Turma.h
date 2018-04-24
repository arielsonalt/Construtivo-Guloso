#ifndef TURMA_H
#define TURMA_H

#include "../include/Cliente.h"
#include <vector>

using namespace std;

class Turma
{
public:
    Turma() {};

    int idTurma;
    int turmaCtId;
    vector <Cliente> vetorAlunos;
};

#endif // TURMA_H

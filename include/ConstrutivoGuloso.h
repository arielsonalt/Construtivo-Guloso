#ifndef CONSTRUTIVOGULOSO_H
#define CONSTRUTIVOGULOSO_H

#include "../include/Solucao.h"

class ConstrutivoGuloso
{
    public:
        ConstrutivoGuloso(Solucao & s);
        void algoritmo();
        void realocar();

        Solucao & solucao;
};

#endif // CONSTRUTIVOGULOSO_H

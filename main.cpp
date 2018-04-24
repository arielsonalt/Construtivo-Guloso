#include "include/Instancia.h"
#include "include/Solucao.h"
#include "include/ConstrutivoGuloso.h"
#include <iostream>

using namespace std;

int main()
{
    Instancia instancia;
    float melhor = 99999999;
    float media = 0;
    float custo;
    const int numExecucoes = 10;

    for (int i = 1; i <= numExecucoes; i++) {
        Solucao so(instancia);
        ConstrutivoGuloso cg(so);
        cg.algoritmo();
        so.calcularCusto();
        //so.imprimir();

        custo = so.custoCTs + so.custoClientes;
        media += custo;

        if (custo < melhor){
            melhor = custo;
             so.gerarXMLSolucao();
             cout<<melhor<<endl;
        }

        int codigoErro = so.viabilidade();
        if (codigoErro != 0) {
            cout << endl << "INViavel " << codigoErro << endl;
        }
    }
    cout << endl << "Melhor: " << melhor << " Media: " << media/numExecucoes << endl;
}

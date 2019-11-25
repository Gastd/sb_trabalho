/**
 * Unversidade de Brasilia
 * Instituto de Ciências Exatas
 * Departamento de Ciencia da Computaçao
 * Software Básico - 2/2019
 * 
 * Trabalho I - Simulador de Assembly Inventado
 * Porfessor: Bruno Macciavelo
 * @author Gabriel F P Araujo
 * @author Gustavo Costa Crispim De Souza
 */

#include <iostream>
#include <string>
#include "simulator.h"

int main(int argc, char *argv[])
{
    bool dump_reg = false;
    std::string text_path("");
    if (argc < 2)
    {
        // std::cout << "Please provide exactly two files (text.bin and data.bin)" << std::endl;
        std::cout << "Please provide the obj file (<your_mounted_code>.obj)" << std::endl;
        return 0;
    }
    else
    {
        text_path = argv[1];
    }

    SBASM sbasm;

    sbasm.loadMemory(text_path);

    sbasm.run();
    if(dump_reg)
        sbasm.dump_reg('h');

    return 0;
}

/**
 * Unversidade de Brasilia
 * Instituto de Ciências Exatas
 * Departamento de Ciencia da Computaçao
 * Software Básico - 2/2019
 * 
 * Trabalho I - Montador de Assembly Inventado
 * Porfessor: Bruno Macciavelo
 * @author Gustavo Costa Crispim De Souza
 * @author Gabriel F P Araujo
 */

#include <iostream>
#include <string>
#include "assembler.h"

int main(int argc, char* argv[])
{
    // DeclaraÃ§Ã£o de VariÃ¡veis
    std::string Nome_Arquivo_Entrada;                    // arquivo de entrada .asm
    std::string Nome_Original;
    

    // if (argc != 2)
    // {
    //     std::cerr << "\n \n Numero de argumentos invalido..." << std::endl;
    //     exit(1);
    // }
    
    // Argumentos de Entrada
    Nome_Original = "bin";
    // Nome_Original = "triangulo";
    Nome_Arquivo_Entrada = Nome_Original + ".asm";              

    Assembler assembler;

    assembler.mount(Nome_Arquivo_Entrada, Nome_Original);

    return 0;
  
}

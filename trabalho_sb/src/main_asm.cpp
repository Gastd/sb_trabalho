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
    
    if (argc != 2)
    {
        std::cerr << "\n \n Numero de argumentos invalido..." << std::endl;
        exit(1);
    }
    
    // Argumentos de Entrada
    // Nome_Original = "triangulo";
    Nome_Arquivo_Entrada = argv[1];              
    size_t dot_asm = Nome_Arquivo_Entrada.find_first_of(".asm");
    if(dot_asm != std::string::npos)
    {
        Nome_Original = Nome_Arquivo_Entrada.substr(0, dot_asm);                  //Retirando a vírgula dos argumentos que tem, como o COPY
    }   
    // Nome_Original = argv[1];

    Assembler assembler;

    assembler.mount(Nome_Arquivo_Entrada, Nome_Original);

    return 0;
}

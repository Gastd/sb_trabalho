# SB's Simulator

Unversidade de Brasilia
Instituto de Ciências Exatas
Departamento de Ciencia da Computaçao
Software Básico - 2/2019
Trabalho II - Simulador, Montador e Ligador de Assembly Inventado
Porfessor: Bruno Macciavelo
Alunos: Gabriel Frutuoso Pereira Araujo
        Gustavo Costa Crispim de Sousa
Matrículas: 12/0050943
            14/0142568
Turma: A

# Montador e Ligador

## Compilacao

É fornecido um arquivo Makefile para auxiliar na comiplação do trabalho.
Para compilar digite no terminal na raiz do diretorio do trabalho:

    $ make

O compilador usado neste trabalho foi o GCC versão 7.0.4 utilizando C++11. O sistema operacional foi o Ubuntu 18.04 64 bits. Não foi utilizado nenhuma IDE neste trabalho, apenas um editor de texto (Sublime 3).

## Uso
### Passo 1
Para usar o montador com apenas um unici codigo fonte

    $ ./montador test.asm

Para usar o montador com dois modulos (dois codigos fontes)

    $ ./montador test_mod_a.asm test_mod_b.asm

A saida do montador eh o <nome_do_arquivo>.obj

### Passo 2
Para usar o ligador com apenas um unici codigo fonte

    $ ./ligador test.asm.obj
ou
    $ ./ligador test.obj

Para usar o ligador com dois modulos (dois codigos fontes)

    $ ./ligador test_mod_a.asm test_mod_b.asm
ou
    $ ./ligador test_mod_a.obj test_mod_b.obj

A saida do ligador sempre eh a.out

# Obs
A pasta testes/ contem os arquivos de testes utilizados para testar o ligador e o montador.
O simulador feito para o Trabalho 1 esta dentro da pasta tab1-sim que tem um Makefile proprio para a construcao do simulador.
A pasta calc/ tem a calculadora em IA-32 pedida, nela tem um script (compile.sh) que usa o nasm para compilar o .asm no
ambiente Linux. 

# Autores
Gabriel Frutuoso Pereira Araujo - Matricula 12/0050943
Gustavo Costa Crispim de Sousa - Matricula 14/0142568

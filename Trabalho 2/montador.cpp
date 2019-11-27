#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cstddef>
#include <vector>
#include <sstream>
#include <stdlib.h>
#include <iomanip>
#include <string.h>

using namespace std;

string noComments(string linha)
{
	size_t Achado = linha.find_first_of("#");
	if(Achado != string::npos)
	{
		linha = linha.substr(0, Achado);
		return(linha);
	}
	else
	{
		return(linha);
	}
}


string Maiusculo(string linha)
{
	int restante, i = 0;

	restante = linha.size();				//No in�cio falta a linha toda
	while (restante>0)
	{
		linha[i] = toupper(linha[i]);		//Passa caracter por caracter
		i++;
		restante = linha.size() - i;		//Verifica se j� terminou o programa
	}
	return(linha);
}

void process1(const string& Nome_Arquivo_Entrada_1, const string& Nome_1)
{
  vector<string> Cod;

  string linha;

  ifstream Codigo(Nome_Arquivo_Entrada_1);
  if(Codigo.is_open())
  {
    while(getline(Codigo,linha))
    {

      linha = noComments(linha);
			linha = Maiusculo(linha);
      Cod.push_back(linha);
      size_t begin = linha.find("BEGIN");
      size_t end = linha.find("END");

      if (begin != string::npos){
        cout << "Erro em programa " << Nome_1 << ": possui início de Módulo BEGIN..." << endl;
        exit(1);
      }

      else if (end != string::npos){
        cout << "Erro em programa " << Nome_1 << ": possui fim de Módulo END..." << endl;
        exit(1);
      }
    }
    cout << "Programa não possui diretivas de Módulos! Continuando..." << endl;
  }


  Codigo.close();
}

void process2(const string& Nome_Arquivo_Entrada_1, const string& Nome_Arquivo_Entrada_2, const string& Nome_1, const string& Nome_2)
{

  vector<string> Cod1;
  vector<string> Cod2;

  string linha1;
  string linha2;


  bool begin = false;
  bool end = false;

  ifstream Codigo(Nome_Arquivo_Entrada_1);

  if(Codigo.is_open())
  {
    while(getline(Codigo,linha1))
    {

      linha1 = noComments(linha1);
			linha1 = Maiusculo(linha1);
      Cod1.push_back(linha1);
      size_t begin_find = linha1.find("BEGIN");
      size_t end_find = linha1.find("END");

      if (begin_find != string::npos){

        if (end == true){
          cout << "Erro programa " << Nome_1 << ": END se encontra antes do BEGIN..." << endl;
          exit(1);
        }

        else
        {

          if (begin == false){
            cout << "Programa " << Nome_1 << ": achou Módulo BEGIN..." << endl;
            begin = true;
          }

          else{
            cout << "Erro programa " << Nome_1 << ": possui mais de um Módulo BEGIN..." << endl;
            exit(1);
          }

        }
      }

      else if (end_find != string::npos){

        if (end == false){
          cout << "Programa " << Nome_1 << ": achou Módulo END..." << endl;
          end = true;
        }

        else{
          cout << "Erro programa " << Nome_1 << ": possui mais de um Módulo END..." << endl;
          exit(1);
        }

      }

    }

    if (begin == false || end == false){
      cout << "Erro programa " << Nome_1 << ": não possui diretivas de Módulos..." << endl;
      exit(1);
    }

    else{
      begin = false;
      end = false;
    }

  }

  Codigo.close();

  ifstream Codigo2(Nome_Arquivo_Entrada_2);

  if(Codigo2.is_open())
  {
    while(getline(Codigo2,linha2))
    {

      linha2 = noComments(linha2);
			linha2 = Maiusculo(linha2);
      Cod2.push_back(linha2);
      size_t begin_find = linha2.find("BEGIN");
      size_t end_find = linha2.find("END");

      if (begin_find != string::npos){

        if (end == true){
          cout << "Erro programa " << Nome_1 << ": END se encontra antes do BEGIN..." << endl;
          exit(1);
        }

        else
        {
          if (begin == false){
            cout << "Programa " << Nome_1 << ": achou Módulo BEGIN..." << endl;
            begin = true;
          }

          else{
            cout << "Erro programa " << Nome_1 << ": possui mais de um Módulo BEGIN..." << endl;
            exit(1);
          }

        }
      }

      else if (end_find != string::npos){

        if (end == false){
          cout << "Programa " << Nome_1 << ": achou Módulo END..." << endl;
          begin = true;
        }

        else{
          cout << "Erro programa " << Nome_1 << ": possui mais de um Módulo END..." << endl;
          exit(1);
        }

      }
    }

    if (begin == false || end == false){
      cout << "Erro programa " << Nome_2 << ": não possui diretivas de Módulos..." << endl;
      exit(1);
    }

  }

  Codigo.close();

}

int main(int argc, char* argv[])
{
    // Declaração de Variáveis
    string Nome_Arquivo_Entrada_1;
    string Nome_Arquivo_Entrada_2;                    // arquivo de entrada .asm
    string Nome_1;
    string Nome_2;

    if (argc == 2)
    {
        cerr << "\n \n Um programa inserido..." << endl;
        Nome_1 = argv[1];
        Nome_Arquivo_Entrada_1 = Nome_1 + ".asm";
        cout << "opening " << Nome_Arquivo_Entrada_1 << endl;
        process1(Nome_Arquivo_Entrada_1, Nome_1);
    }

    else if (argc == 3)
    {
        cerr << "\n \n Dois programas inseridos..." << endl;
        Nome_1 = argv[1];
        Nome_2 = argv[2];
      	Nome_Arquivo_Entrada_1 = Nome_1 + ".asm";
        Nome_Arquivo_Entrada_2 = Nome_2 + ".asm";

      	cout << "opening " << Nome_Arquivo_Entrada_1 << endl;
        cout << "opening " << Nome_Arquivo_Entrada_2 << endl;

      	process2(Nome_Arquivo_Entrada_1, Nome_Arquivo_Entrada_2, Nome_1, Nome_2);
    }

    else
    {
        cout << "\n \n Numero de argumentos invalido..." << endl;
        exit(1);
    }

    return 0;

}

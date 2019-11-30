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

void process(const string& Nome_Arquivo_Entrada_1, const string& Nome_1)
{
  string linha;
  vector <string> Cod;
  int unsigned i;

  ifstream Codigo(Nome_Arquivo_Entrada_1);

  if(Codigo.is_open())
  {
    while(getline(Codigo,linha))
    {
      if (linha.find("Tabela de Uso") != string::npos){
        cout<<"Erro Código objeto "<<Nome_1<<": programa possui endereços de memória de outro programa não listado. Encerrando..."<<endl;
        exit(1);
      }
      else{
        size_t Achado = linha.find("T: ");
        if (Achado!=string::npos){
          linha.erase(linha.begin());
          linha.erase(linha.begin());
          linha.erase(linha.begin());
          Cod.push_back(linha);
        }
      }
    }
  }
  Codigo.close();

  ofstream fout;

  fout.open(Nome_1 + "_final.obj");

  for(i=0;i<Cod.size();i++)
  {
    fout<<Cod[i];
  }
}

void process2(const string& Nome_Arquivo_Entrada_1, const string& Nome_1, const string& Nome_Arquivo_Entrada_2, const string& Nome_2)
{
  string linha;

  vector <string> Cod1;
  vector <string> Cod2;

  int unsigned i = 0;
  int cod1_size=0;
  int cod2_size=0;

  vector <int> point_entry;

  cout << "opening " << Nome_Arquivo_Entrada_1 << endl;

  ifstream Codigo(Nome_Arquivo_Entrada_1);

  i = 0;

  if(Codigo.is_open())
  {
    while(getline(Codigo,linha))
    {
      if (i==1){
        cod1_size = linha[3];
        cout<<cod1_size<<endl;
      }
      i++;
    }
  }
  Codigo.close();

  cout << "opening " << Nome_Arquivo_Entrada_2 << endl;

  i = 0;

  ifstream Codigo2(Nome_Arquivo_Entrada_2);

  if(Codigo2.is_open())
  {
    while(getline(Codigo2,linha))
    {

    }
  }
  Codigo2.close();

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
        Nome_Arquivo_Entrada_1 = Nome_1 + ".obj";
        cout << "opening " << Nome_Arquivo_Entrada_1 << endl;
        process(Nome_Arquivo_Entrada_1, Nome_1);
    }

    else if (argc == 3)
    {
        cerr << "\n \n Dois programas inseridos..." << endl;
        Nome_1 = argv[1];
        Nome_2 = argv[2];
      	Nome_Arquivo_Entrada_1 = Nome_1 + ".obj";
        Nome_Arquivo_Entrada_2 = Nome_2 + ".obj";


      	process2(Nome_Arquivo_Entrada_1, Nome_1, Nome_Arquivo_Entrada_2, Nome_2);

    }

    else
    {
        cout << "\n \n Numero de argumentos invalido..." << endl;
        exit(1);
    }

    cout << "Programa finalizou..." << endl;
    return 0;

}

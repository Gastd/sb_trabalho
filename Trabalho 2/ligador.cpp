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
#include <map>

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

  vector<string> Cod1;
  vector<string> Cod2;
  
  vector<int> realocacao1, realocacao2;
  map<string, int> tabelaDef1, tabelaDef2;
  vector<vector<string>> tabelaUso1, tabelaUso2;
  vector<vector<string>> tgd; // Tabela Geral de Definicoes
  bool tabDef = false;
  bool tabUso = false;

  int unsigned i = 0;
  int cod1_size=0;
  int cod2_size=0;

  vector <int> point_entry;

  cout << "opening " << Nome_Arquivo_Entrada_1 << endl;

  ifstream Codigo(Nome_Arquivo_Entrada_1);

  i = 0;
  stringstream ss;
  string buf;

  if(Codigo.is_open())
  {
    while(getline(Codigo,linha))
    {
      if (linha.find("H: Tabela de definicoes") != string::npos)
      {
        tabDef = true;
        tabUso = false;
        continue;
      }

      if (linha.find("H: Tabela de Uso") != string::npos)
      {
        tabUso = true;
        tabDef = false;
        continue;
      }

      if (linha.find("T: ") != string::npos)
      {
        tabUso = false;
        tabDef = false;
      }

      if (i == 1)
      {
        ss << linha;
        ss >> buf;
        ss >> buf;
        cod1_size = stoi(buf);
        cout << "Tamanho do arquivo " << cod1_size << endl;
      }

      if (i == 2) // info de realocacao
      {
        ss << linha;
        ss >> buf;
        ss >> buf;
        ss >> buf;
        while (ss >> buf)
        {
          realocacao1.push_back(stoi(buf));
        }
      }

      if (tabUso)
      {
        ss << linha;
        ss >> buf;
        vector<string> vec_aux;
        while (ss >> buf)
        {
          vec_aux.push_back(buf);
        }
        tabelaUso1.push_back(vec_aux);
      }

      if (tabDef)
      {
        ss << linha;
        ss >> buf; // remove "H: "
        string key;
        ss >> key; // get key
        if (key.size() == 0)  break;
        ss >> buf; // get value
        int value = stoi(buf);
        cout << linha << endl;
        cout << "key = " << key << " value = " << value << endl;
        tabelaDef1[key] = value;
      }

      ss.clear();
      i++;
    }
  }

  cout << endl << "Tabela de Definicoes" << endl;
  for (auto it = tabelaDef1.begin(); it != tabelaDef1.end(); it++)
  {
    cout << it->first << " => " << it->second << endl;
  }
  cout << endl;
  cout << "Tabela de Uso" << endl;
  for (long unsigned int i = 0; i < tabelaUso1.size(); i++)
  {
    for (long unsigned int j = 0; j < tabelaUso1.at(i).size(); j++)
    {
      cout << tabelaUso1.at(i).at(j);
      cout << " ";
    }
    cout << endl;
  }
  cout << endl;

  Codigo.close();

  cout << "opening " << Nome_Arquivo_Entrada_2 << endl;

  i = 0;

  ifstream Codigo2(Nome_Arquivo_Entrada_2);

  if(Codigo2.is_open())
  {
    while(getline(Codigo2,linha))
    {
      if (linha.find("H: Tabela de definicoes") != string::npos)
      {
        tabDef = true;
        tabUso = false;
        continue;
      }

      if (linha.find("H: Tabela de Uso") != string::npos)
      {
        tabUso = true;
        tabDef = false;
        continue;
      }

      if (linha.find("T: ") != string::npos)
      {
        tabUso = false;
        tabDef = false;
      }

      if (i == 1)
      {
        ss << linha;
        ss >> buf;
        ss >> buf;
        cod2_size = stoi(buf);
        cout << cod2_size << endl;
      }

      if (i == 2) // info de realocacao
      {
        ss << linha;
        ss >> buf;
        ss >> buf;
        ss >> buf;
        while (ss >> buf)
        {
          realocacao2.push_back(stoi(buf));
        }
      }

      if (tabUso)
      {
        ss << linha;
        ss >> buf;
        vector<string> vec_aux;
        while (ss >> buf)
        {
          vec_aux.push_back(buf);
        }
        tabelaUso2.push_back(vec_aux);
      }

      if (tabDef)
      {
        ss << linha;
        ss >> buf; // remove "H: "
        string key;
        ss >> key; // get key
        if (key.size() == 0)  break;
        ss >> buf; // get value
        int value = stoi(buf);
        tabelaDef2[key] = value;
      }

      ss.clear();
      i++;
    }
  }

  cout << endl << "Tabela de Definicoes" << endl;
  for (auto it = tabelaDef2.begin(); it != tabelaDef2.end(); it++)
  {
    cout << it->first << " => " << it->second << endl;
  }

  cout << endl << "Tabela de Uso" << endl;
  for (long unsigned int i = 0; i < tabelaUso2.size(); i++)
  {
    for (long unsigned int j = 0; j < tabelaUso2.at(i).size(); j++)
    {
      cout << tabelaUso2.at(i).at(j);
      cout << " ";
    }
    cout << endl;
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

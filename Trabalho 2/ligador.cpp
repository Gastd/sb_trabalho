/**
 * Unversidade de Brasilia
 * Instituto de Ciências Exatas
 * Departamento de Ciencia da Computaçao
 * Software Básico - 2/2019
 * 
 * Trabalho II - Montador e Ligador de Assembly Inventado de ate Dois Modulos
 * Porfessor: Bruno Macciavelo
 * @author Gustavo Costa Crispim De Souza
 * @author Gabriel F P Araujo
 */

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cstddef>
#include <vector>
#include <sstream>
#include <stdlib.h>
#include <iomanip>
#include <cstring>
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

  fout.open("a.out");

  for(i=0;i<Cod.size();i++)
  {
    fout<<Cod[i];
  }
}

void process2(const string& Nome_Arquivo_Entrada_1, const string& Nome_1, const string& Nome_Arquivo_Entrada_2, const string& Nome_2)
{

  vector<string> Cod1;
  vector<string> Cod2;
  
  vector<int> realocacao1, realocacao2;
  vector<int> srcCode1, srcCode2;
  map<string, int> tabelaDef1, tabelaDef2;
  map<string, int> tabelaUso1, tabelaUso2;
  vector<string> rotTabelaUso1, rotTabelaUso2;
  vector<int> posTabelaUso1, posTabelaUso2;
  map<string, int> tgd; // Tabela Geral de Definicoes
  bool tabDef1 = false;
  bool tabDef2 = false;
  bool tabUso1 = false;
  bool tabUso2 = false;
  int fator_corr_A = 0;
  int fator_corr_B = 0;

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
    string linha;
    while(getline(Codigo,linha))
    {
      if (linha.find("H: Tabela de definicoes") != string::npos)
      {
        tabDef1 = true;
        tabUso1 = false;
        continue;
      }

      if (linha.find("H: Tabela de Uso") != string::npos)
      {
        tabUso1 = true;
        tabDef1 = false;
        continue;
      }

      if (i == 1)
      {
        ss << linha;
        ss >> buf;
        ss >> buf;
        cod1_size = stoi(buf);
        cout << "Tamanho do arquivo " << cod1_size << endl;
        fator_corr_B = cod1_size;
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

      if (tabUso1)
      {
        ss << linha;
        ss >> buf; // remove "H: "
        string key;
        ss >> key; // get key
        if (key.size() == 0)  break;
        ss >> buf; // get value
        int value = stoi(buf);
        tabelaUso1[key] = value;
        rotTabelaUso1.push_back(key);
        posTabelaUso1.push_back(value);
      }

      if (tabDef1)
      {
        ss << linha;
        ss >> buf; // remove "H: "
        string key;
        ss >> key; // get key
        if (key.size() == 0)  break;
        ss >> buf; // get value
        int value = stoi(buf);
        tabelaDef1[key] = value;
      }

      ss.clear();
      i++;
    }
  }

  // cout << endl << "Tabela de Definicoes 1" << endl;
  for (auto it = tabelaDef1.begin(); it != tabelaDef1.end(); it++)
  {
    // cout << it->first << " => " << it->second << endl;
    tgd[it->first] = it->second;
  }
  // cout << endl;
  // cout << "Tabela de Uso 1" << endl;
  // for (size_t i = 0; i != rotTabelaUso1.size(); i++)
  // {
  //   cout << rotTabelaUso1[i] << " => " << posTabelaUso1[i] << endl;
  // }
  // cout << endl;


  cout << "opening " << Nome_Arquivo_Entrada_2 << endl;

  i = 0;

  ifstream Codigo2(Nome_Arquivo_Entrada_2);

  if(Codigo2.is_open())
  {
    string linha;
    while(getline(Codigo2,linha))
    {
      if (linha.find("H: Tabela de definicoes") != string::npos)
      {
        tabDef2 = true;
        tabUso2 = false;
        continue;
      }

      if (linha.find("H: Tabela de Uso") != string::npos)
      {
        tabUso2 = true;
        tabDef2 = false;
        continue;
      }

      if (i == 1)
      {
        ss << linha;
        ss >> buf;
        ss >> buf;
        cod2_size = stoi(buf);
        cout << "Tamanho do arquivo " << cod2_size << endl;
        // fator_corr_A = cod2_size + 1;
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

      if (tabUso2)
      {
        ss << linha;
        ss >> buf; // remove "H: "
        string key;
        ss >> key; // get key
        if (key.size() == 0)  break;
        ss >> buf; // get value
        int value = stoi(buf);
        tabelaUso2[key] = value;
        rotTabelaUso2.push_back(key);
        posTabelaUso2.push_back(value);
      }

      if (tabDef2)
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

  // cout << endl << "Tabela de Definicoes 2" << endl;
  for (map<string,int>::iterator it = tabelaDef2.begin(); it != tabelaDef2.end(); it++)
  {
    // cout << it->first << " => " << it->second << endl;
    tgd[it->first] = it->second + fator_corr_B;
  }

  // cout << endl << "Tabela de Uso 2" << endl;
  // for (size_t i = 0; i != rotTabelaUso2.size(); i++)
  // {
  //   cout << rotTabelaUso2[i] << " => " << posTabelaUso2[i] << endl;
  // }

  // cout << endl << "Tabela Geral de Definicoes" << endl;
  // for (map<string,int>::iterator it = tgd.begin(); it != tgd.end(); it++)
  // {
  //   cout << it->first << " => " << it->second << endl;
  // }

  // Condigo fonte total
  // for (long unsigned int i = 0; i < srcCode1.size(); i++)
  // {
  //   cout << srcCode1[i] << " ";
  // }
  // for (long unsigned int i = 0; i < srcCode2.size(); i++)
  // {
  //   cout << srcCode2[i] << " ";
  // }
  // cout << endl;

  if (Codigo.is_open() && Codigo2.is_open())
  {
    string linha;

    while(getline(Codigo, linha))
    {
      ss.clear();

      if (linha.find("T: ") != string::npos)
      {
        ss << linha;
        ss >> buf;
        // cout << buf << endl;
        while (ss >> buf)
        {
          srcCode1.push_back(stoi(buf));
        }
      }
    }

    while(getline(Codigo2, linha))
    {
      ss.clear();
      
      // cout << linha << endl;
      if (linha.find("T: ") != string::npos)
      {
        ss << linha;
        ss >> buf;
        // cout << buf << endl;
        while (ss >> buf)
        {
          srcCode2.push_back(stoi(buf));
        }
      }
    }
  }

  Codigo.close();
  Codigo2.close();

  // realocacao
  // cout << "Code" << endl;
  
  // cout << endl;
  
  // cout << endl;

  // corrigindo os valores nos codigos fontes
  // cout << "MOD A antes realocacao" << endl;
  // for (size_t i = 0; i != srcCode1.size(); i++)
  // {
  //   cout << srcCode1[i] << " ";
  // }
  // cout << endl;

  for (size_t i = 0; i != rotTabelaUso1.size(); i++)
  {
    srcCode1[posTabelaUso1[i]] = tgd[rotTabelaUso1[i]];
    // cout << "trocando o valor de " << rotTabelaUso1[i] << " em " << posTabelaUso1[i] << " por " << tgd[rotTabelaUso1[i]] << endl;
  }
  for (size_t i = 0; i < realocacao1.size(); ++i)
  {
    // std::vector<int>::iterator it = std::find(posTabelaUso1.begin(), posTabelaUso1.end(), i);
    srcCode1[realocacao1[i]] += fator_corr_A;
  }
  // cout << "MOD A depois realocacao" << endl;
  // for (size_t i = 0; i != srcCode1.size(); i++)
  // {
  //   cout << srcCode1[i] << " ";
  // }
  // cout << endl;
  // cout << endl;

  // cout << "MOD B depois realocacao" << endl;
  // for (size_t i = 0; i != srcCode2.size(); i++)
  // {
  //   cout << srcCode2[i] << " ";
  // }
  // cout << endl;
  for (size_t i = 0; i != rotTabelaUso2.size(); i++)
  {
    srcCode2[posTabelaUso2[i]] += tgd[rotTabelaUso2[i]];
    // cout << "trocando o valor de " << rotTabelaUso2[i] << " em " << posTabelaUso2[i] << " por " << tgd[rotTabelaUso2[i]] << endl;
  }
  for (size_t i = 0; i < realocacao2.size(); ++i)
  {
    // cout << realocacao2[i] << ": " << srcCode2[realocacao2[i]] << " " << endl;
    std::vector<int>::iterator it = std::find(posTabelaUso2.begin(), posTabelaUso2.end(), realocacao2[i]);
    if (it == posTabelaUso2.end())
    {
      // cout << "realocando pos " << realocacao2[i] << "por fator +=" << fator_corr_B << endl;
      srcCode2[realocacao2[i]] += fator_corr_B;
    }
    // else
    // {
    //   cout << "pos " << realocacao2[i] << " nao realocada, pois esta em TGD" << endl;
    // }

  }
  // cout << "MOD B depois realocacao" << endl;
  // for (size_t i = 0; i != srcCode2.size(); i++)
  // {
  //   cout << srcCode2[i] << " ";
  // }
  // cout << endl;
  // cout << endl;

  // concatendando os codigos
  vector<int> finalCode;
  finalCode.resize(cod1_size + cod2_size);
  cout << endl << finalCode.size() << endl;

  for(size_t i = 0; i < srcCode1.size(); i++)
  {
    finalCode[i] = srcCode1[i];
  }
  for(size_t i = 0; i < srcCode2.size(); i++)
  {
    finalCode[i + cod1_size] = srcCode2[i];
    // cout << i<< " " << finalCode[i+ cod1_size] << " " << endl;
  }
  // cout << endl << finalCode.size() << endl;

  ofstream executavel("a.out");
  for(size_t i = 0; i < finalCode.size(); i++)
  {
    executavel << finalCode[i] << " ";
  }
  executavel << endl;

}

int main(int argc, char* argv[])
{
    // Declaração de Variáveis
    string Nome_Arquivo_Entrada_1;
    string Nome_Arquivo_Entrada_2;                    // arquivo de entrada .obj
    string Nome_1;
    string Nome_2;

    if (argc == 2)
    {
        cerr << "\n \n Um programa inserido..." << endl;
        Nome_1 = argv[1];
        size_t obj_find = Nome_1.find(".obj");
        if (obj_find == string::npos)
        {
          Nome_Arquivo_Entrada_1 = Nome_1 + ".obj";
          // Nome_1 += ".obj";
        }
        else
        {
          Nome_Arquivo_Entrada_1 = Nome_1;
          Nome_1 = Nome_1.substr(0, obj_find);
        }
        cout << "opening " << Nome_Arquivo_Entrada_1 << endl;
        process(Nome_Arquivo_Entrada_1, Nome_1);
    }

    else if (argc == 3)
    {
        cerr << "\n \n Dois programas inseridos..." << endl;
        Nome_1 = argv[1];
        Nome_2 = argv[2];
      	size_t obj_find = Nome_1.find(".obj");
        if (obj_find == string::npos)
        {
          Nome_Arquivo_Entrada_1 = Nome_1 + ".obj";
          // Nome_1 += ".obj";
        }
        else
        {
          Nome_Arquivo_Entrada_1 = Nome_1;
          Nome_1 = Nome_1.substr(0, obj_find);
        }
        
        obj_find = Nome_2.find(".obj");
        if (obj_find == string::npos)
        {
          Nome_Arquivo_Entrada_2 = Nome_2 + ".obj";
          // Nome_2 += ".obj";
        }
        else
        {
          Nome_Arquivo_Entrada_2 = Nome_2;
          Nome_2 = Nome_2.substr(0, obj_find);
        }

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

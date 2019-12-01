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

string removeSpaces(string str)
{
    str.erase(remove(str.begin(), str.end(), ' '), str.end());
    return str;
}

string noComments(string linha)
{
	size_t Achado = linha.find_first_of('#');
	size_t Achado2 = linha.find_first_of(';');
	if(Achado != string::npos)
	{
		linha = linha.substr(0, Achado);
		return(linha);
	}
	else if(Achado2 != string::npos){
        cout << linha << endl;
		linha = linha.substr(0, Achado2);
        cout << linha << endl;
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

void process(const string& Nome_Arquivo_Entrada_1, const string& Nome_1, bool two)
{
    vector <string> Cod;
	vector <string> Rotulos2;
	vector <string> tokens;
	vector<string> operando;
	vector<string> inst_op;
	vector<string> Rot_Tab_Uso;
	vector<string> Rot_Tab_Def;
	vector<string> Tab_Uso;

	vector <int> End_Rotulo;
	vector <int> realocacao;
	vector<int> End_Tab_Uso;
	vector<int> End_Tab_Def;
    vector<int> operando_ln;

	int Num=0, x=0, Aux7=0, Aux9=0, y=0, Modulo=0, teve_soma=0, teve_public=0, nlinha = 0;
    int memory_data = 0;
	int unsigned i = 0;

    string linha;
	string Aux3;
	string Aux4;
    string Aux5;
	string buf;

	if (two==false){

    Modulo=0;

	  ifstream Codigo(Nome_Arquivo_Entrada_1);

	  if(Codigo.is_open())
	  {
	    while(getline(Codigo,linha))
	    {
        linha = noComments(linha);
		linha = Maiusculo(linha);

        if ((linha.find("ADD")!=string::npos)||(linha.find("SUB")!=string::npos)||(linha.find("MULT")!=string::npos)||(linha.find("DIV")!=string::npos)||
            (linha.find("JMP")!=string::npos)||(linha.find("JMPN")!=string::npos)||(linha.find("JMPP")!=string::npos)||(linha.find("JMPZ")!=string::npos)||
            (linha.find("LOAD")!=string::npos)||(linha.find("STORE")!=string::npos)||(linha.find("INPUT")!=string::npos)||(linha.find("OUTPUT")!=string::npos)){
          nlinha = nlinha + 2;
        }
        if(linha.find("COPY")!=string::npos){
          nlinha = nlinha + 3;
        }
        if(linha.find("STOP")!=string::npos){
          nlinha++;
        }
        // cout << linha << endl;
        if((linha.find("SECTION TEXT")!=string::npos)||(linha.find("SECTION DATA")!=string::npos)){
          continue;
        }



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

				stringstream ss(linha);

			  while (ss >> buf)
			    {
			    tokens.push_back(buf);              //Pilha com todos os tokens
			    }

				for(i=0; i<tokens.size(); i++)
			    {
			    Cod.push_back(tokens[i]);
			    }

          for(i=0;i<tokens.size();i++)
      		{

      			if(tokens[i] == "MULT" || tokens[i] == "DIV" || tokens[i] == "ADD" || tokens[i] == "SUB"|| tokens[i] == "LOAD" || tokens[i] == "STORE" || tokens[i] == "INPUT" || tokens[i] == "OUTPUT" || tokens[i] == "JMP" || tokens[i] == "JMPN" || tokens[i] == "JMPZ" || tokens[i] == "JMPP")
      			{
      				Aux5 = tokens[i];
      				inst_op.push_back(Aux5);						//Salvando a instru��o correspondente ao operando porque tava dando as linhas erradas
      				Aux5 = tokens[i+1];
      				operando.push_back(Aux5);					//Salvando cada operando
      				operando_ln.push_back(nlinha);					//Salvando a sua respectiva linha
      			}
      			if(tokens[i] == "COPY")
      			{
      				Aux5 = tokens[i+1];
      				size_t Achado = Aux5.find_first_of(",");
      				if(Achado != string::npos)
      				{

      					Aux5 = Aux5.substr(0, Achado);					//Retirando a v�rgula dos argumentos que tem, como o COPY
      				}
      				operando.push_back(Aux5);
      				operando_ln.push_back(nlinha);
      				Aux5 = tokens[i+2];
      				operando.push_back(Aux5);
      				operando_ln.push_back(nlinha);
      				Aux5 = tokens[i];
      				inst_op.push_back(Aux5);
      				inst_op.push_back(Aux5);						//Duas vezes porque h� duas linhas e dois operandos para esta instru��o
      			}

      		}

          tokens.clear();

	    }
	    cout << "Programa não possui diretivas de Módulos! Continuando..." << endl;
	  }



	  Codigo.close();
	}

	else{
		Modulo = 1;
		bool begin = false;
	  bool end = false;

	  ifstream Codigo(Nome_Arquivo_Entrada_1);

	  if(Codigo.is_open())
	  {
	    while(getline(Codigo,linha))
	    {

        if ((linha.find("ADD")!=string::npos)||(linha.find("SUB")!=string::npos)||(linha.find("MULT")!=string::npos)||(linha.find("DIV")!=string::npos)||(linha.find("JMP")!=string::npos)||(linha.find("JMPN")!=string::npos)||(linha.find("JMPP")!=string::npos)||(linha.find("JMPZ")!=string::npos)||(linha.find("LOAD")!=string::npos)||(linha.find("STORE")!=string::npos)||(linha.find("INPUT")!=string::npos)||(linha.find("OUTPUT")!=string::npos)){
          nlinha = nlinha + 2;
        }
        if(linha.find("COPY")!=string::npos){
          nlinha = nlinha + 3;
        }
        if(linha.find("STOP")!=string::npos){
          nlinha++;
        }
        // cout << linha << endl;
        if((linha.find("SECTION TEXT")!=string::npos)||(linha.find("SECTION DATA")!=string::npos)){
          continue;
        }

        linha = noComments(linha);
		linha = Maiusculo(linha);

	      size_t begin_find = linha.find("BEGIN");
	      size_t end_find = linha.find("END");

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

        stringstream ss(linha);

        while (ss >> buf)
          {
          tokens.push_back(buf);              //Pilha com todos os tokens
          }

        for(i=0; i<tokens.size(); i++)
          {
          Cod.push_back(tokens[i]);
          }

          for(i=0;i<tokens.size();i++)
      		{

      			if(tokens[i] == "MULT" || tokens[i] == "DIV" || tokens[i] == "ADD" || tokens[i] == "SUB"|| tokens[i] == "LOAD" || tokens[i] == "STORE" || tokens[i] == "INPUT" || tokens[i] == "OUTPUT" || tokens[i] == "JMP" || tokens[i] == "JMPN" || tokens[i] == "JMPZ" || tokens[i] == "JMPP")
      			{
      				Aux5 = tokens[i];
      				inst_op.push_back(Aux5);						//Salvando a instru��o correspondente ao operando porque tava dando as linhas erradas
      				Aux5 = tokens[i+1];
      				operando.push_back(Aux5);					//Salvando cada operando
      				operando_ln.push_back(nlinha);					//Salvando a sua respectiva linha
      			}
      			if(tokens[i] == "COPY")
      			{
      				Aux5 = tokens[i+1];
      				size_t Achado = Aux5.find_first_of(",");
      				if(Achado != string::npos)
      				{

      					Aux5 = Aux5.substr(0, Achado);					//Retirando a v�rgula dos argumentos que tem, como o COPY
      				}
      				operando.push_back(Aux5);
      				operando_ln.push_back(nlinha);
      				Aux5 = tokens[i+2];
      				operando.push_back(Aux5);
      				operando_ln.push_back(nlinha);
      				Aux5 = tokens[i];
      				inst_op.push_back(Aux5);
      				inst_op.push_back(Aux5);						//Duas vezes porque h� duas linhas e dois operandos para esta instru��o
      			}

      		}

          tokens.clear();

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
	}

		for(i=0;i<Cod.size();i++)
			{

	    size_t Achado = Cod[i].find_first_of(",");

	    if(Achado != string::npos)
	    	{
	        Aux4 = Cod[i].substr(0, Achado);                    //Retirando a vírgula dos argumentos que tem, como o COPY

	        Cod[i] = Aux4;
	    	}
			}

			//Passagem

			x = 0;
			for(i=0;i<Cod.size();i++)
			{

        if ((Cod[i].find("ADD")!=string::npos)||(Cod[i].find("SUB")!=string::npos)||(Cod[i].find("MULT")!=string::npos)||(Cod[i].find("DIV")!=string::npos)||(Cod[i].find("JMP")!=string::npos)||(Cod[i].find("JMPN")!=string::npos)||(Cod[i].find("JMPP")!=string::npos)||(Cod[i].find("JMPZ")!=string::npos)||(Cod[i].find("LOAD")!=string::npos)||(Cod[i].find("STORE")!=string::npos)||(Cod[i].find("INPUT")!=string::npos)||(Cod[i].find("OUTPUT")!=string::npos)){
          x = x + 2;
        }
        if(Cod[i].find("COPY")!=string::npos){
          x = x + 3;
        }
        if(Cod[i].find("STOP")!=string::npos){
          x++;
        }

        if(Cod[i] == "SPACE")
				{
                    try
                    {
                    // cout << Cod.at(i+1) << endl;
					if(!isalpha(Cod.at(i+1).at(0))){
                        x = x + stoi(Cod[i+1]);
                    }
                    else{
                        x++;
                    }
                    }
                    catch(...)
                    {

                    }
				}

				if(Cod[i] == "CONST")
				{
					x++;
				}

				size_t Achado = Cod[i].find_first_of(":");

				if(Achado != string::npos && Modulo == 0)					//Montando tabela de S�mbolos encontrando os endere�os dos labels e tirando eles do c�digo
				{
					Aux3 = Cod[i].substr(0, Achado);
					Rotulos2.push_back(Aux3);
					End_Rotulo.push_back(x);
          x++;
				}

				else if(Achado != string::npos && Modulo == 1)
				{
					if(Cod[i+1] == "BEGIN")
					{

						x = 0;											//Quando encontra um label apontando para BEGIN e EXTERN o contador x se mant�m zerado

					}
					else if(Cod[i+1] == "EXTERN")
					{
						Aux3 = Cod[i].substr(0, Achado);
						Rot_Tab_Uso.push_back(Aux3);
						x = 0;									//Assumindo que o EXTERN e o PUBLIC estar�o no in�cio

					}
					else
					{

						Aux3 = Cod[i].substr(0, Achado);
						Rotulos2.push_back(Aux3);
						End_Rotulo.push_back(x);

					}
				}

				if(teve_public == 1)
				{
					Aux3 = Cod[i];
					Rot_Tab_Def.push_back(Aux3);
					End_Tab_Def.push_back(x);
					teve_public = 0;

				}
				if(Cod[i] == "PUBLIC")
				{
					x = 0;
					teve_public = 1;

				}

			}
			///////OPCODES/////////

			for(i=0;i<Cod.size();i++)
			{

				Recomecar:


				/////Retiradas de diretivas/////


				if(Cod[i] == "BEGIN")
				{
					Cod.erase(Cod.begin() + i);
					Cod.erase(Cod.begin() + i - 1);
					i--;
					goto Recomecar;
				}
				if(Cod[i] == "EXTERN")
				{
					Cod.erase(Cod.begin() + i);
					Cod.erase(Cod.begin() + i - 1);
					i--;
					goto Recomecar;
				}
				if(Cod[i] == "PUBLIC")
				{
					Cod.erase(Cod.begin() + i);
					Cod.erase(Cod.begin() + i);
					goto Recomecar;
				}
				if(Cod[i] == "SPACE")
				{
					Cod.erase(Cod.begin() + i);
                    memory_data++;
					if(!isalpha(Cod[i][0]))				//Preenchendo com 0's onde ficava a diretiva SPACE e onde ficava o operando
					{
                      switch (stoi(Cod[i])){
                        case 1:
                          Cod[i] = "0";
                          // memory_data++;
                          break;

                        case 2:
                          Cod[i] = "0 0";
                          memory_data += 1;
                          break;

                        case 3:
                          Cod[i] = "0 0 0";
                          memory_data += 2;
                          break;

                        case 4:
                          Cod[i] = "0 0 0 0";
                          memory_data += 3;
                          break;

                        default:
                          // cout << "zeros > 4" << endl;
                          int n_zeros = stoi(Cod[i]);
                          memory_data += n_zeros - 1;
                          if(n_zeros < 0) break;
                          string str_aux;
                          str_aux = "";
                          for(int count = 0; count < n_zeros; count++)
                          {
                              if (count < (n_zeros - 1))
                                  str_aux += "0 ";
                              else
                                  str_aux += "0";
                          }
                          Cod[i] = str_aux.c_str();
                          break;
                        }
					}
                    else{
                        Cod.insert(Cod.begin() + i, "0");
                      // Cod[i] = "0";
                    }
					goto Recomecar;
				}
				if(Cod[i] == "CONST")
				{
					Cod.erase(Cod.begin() + i);
					Cod.erase(Cod.begin() + i - 1);
                    memory_data++;
					i--;
					if(Cod[i].size() > 2)
					{
						if(Cod[i][0] == '0' && Cod[i][1] == 'X')
						{
						 	Num = std::stol (Cod[i],nullptr,0);
						 	Cod[i] = to_string(Num);
						}
					}
					goto Recomecar;
				}
				if(Cod[i] == "END")
				{
					Cod.erase(Cod.begin() + i);
					i--;
					goto Recomecar;
				}
				if(Cod[i] == "ADD")
				{
					Cod[i] = "1";
				}
				if(Cod[i] == "SUB")
				{
					Cod[i] = "2";
				}
				if(Cod[i] == "MULT")
				{
					Cod[i] = "3";
				}
				if(Cod[i] == "DIV")
				{
					Cod[i] = "4";
				}
				if(Cod[i] == "JMP")
				{
					Cod[i] = "5";
				}
				if(Cod[i] == "JMPN")
				{
					Cod[i] = "6";
				}
				if(Cod[i] == "JMPP")
				{
					Cod[i] = "7";
				}
				if(Cod[i] == "JMPZ")
				{
					Cod[i] = "8";
				}
				if(Cod[i] == "COPY")
				{
					Cod[i] = "9";
				}
				if(Cod[i] == "LOAD")
				{
					Cod[i] = "10";
				}
				if(Cod[i] == "STORE")
				{
					Cod[i] = "11";
				}
				if(Cod[i] == "INPUT")
				{
					Cod[i] = "12";
				}
				if(Cod[i] == "OUTPUT")
				{
					Cod[i] = "13";
				}
				if(Cod[i] == "STOP")
				{
					Cod[i] = "14";
				}
			}


			for(i=0;i<Cod.size();i++)
			{

					///////Retirada de Labels/////////
				size_t Achado = Cod[i].find_first_of(":");
				if(Achado != string::npos)
				{
					Cod.erase(Cod.begin() + i);
				}
			}

			//Realocacao


			for(i=0;i<Cod.size();i++)
			{

				if(Cod[i] == "+")
				{
					y = y-2;
				}
				for(int unsigned j=0;j<operando.size();j++)
				{
					if(operando[j] == Cod[i])
					{
						if(inst_op[j] == "ADD")
						{
							if(Cod[i-1] == "1")
							{
								realocacao.push_back(y);
							}
						}
						if(inst_op[j] == "SUB")
						{
							if(Cod[i-1] == "2")
							{
								realocacao.push_back(y);
							}
						}
						if(inst_op[j] == "MULT")
						{
							if(Cod[i-1] == "3")
							{
								realocacao.push_back(y);
							}
						}
						if(inst_op[j] == "DIV")
						{
							if(Cod[i-1] == "4")
							{
								realocacao.push_back(y);
							}
						}
						if(inst_op[j] == "JMP")
						{
							if(Cod[i-1] == "5")
							{
								realocacao.push_back(y);						//Salva o endere�o deste operando para a tabela de realoca��o
							}
						}
						if(inst_op[j] == "JMPN")
						{
							if(Cod[i-1] == "6")
							{
								realocacao.push_back(y);						//Salva o endere�o deste operando para a tabela de realoca��o
							}
						}
						if(inst_op[j] == "JMPP")
						{
							if(Cod[i-1] == "7")
							{
								realocacao.push_back(y);						//Salva o endere�o deste operando para a tabela de realoca��o
							}
						}
						if(inst_op[j] == "JMPZ")
						{
							if(Cod[i-1] == "8")
							{
								realocacao.push_back(y);						//Salva o endere�o deste operando para a tabela de realoca��o
							}
						}
						if(inst_op[j] == "COPY")
						{
							if(Cod[i-1] == "9")
							{
								realocacao.push_back(y);						//Salva o endere�o deste operando para a tabela de realoca��o
							}
						}
						if(inst_op[j] == "LOAD")
						{
							if(Cod[i-1] == "10")
							{
								realocacao.push_back(y);						//Salva o endere�o deste operando para a tabela de realoca��o
							}
						}
						if(inst_op[j] == "STORE")
						{
							if(Cod[i-1] == "11")
							{
								realocacao.push_back(y);						//Salva o endere�o deste operando para a tabela de realoca��o
							}
						}
						if(inst_op[j] == "INPUT")
						{
							if(Cod[i-1] == "12")
							{
								realocacao.push_back(y);						//Salva o endere�o deste operando para a tabela de realoca��o
							}
						}
						if(inst_op[j] == "OUTPUT")
						{
							if(Cod[i-1] == "13")
							{
								realocacao.push_back(y);						//Salva o endere�o deste operando para a tabela de realoca��o
							}
						}
					}
				}
				y++;
			}



			//Tabela de uso

			for(i=0;i<Cod.size();i++)
			{
				if(Cod[i] == "+")
				{
					teve_soma = 1;
				}
				for(int unsigned j=0; j<Rot_Tab_Uso.size(); j++)
				{

					if(Cod[i] == Rot_Tab_Uso[j])
					{

						Cod[i] = "0";
						Tab_Uso.push_back(Rot_Tab_Uso[j]);
						if(teve_soma == 1)
						{
							End_Tab_Uso.push_back(i-2);
						}
						else
						{
							End_Tab_Uso.push_back(i);
						}
					}
				}

			}


			//Endreços solving

			for(i=0;i<Cod.size();i++)
			{
				for(int unsigned j=0; j<Rotulos2.size(); j++)
				{

					if(Cod[i] == Rotulos2[j])
					{
						Cod[i] = to_string(End_Rotulo[j]);
					}
				}
			}



			//Tabela de definição

			if(Modulo == 1)
			{



			for(i=0;i<Rotulos2.size();i++)
			{

				for(int unsigned j=0; j<Rot_Tab_Def.size(); j++)
				{
					if(Rot_Tab_Def[j] == Rotulos2[i])						//Procura os rotulos da tabela de defini��o para atuliazar os endere�os
					{

						End_Tab_Def[j] = End_Rotulo[i];

					}
				}
			}

		//Vetores

			}

			for(i=0;i<Cod.size();i++)
			{
				if(Cod[i] == "+")
				{
					Aux7 = stoi(Cod[i+1]);
					Aux9 = stoi(Cod[i-1]);
					Aux7 = Aux7 + Aux9;
					Cod[i-1] = to_string(Aux7);
					Cod.erase(Cod.begin() + i);
					Cod.erase(Cod.begin() + i);
				}
      }

			//Foutt


			if(Modulo == 1)
			{
				ofstream fout;
				fout.open(Nome_1 + ".obj");
        fout << "H: "<<Nome_1<<endl;

        fout << "H: " << nlinha + memory_data << endl;

        fout << "H: Realocacao - ";
				for(i=0;i<realocacao.size();i++)
				{
					fout<<realocacao[i]<<" ";
				}

				fout<<endl<<"H: Tabela de Uso - "<<endl;
				for(i=0;i<Tab_Uso.size();i++)
				{
					fout<<"H: "<<Tab_Uso[i]<<" "<<End_Tab_Uso[i]<<endl;
				}
				fout<<"H: Tabela de definicoes - "<<endl;
				for(i=0;i<Rot_Tab_Def.size();i++)
				{
					fout<<"H: "<<Rot_Tab_Def[i]<<" "<<End_Tab_Def[i]<<endl;
				}

				fout<<endl<<"T: ";
				for(i=0;i<Cod.size();i++)
				{
					fout<<Cod[i]<<" ";
				}
			}
			else
			{
				ofstream fout;
				fout.open(Nome_1 + ".obj");

        fout<<"H: "<<Nome_1<<endl;

        fout<<"H: " << nlinha + memory_data << endl;

        fout<<"H: Realocacao - ";

				for(i=0;i<realocacao.size();i++)
				{
					fout<<realocacao[i]<<" ";
				}

        fout<<endl<<"T: ";
				for(i=0;i<Cod.size();i++)
				{
					fout<<Cod[i]<<" ";
				}
			}

}


int main(int argc, char* argv[])
{
    // Declaração de Variáveis
    string Nome_Arquivo_Entrada_1;
    string Nome_Arquivo_Entrada_2;                    // arquivo de entrada .asm
    string Nome_1;
    string Nome_2;
		bool two;

    if (argc == 2)
    {
        cerr << "\n \n Um programa inserido..." << endl;
				two=false;
        Nome_1 = argv[1];
        Nome_Arquivo_Entrada_1 = Nome_1;
        cout << "opening " << Nome_Arquivo_Entrada_1 << endl;
        process(Nome_Arquivo_Entrada_1, Nome_1, two);
    }

    else if (argc == 3)
    {
        cerr << "\n \n Dois programas inseridos..." << endl;
        Nome_1 = argv[1];
        Nome_2 = argv[2];
      	Nome_Arquivo_Entrada_1 = Nome_1;
        Nome_Arquivo_Entrada_2 = Nome_2;
				two=true;

      	cout << "opening " << Nome_Arquivo_Entrada_1 << endl;

      	process(Nome_Arquivo_Entrada_1, Nome_1, two);

				cout << "opening " << Nome_Arquivo_Entrada_2 << endl;

				process(Nome_Arquivo_Entrada_2, Nome_2, two);
    }

    else
    {
        cout << "\n \n Numero de argumentos invalido..." << endl;
        exit(1);
    }

    cout << "Programa finalizou..." << endl;
    return 0;

}

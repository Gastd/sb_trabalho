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

string Apagar_Comentarios(string linha)
{
	size_t Achado = linha.find_first_of(";");
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
	
	restante = linha.size();				//No início falta a linha toda
	while (restante>0)
	{	
		linha[i] = toupper(linha[i]);		//Passa caracter por caracter
		i++;
		restante = linha.size() - i;		//Verifica se já terminou o programa	
	}
	return(linha);
}


vector<string> Apenas_Tokens(string linha)
{
    string buffer;
    stringstream ss(linha);
    vector<string> tokens;
    
    while (ss >> buffer)
    {
	    tokens.push_back(buffer);
	}
    return(tokens);
}


void Montador(string Nome_Arquivo_Entrada, string Nome_Original)
{
	ofstream fout;
	ofstream fouta;
	
	string linha;
	string buf; 
	string Aux_EQU;
	string Aux;
	string Aux_JUMP;
	string Aux3;
	string Aux4;
	string Aux5;
	string Aux6;
	string Tipo;
	string linha_erro;
	//Vetores String//
	
	vector<string> tokens;
	vector<string> tokens_carac;
	vector<string> completo;
	vector<string> Inst_var;
	vector<string> Restante;
	vector<string> Nomes_macro;
	vector<string> linha_completa;
	vector<string> var_equ;
	vector<string> val_equ;
	vector<string> final;
	vector<string> val_EQU;
	vector<string> var_EQU;
	vector<string> printa;
	vector<string> Rotulo;
	vector<string> tipo_erro;
	vector<string> Cod;
	vector<string> Rotulos2;
	vector<string> Rot_Tab_Uso;
	vector<string> Rot_Tab_Def;
	vector<string> operando;
	vector<string> Rot_Const;
	vector<string> inst_op;
	vector<string> Tab_Uso;
	vector<string> Pre_Cod;
	vector<string> Label_Space;
	vector<string> Arg_Space;
	
	//Vetores Int//
	vector<int> Linhas_text;
	vector<int>	Linhas_data;
	vector<int> Linha_Rotulo;
	vector<int> flag_labels;
	vector<int> End_Rotulo;
	vector<int> End_Tab_Uso;
	vector<int> End_Tab_Def;
	vector<int> operando_ln;
	vector<int> zero;
	vector<int> realocacao;
	int unsigned i, ntokens=0; 
	int tamanho_anterior_tokens = 0, nlinha = 0, linha_rotulo = 0, Aux2, Num = 0, Aux7 = 0, Aux9 = 0, y = 0;
	//FLAGS//
	int teve_text = 0, antes_text = 0, secao_text = 0, secao_data = 0, inst_if = 0, inst_equ = 0, teve_if = 0, nRotulo = 0, if_valido = 0, teve_soma = 0;
	int ha_rotulo = 1, ind_erro = 0, Encontrou_algo = 0, secao_bss = 0, teve_rotulo = 0, Modulo = 0, teve_public = 0, achou = 0, x, modulo = 0, achou_end = 0, achou_stop = 0, tirar_linha = 0, Aux13 = 0, Aux14 = 0;
	
	//Vetor com os 3 tipos de erro
	tipo_erro.push_back("Erro Lexico");
	tipo_erro.push_back("Erro Sintatico");
	tipo_erro.push_back("Erro Semantico");


	ifstream Codigo(Nome_Arquivo_Entrada);
	
	if(Codigo.is_open())
	{
			
		while(getline(Codigo,linha))
		{	
			tirar_linha = 0;									//Preciso para apagar o \n no pré-processamento	
			ha_rotulo = 0;										//Nova linha, é preciso verificar novamente se há rótulo
			inst_if = 0;										//Nova linha, é preciso verificar novamente se tem IF
			tamanho_anterior_tokens = tokens.size();			//Quantidade total de tokens antes da linha atual
			linha = Apagar_Comentarios(linha);
			linha = Maiusculo(linha);
			Pre_Cod.push_back(linha);
			
			
			/////////////////////////////////////////
			////Verificando se a linha está vazia////
			/////////////////////////////////////////
			
			if(linha.empty())
			{
				continue;
			}
			
			///////////////////////////////////////
			// Separando os tokens pelos espaços //
			///////////////////////////////////////
			
    		stringstream ss(linha);
			
   			while (ss >> buf)
   			{
   				
      			tokens.push_back(buf);				//Pilha com todos os tokens
			}	
			nlinha = nlinha+1;	
			ntokens = tokens.size() - tamanho_anterior_tokens;			//Número de tokens da linha
			
			
			
			/////////////////////////////////////////////////
			/////////////////Reirando os Tab's///////////////
			/////////////////////////////////////////////////
			
			char espaco[] = "	";
			for (i = 0; i < strlen(espaco); ++i)
		   {
		      linha.erase (remove(linha.begin(), linha.end(), espaco[i]), linha.end());
		   }
			
			
			///////////////////////////////////////////////////////////////////////////////////////////
			size_t Achado = linha.find_first_of(":");			//Verifica se tem rótulo na linha atual
			///////////////////////////////////////////////////////////////////////////////////////////
			
			////////////////////////////////////////////////////////////
			////////////Procurando por characteres inválidos////////////
			////////////////////////////////////////////////////////////
			
			for(i=0;i<tokens.size();i++)
			{
				
				if (tokens[i].find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_:+,") != string::npos)
					{
					    ind_erro = 0;
					    goto Interrupcao;
					}
			}
			
			
			for(i=0; i<linha.size();i++)
			{
				
				if(linha[i] == 0X3A )			//Codigo Hexa de :
				{
					nRotulo++;
				
				}
			}
			if(nRotulo > 1)						//Se tem mais de um : é porque tem mais de um rótulo, então erro sintático
			{
				ind_erro = 1;
				goto Interrupcao;
			}
			
			nRotulo = 0;						//Zera contador de : porque já foi verificado que não há dois rótulos na mesma linha

			for(i=0; i<tokens.size(); i++)
			{
				Cod.push_back(tokens[i]);
			}
			//////////////////////////////////////
			//Verifica se há rótulos nesta linha//
			////////e salva eles em Rótulo////////
			//////////////////////////////////////
			
			
				
				if(Achado != string::npos)
					{
						if(teve_rotulo == 1 && nlinha == linha_rotulo + 1)	//Se já teve um rótulo sozinho e a linha seguinte está tendo um novo label, erro
						{
							ind_erro = 2;							//Se tiver rótulo
							goto Interrupcao;
						}
						Aux = tokens[0].substr(0, Achado);					//Retirando o : do token do nome da variavel
						tokens[0] = Aux;
						if(isalpha(Aux[0]))							//verifica se inicia com letra
						{
							for(i=0;i<Rotulo.size();i++)
							{
								if(Aux == Rotulo[i])					//Rótulo definido novamente
								{
									ind_erro = 2;
									goto Interrupcao;
								}
							}
							Rotulo.push_back(Aux);												//Salvando nome do Rotulo
							
							Linha_Rotulo.push_back(nlinha);										//Salvando linha do Rotulo para tabela de Simbolos
							ha_rotulo = 1;
							
							if(ntokens == 1)							//Nesta linha só tem o rótulo
							{
								nlinha--;
								teve_rotulo = 1;						//Indicar que já existe um rótulo, se houver outro na próxima linha, deve dar erro
								linha_rotulo = nlinha;					//Linha onde teve o rótulo sozinho
							}
						}
						else
						{
							
								ind_erro = 0;
								goto Interrupcao;
						}
					}
					
					if(nlinha == linha_rotulo + 1)			//Caso na linha seguinte não haja rótulo, zera a flag
					{
						teve_rotulo = 0;
					}
					
					
			/////////////////////////////////////////////////////
			///////Verificando se a SECTION é válida/////////////
			/////////////////////////////////////////////////////
			
			
			if(tokens[ntokens-tokens.size()] == "SECTION")					//Se for SECTION, deve ser um dos 3 abaixo
			{
				if(tokens[ntokens-tokens.size()+1] != "TEXT" && tokens[ntokens-tokens.size()+1] != "DATA" &&  tokens[ntokens-tokens.size()+1] != "BSS")
				{
					
					ind_erro = 1;
					goto Interrupcao;
				}
			}
			
			/////////////////////////////////////////////////////
			//Localizando a instruções em alguma das seções//////
			/////////////////////////////////////////////////////
			
			
				if(nlinha == 1)						//Caso estiver no início, índice dos vetores estava dando negativo
				{
					if(tokens[0] != tokens.back())
					{
						if(tokens[1] == "BEGIN")
						{
							modulo = 1;
							antes_text = 1;
							goto continuar;						}
						
						if(tokens[ntokens-tokens.size()] == "SECTION" && tokens[ntokens-tokens.size()+1] == "TEXT")		//Verifica se o código começa com SECTION TEXT
						{
							secao_text = 1;									//Encontrou o SECTION TEXT
							antes_text = 0;									//Não é preciso mais procurar por EQU, já entrou em SECTION TEXT
					 		teve_text = 1;									//Indicar que teve SECTION TEXT em algum momento
					 		goto continuar;
							
						}
						else if(tokens[ntokens-tokens.size()+1] == "EQU")					//Verifica se tem EQU antes do SECTION TEXT
							{
								antes_text = 1;		
								inst_equ = 1;			
								size_t Achado = tokens[0].find_first_of(":");
								Aux_EQU = tokens[i].substr(0, Achado);					//Retirando o : do token do nome da variavel
								tokens[i] = Aux_EQU;								//Salvando de volta na pilha
								var_EQU.push_back(tokens[ntokens-tokens.size()]);					//Salvando nome 
								val_EQU.push_back(tokens[ntokens-tokens.size()+2]);	
								tirar_linha = 1;
								while(!Pre_Cod.empty() && Pre_Cod[i] != "\n")
									{
					    				Pre_Cod.pop_back();								//Como está no início, não vai ter \n para ele parar
									}		
								for(i=0; i<ntokens;i++)
								{														//Retirando a instrução EQU
									Cod.pop_back();
								}
								goto continuar;
							}
					}
					else if(tokens[0] == "BEGIN")					//Caso na linha inicial tenha apenas 1 token, então pode ser um módulo sem label no BEGIN
						{
							antes_text = 1;
							goto continuar;
						}
						else
						{
							ind_erro = 2;
							
							goto Interrupcao;
						}
						
						ind_erro = 2;							//Se forem esssas instruções ou diretivas, então o tipo de erro é outro porque elas estão escritas corretamente				
						
						goto Interrupcao;
				}
				continuar:
				if(antes_text == 1 && nlinha > 1)									//Caso onde há parâmetros antes do SECTION TEXT
				{	
					if(tokens[ntokens-tokens.size()] == "SECTION" && tokens[ntokens-tokens.size()+1] == "TEXT")		//Verifica se o código começa com SECTION TEXT
					{
						secao_text = 1;									//Encontrou o SECTION TEXT
						antes_text = 0;									//Não é preciso mais procurar por EQU, já entrou em SECTION TEXT
						teve_text = 1;
						goto continuar_2;
						
					}
					else if(tokens[ntokens-tokens.size()+1] == "EQU")					//Verifica se tem mais EQU's antes do SECTION TEXT
						{			
							while(!Pre_Cod.empty() && Pre_Cod[i] != "\n")
							{
					    		Pre_Cod.pop_back();								//Como está no início, não vai ter \n para ele parar
							}
							tirar_linha = 1;
							antes_text = 1;	
							inst_equ = 1;												
							size_t Achado = tokens[0].find_first_of(":");
							Aux_EQU = tokens[0].substr(0, Achado);					//Retirando o : do token do nome da variavel
							tokens[0] = Aux_EQU;								//Salvando de volta na pilha
							var_EQU.push_back(tokens[ntokens-tokens.size()]);					//Salvando nome 
							val_EQU.push_back(tokens[ntokens-tokens.size()+2]);
							
							for(i=0; i<ntokens;i++)
							{
								Cod.pop_back();
							}
							goto continuar_2;
						
						}
						if(ha_rotulo == 0)
						{
							if(tokens[0] == "MULT" || tokens[0] == "DIV" || tokens[0] == "ADD" || tokens[0] == "SUB" || tokens[0] == "JMP" || tokens[0] == "JMPN" || tokens[0] == "JMPP" || tokens[0] == "JMPZ" || tokens[0] == "COPY" || tokens[0] == "LOAD" || tokens[0] == "STORE" || tokens[0] == "INPUT" || tokens[0] == "OUTPUT" || tokens[0] == "STOP" || tokens[0] == "IF" || tokens[0] == "PUBLIC" || tokens[0] == "EXTERN" || tokens[0] == "CONST" || tokens[0] == "SPACE" || tokens[0] == "BEGIN" || tokens[0] == "END")
							{
								ind_erro = 2;							//Se forem esssas instruções ou diretivas, então o tipo de erro é outro porque elas estão escritas corretamente				
								
								goto Interrupcao;
							}
						}
						if(teve_rotulo == 0)
						{
							if(tokens[1] == "MULT" || tokens[1] == "DIV" || tokens[1] == "ADD" || tokens[1] == "SUB" || tokens[1] == "JMP" || tokens[1] == "JMPN" || tokens[1] == "JMPP" || tokens[1] == "JMPZ" || tokens[1] == "COPY" || tokens[1] == "LOAD" || tokens[1] == "STORE" || tokens[1] == "INPUT" || tokens[1] == "OUTPUT" || tokens[1] == "STOP" || tokens[1] == "IF" || tokens[1] == "PUBLIC" || tokens[1] == "EXTERN" || tokens[1] == "CONST" || tokens[1] == "SPACE" || tokens[1] == "BEGIN" || tokens[1] == "END")
							{
								ind_erro = 2;							//Se forem esssas instruções ou diretivas, então o tipo de erro é outro porque elas estão escritas corretamente				
								
								goto Interrupcao;
							}
						}
						
						else
						{
							ind_erro = 1;								//Se não for nem instrução nem diretiva no lugar errado, então é sintático mesmo o erro
							goto Interrupcao;
						}
				}
				continuar_2:
				if ((teve_text == 0 && tokens[ntokens-tokens.size()] == "SECTION" && tokens[ntokens-tokens.size()+1] == "DATA" )|| (teve_text == 0 && tokens[ntokens-tokens.size()] == "SECTION" && tokens[ntokens-tokens.size()+1] == "BSS" ))
				{
						ind_erro = 2;
						goto Interrupcao;
				}
				else if (tokens[ntokens-tokens.size()] == "SECTION" && tokens[ntokens-tokens.size()+1] == "DATA")
					{
							secao_text = 0;								//Acabou a SECTION TEXT
							secao_data = 1;								//Encontrou a SECTION DATA
							secao_bss = 0;
					} 
				else if(tokens[ntokens-tokens.size()] == "SECTION" && tokens[ntokens-tokens.size()+1] == "BSS")
				{
					secao_text = 0;
					secao_data = 0;
					secao_bss = 1;
				}
					
				//////////////////////////
				//////Tratando IF/////////
				//////////////////////////
				if(secao_text == 1 && tokens[0] == "IF")
				{
					inst_if = 1;														//A instrução atual é um if
					teve_if = 1;														//A instrução anterior foi um if
					tirar_linha = 1;
					for(i=Pre_Cod.size()-1;i>0;i--)
					{
						if(Pre_Cod[i] != "\n")
						{
							Pre_Cod.pop_back();
						}
						else
						{
						goto aqui0;
						}
					}
					aqui0:

					for(i=0; i<ntokens;i++)
					{
						Cod.pop_back();
					}
						for(unsigned int p=0;p<var_EQU.size();p++)
						{
							if(var_EQU[p] == tokens[1])				//Se achar o nome da variável na pilha
							{
								tokens[1] = val_EQU[p];							//Substitui ela pelo seu respectivo valor
								Encontrou_algo++;
								
							}
						}
						if(Encontrou_algo == 0)									//Depois de verificar toda a pilha
							{												//Senão achar o nome na pilha é porque não foi definido, então é erro semântico
								ind_erro = 2;
								goto Interrupcao;
							}
						Encontrou_algo = 0;
					
					if(tokens[1] == "1")
					{
						if_valido = 1;
					}
				}
				
				else if(secao_text == 0 && tokens[0] == "IF")
				{
					ind_erro = 2;										//Instrução no campo errado
					goto Interrupcao;	
				}
				
				
					///////////////////////////////////////////////////////
					////Verificando se a linha seguinte vai fazer parte////
					///////////////////////////////////////////////////////
				
				if(teve_if == 1 && if_valido == 1 && inst_if == 0)						//Se teve IF, mas ao mesmo tempo não é mais a instrução IF
				{
					for(i=0; i<tokens.size();i++)
					{
							printa.push_back(tokens[i]);						//Printa na tela
							printa.push_back(" ");
					}
					printa.push_back("\n");
					if_valido = 0;									//Próximo if ainda vai precisar ser tratado
					teve_if = 0;									//If já foi tratado
				}
				else if(teve_if == 1 && if_valido == 0 && inst_if == 0)
				{
					for(i=0;i<ntokens;i++)
					{
						Cod.pop_back();
					}
					tirar_linha = 1;
					for(i=Pre_Cod.size()-1;i>0;i--)
					{
						if(Pre_Cod[i] != "\n")
						{
							Pre_Cod.pop_back();
						}
						else
						{
						goto aqui1;
						}
					}
					aqui1:
					if_valido = 0;									//Caso o IF não seja válido, simplesmente pulará a instrução
					teve_if = 0;
					
				}
				else if(inst_equ == 0 && inst_if == 0)		//Senão for a instrução IF ou EQU e ainda se teve IF e 
				{
					if(tokens.size()>1)
					{
						if(tokens[0] != "SECTION" && tokens[1] != "SECTION")
						{
							for(i=0; i<tokens.size();i++)
								{
									printa.push_back(tokens[i]);
									printa.push_back(" ");
								}
							printa.push_back("\n");
						}
						goto fora;
						
					}
					printa.push_back(tokens[0]);
					printa.push_back(" ");
					printa.push_back("\n");
				}
				
				fora:
				inst_equ = 0;
				inst_if = 0;					//Reinicia as flags, senão caso encontrasse equ ou if, não printaria mais nada
				//////////////////////////////////
				///////Reiniciando tokens/////////
				//////////////////////////////////

		//////////////////////////////////////////////////////////////
		//////////Verificando quantidade de operandos/////////////////
		//////////////////////////////////////////////////////////////
		
		////////////////Diretiva EQU antes da TEXT////////////////////
		
			if(antes_text == 1)
			{
				
				if(tokens[1] == "EQU")
				{
					if(tokens.size() == 3)
					{
						goto correto;
					}
				}
				if(tokens[1] == "BEGIN")
				{
					if(tokens.size() == 2)
					{
						goto correto;
					}
				}
				else
				{
					ind_erro = 1;							
					goto Interrupcao;
				}
			}
		//////Instruções e diretivas no campo TEXT////////
		
			if(secao_text == 1)
			{
				if(tokens[0] == "SECTION")				//Primeiro caso onde verifica se o SECTION TEXT tem apenas esses dois tokens 							
				{
					if(tokens[1] == "TEXT")
					{
						if(tokens.size() > 2)
						{
							
							ind_erro = 1;							
							goto Interrupcao;
						}
						goto correto;					
					}
				}
				if(ha_rotulo == 0)
				{
					if(tokens[0] == "MULT" || tokens[0] == "DIV" || tokens[0] == "ADD" || tokens[0] == "SUB" || tokens[0] == "JMP" || tokens[0] == "JMPN" || tokens[0] == "JMPP" || tokens[0] == "JMPZ" || tokens[0] == "LOAD" || tokens[0] == "STORE" || tokens[0] == "INPUT" || tokens[0] == "OUTPUT" || tokens[0] == "IF" || tokens[0] == "PUBLIC" || tokens[0] == "EXTERN" || tokens[0] == "BEGIN" || tokens[0] == "END")
					{
						if(tokens.size() == 2)				//Se não tiver apenas 2 tokens, só existe mais uma possibilidade
						{	
						
							goto correto;
						}
						else if(tokens.size() == 4)
							{
								if(tokens[2] == "+" || tokens[2] == "-")		//Se tiver 4 tokens e o terceiro não for de + ou -, então está errado
								{
									goto correto;
								}
							}
								
								ind_erro = 1;							
								goto Interrupcao;		
					}
					else if(tokens[0] == "COPY")				
					{
						if(tokens.size() == 3)								//Se não tiver 3 tokens, deve ter 5 e o quarto deve ser um + ou -
						{
							goto correto;
						}
						else if(tokens.size() == 5)
							{
								if(tokens[2] == "+" || tokens[3] == "+")
								{
									goto correto;
								}
							}
							
						else if(tokens.size() == 7)
							{
								if(tokens[2] == "+" && tokens[5] == "+")
								{
									goto correto;
								}
							}
							
							else
							{
							ind_erro = 1;									
							goto Interrupcao;
							}
						
					}
					else if(tokens[0] == "STOP" || tokens[0] == "END")
					{
						if(tokens.size() == 1)
						{									
							goto correto;
						}
					}
					else										//Se não for nenhuma das opções acima, então não se conhece a instrução
					{
						
						ind_erro = 1;						
						goto Interrupcao;
					}
					
				}
				else if(teve_rotulo == 0)						//Considerando que exista rótulo nesta linha é preciso garantir que ele não esteja sozinho caso contrário ele vai tentar ler tokens onde não existem e vai dar erro
					{
						if(tokens[1] == "MULT" || tokens[1] == "DIV" || tokens[1] == "ADD" || tokens[1] == "SUB" || tokens[1] == "JMP" || tokens[1] == "JMPN" || tokens[1] == "JMPP" || tokens[1] == "JMPZ" || tokens[1] == "LOAD" || tokens[1] == "STORE" || tokens[1] == "INPUT" || tokens[1] == "OUTPUT" || tokens[1] == "IF" || tokens[1] == "PUBLIC" || tokens[1] == "EXTERN" || tokens[1] == "BEGIN")
						{
							if(tokens[1] == "EXTERN" || tokens[1] == "BEGIN")			//Essas diretivas não podem ter somas por exemplo FAT: BEGIN + 3
							{
								if(tokens.size() == 2)
								{
									goto correto;
								}
								else
								{
								
									ind_erro = 1;							
									goto Interrupcao;	
								}
							}
							if(tokens.size() == 3)				//Se não tiver apenas 3 tokens, só existe mais uma possibilidade
							{
								goto correto;
							}
							else if(tokens.size() == 5)
								{
									if(tokens[4] == "+" || tokens[4] == "-")		//Se tiver 5 tokens e o terceiro não for de + ou -, então está errado
									{
										goto correto;
									}
								}
								else
								{
									ind_erro = 1;							
									goto Interrupcao;
								}		
						}
					}
						else if(tokens[1] == "COPY")				
						{
							if(tokens.size() == 4)								//Se não tiver 4 tokens, deve ter 6 e o quinto deve ser um + ou -
							{
								goto correto;
							}
							else if(tokens.size() == 6)							//Caso onde tiver soma em apenas um dos argumentos
								{
									if(tokens[4] == "+" || tokens[3] == "+")
									{
										goto correto;
									}
								}
							else if(tokens.size() == 8)							//Caso com dois argumentos com soma
								{
									if(tokens[3] == "+" && tokens[6] == "+")
									{
										goto correto;
									}
								}
								else
								{
									ind_erro = 1;									
									goto Interrupcao;
								}
								
							
						}
						else if(tokens[1] == "STOP")
						{
							if(tokens.size() == 2)
							{
								goto correto;
							}
							else
							{
								ind_erro = 1;
								goto Interrupcao;
							}
						}
						else 
						{
							ind_erro = 1;			//Se não for nenhuma das opções acima, então não se conhece a instrução	ou então é PUBLIC que não deveria ter rótulo		
							goto Interrupcao;
						}
					}
				
		
		
						
		/////////////////DIRETIVAS DA SECTION DATA//////////////////
		if(secao_data == 1)
		{
			if(tokens[0] == "SECTION")				//Primeiro caso onde verifica se o SECTION TEXT tem apenas esses dois tokens 							
			{
				if(tokens[1] == "DATA")
				{
					if(tokens.size() > 2)
					{
						ind_erro = 1;							
						goto Interrupcao;
					}
					goto correto;					/////////////////////////////////////////////////////
				}									//////Desta maneira não há como diferenciar//////////
			}										////////uma instrução no lugar errado ou uma ////////
			if(ha_rotulo == 0)						////////////////instrução desconhecida///////////////
			{										/////////////////////////////////////////////////////
				if(tokens[0] == "MULT" || tokens[0] == "DIV" || tokens[0] == "ADD" || tokens[0] == "SUB" || tokens[0] == "JMP" || tokens[0] == "JMPN" || tokens[0] == "JMPP" || tokens[0] == "JMPZ" || tokens[0] == "LOAD" || tokens[0] == "STORE" || tokens[0] == "INPUT" || tokens[0] == "OUTPUT" || tokens[0] == "IF" || tokens[0] == "PUBLIC" || tokens[0] == "EXTERN" || tokens[0] == "EQU" || tokens[0] == "STOP")
				{
					ind_erro = 2;							//Se forem esssas instruções ou diretivas, então o tipo de erro é outro porque elas estão escritas corretamente				
					goto Interrupcao;
				}
				if(tokens[0] == "CONST" )	
				{
					if(teve_rotulo == 1)
					{
						Aux6 = Rotulo.back();			//Se o rótulo para a diretiva CONST foi definido na linha anterior, então é esse label que deve ser salvo para não ser alterado
						Rot_Const.push_back(Aux6);
					}
					if(tokens.size() == 2)
					{
						goto correto;
					}
				}
				if(tokens[0] == "SPACE")
				{
					if(tokens.size() == 1 || tokens.size() == 2)
					{
						if(tokens.size() == 2)
						{
							Aux2 = stoi(tokens[1]);					//Transformando o argumento do SPACE em int
							for(int t=1;t<Aux2;t++)
							{	
								Cod.push_back("0");					//Alocando os espaços com 0's
							}
						}
						goto correto;								//No SPACE pode ou não estar especificado a quantidade
					}
				}
				else
				{
				ind_erro = 1;							//Se for quaisquer outros tokens (instruções/diretivas do TEXT) ou de tamanho diferente					
				goto Interrupcao;
				}	
			}
			
			if(teve_rotulo == 0)
			{
				if(tokens[1] == "MULT" || tokens[1] == "DIV" || tokens[1] == "ADD" || tokens[1] == "SUB" || tokens[1] == "JMP" || tokens[1] == "JMPN" || tokens[1] == "JMPP" || tokens[1] == "JMPZ" || tokens[1] == "LOAD" || tokens[1] == "STORE" || tokens[1] == "INPUT" || tokens[1] == "OUTPUT" || tokens[1] == "IF" || tokens[1] == "PUBLIC" || tokens[1] == "EXTERN" || tokens[1] == "EQU" || tokens[1] == "STOP")
						{
							ind_erro = 2;							//Se forem esssas instruções ou diretivas, então o tipo de erro é outro porque elas estão escritas corretamente				
							goto Interrupcao;
						}
				if(tokens[1] == "CONST")	
				{
					Aux6 = tokens[0];
					Rot_Const.push_back(Aux6);		//Salvando o rótulo correspondente a diretiva const para que ele não seja alterado
					if(tokens[2] == "0")
					{
						zero.push_back(1);			//Identificando se o CONST é zero ou não para o caso da divisão por zero
					}
					else
					{
						zero.push_back(0);
					}
					if(tokens.size() == 3)
					{
						goto correto;
					}
				}
				if(tokens[1] == "SPACE")
				{
					if(tokens.size() == 2 || tokens.size() == 3)
					{
						Label_Space.push_back(tokens[0]);			//Fazendo uma tabela com todos os labels dos SPACE's 
						if(tokens.size() == 3)
						{
							Arg_Space.push_back(tokens[2]);			//Salvando o argumento dos SPACE's
							Aux2 = stoi(tokens[2]);					//Transformando o argumento do SPACE em int
							for(int t=2;t<Aux2;t++)
							{	
								Cod.push_back("0");					//Alocando os espaços com 0's
							}
						}
						else
						{
							Arg_Space.push_back("1");				//Não ter argumento é o mesmo que ter argumento igual a 1
						}
						goto correto;
					}
				}
				else
				{
				ind_erro = 1;							
				goto Interrupcao;
				}
			}		
		}
		////////////DIRETIVAS DA SECTION BSS////////////////
//		if(secao_bss == 1)
//		{	
//			if(tokens[0] == "SECTION")				//Primeiro caso onde verifica se o SECTION TEXT tem apenas esses dois tokens 							
//			{
//				if(tokens[1] == "BSS")
//				{
//					if(tokens.size() > 2)
//					{	
//						ind_erro = 1;							
//						goto Interrupcao;
//					}
//					goto correto;					
//				}
//			}
//			if(ha_rotulo == 0)
//			{
//				if(tokens[0] == "MULT" || tokens[0] == "DIV" || tokens[0] == "ADD" || tokens[0] == "SUB" || tokens[0] == "JMP" || tokens[0] == "JMPN" || tokens[0] == "JMPP" || tokens[0] == "JMPZ" || tokens[0] == "LOAD" || tokens[0] == "STORE" || tokens[0] == "INPUT" || tokens[0] == "OUTPUT" || tokens[0] == "IF" || tokens[0] == "PUBLIC" || tokens[0] == "EXTERN" || tokens[0] == "EQU" || tokens[0] == "CONST")
//				{
//					ind_erro = 2;							//Se forem esssas instruções ou diretivas, então o tipo de erro é outro porque elas estão escritas corretamente				
//					goto Interrupcao;
//				}
//				if(tokens[0] == "END")
//				{
//					if(tokens.size() == 1)
//					{
//						goto correto;
//					}
//					else
//					{
//						ind_erro = 1;										
//						goto Interrupcao;
//					}
//				}
//				if(tokens[0] == "SPACE")
//				{
//					if(tokens.size() == 1 || tokens.size() == 2)
//					{
//						if(tokens.size() == 2)
//						{
//							Aux2 = stoi(tokens[1]);					//Transformando o argumento do SPACE em int
//							for(int t=1;t<Aux2;t++)
//							{	
//								Cod.push_back("0");					//Alocando os espaços com 0's
//							}
//						}
//						goto correto;								//No SPACE pode ou não estar especificado a quantidade
//					}
//				}
//				else
//				{
//				ind_erro = 1;							
//				goto Interrupcao;	
//				}
//			}
//			if(teve_rotulo == 0)
//			{
//				if(tokens[1] == "MULT" || tokens[1] == "DIV" || tokens[1] == "ADD" || tokens[1] == "SUB" || tokens[1] == "JMP" || tokens[1] == "JMPN" || tokens[1] == "JMPP" || tokens[1] == "JMPZ" || tokens[1] == "LOAD" || tokens[1] == "STORE" || tokens[1] == "INPUT" || tokens[1] == "OUTPUT" || tokens[1] == "IF" || tokens[1] == "PUBLIC" || tokens[1] == "EXTERN" || tokens[1] == "EQU" || tokens[1] == "CONST")
//				{
//					ind_erro = 2;							//Se forem esssas instruções ou diretivas, então o tipo de erro é outro porque elas estão escritas corretamente				
//					goto Interrupcao;
//				}
//				if(tokens[1] == "SPACE")
//				{
//					if(tokens.size() == 2 || tokens.size() == 3)
//					{
//						if(tokens.size() == 3)
//						{
//							Aux2 = stoi(tokens[2]);					//Transformando o argumento do SPACE em int
//							for(int t=2;t<Aux2;t++)
//							{	
//								Cod.push_back("0");					//Alocando os espaços com 0's
//							}
//						}
//						goto correto;
//					}
//				}
//				else
//				{
//				ind_erro = 1;							
//				goto Interrupcao;
//				}
//			}
//		}
			
			correto:
		
		
		///////////////////////////////////////////////////////
		/////Salvando a linha em que cada operando estava//////
		///////////////////////////////////////////////////////
			
		for(i=0;i<tokens.size();i++)
		{
		
			if(tokens[i] == "MULT" || tokens[i] == "DIV" || tokens[i] == "ADD" || tokens[i] == "SUB"|| tokens[i] == "LOAD" || tokens[i] == "STORE" || tokens[i] == "INPUT" || tokens[i] == "OUTPUT" || tokens[i] == "JMP" || tokens[i] == "JMPN" || tokens[i] == "JMPZ" || tokens[i] == "JMPP")
			{
				Aux5 = tokens[i];
				inst_op.push_back(Aux5);						//Salvando a instrução correspondente ao operando porque tava dando as linhas erradas
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
				
					Aux5 = Aux5.substr(0, Achado);					//Retirando a vírgula dos argumentos que tem, como o COPY
				}	
				operando.push_back(Aux5);
				operando_ln.push_back(nlinha);
				Aux5 = tokens[i+2];		
				operando.push_back(Aux5);
				operando_ln.push_back(nlinha);
				Aux5 = tokens[i];
				inst_op.push_back(Aux5);
				inst_op.push_back(Aux5);						//Duas vezes porque há duas linhas e dois operandos para esta instrução
			}	
			
		}

		///////////////////////////////////////////////////////
		//////////Localizando os rótulos no código/////////////	
		///////////////////////////////////////////////////////
		
		if(ha_rotulo == 1)
		{	
			if(antes_text == 1)
			{
				flag_labels.push_back(0);
			}
			if(secao_text == 1)
			{
				flag_labels.push_back(1);
			}
			if(secao_data == 1)
			{
				flag_labels.push_back(2);
			}
			if(secao_bss == 1)
			{
				flag_labels.push_back(3);
			}	
		}
		
				for(i=0;i<Cod.size();i++)
		{
			if(Cod[i] == "SECTION")
			{
				for(i=0; i<ntokens;i++)
				{
					Cod.pop_back();
				}
			}
		}
		//////////////////////////////////////////////////
		////Procurando END para o caso se for modulo /////
		//////////////////////////////////////////////////
		
		if(modulo == 1)
		{
			for(i=0;i<tokens.size();i++)							//se tiver encontrado begin no início, então vai procurar em todas as linhas por um END
			{
				if(tokens[i] == "END")
				{
					achou_end = achou_end+1;
				}
			}
		}
		else
		{
			for(i=0;i<tokens.size();i++)							//Se não for módulo, deve ter STOP
			{
				if(tokens[i] == "STOP")
				{
					achou_stop++;
				}
			}
		}
	goto Final;

	Interrupcao:
	linha_erro = to_string(nlinha);
	cout<<tipo_erro[ind_erro]<< " na linha "<<linha_erro<<endl;
	exit(0);

	

	Final:
		if(tirar_linha == 0)
		{
			Pre_Cod.push_back("\n");								//Quebra de linha necessária para a próxima instrução
		}
		

		while(!tokens.empty())
				{
    				tokens.pop_back();					//Limpando a pilha para as próximas linhas a serem analisadas
				}
	}


		Codigo.close();
	}
	
	
		fout.open(Nome_Original + ".pre");
		
			
		for(i=0;i<Pre_Cod.size();i++)
		{
			fout<<Pre_Cod[i]<<" ";
		}


if(modulo == 1)
{
	if(achou_end != 1)				//Se for módulo e não encontrar um e apenas um END no código inteiro, dá erro
	{
		ind_erro = 2;
		goto Interrupcao2;
	}
}
else
{
	if(achou_stop != 1)				//Se for módulo e não encontrar um e apenas um END no código inteiro, dá erro
	{
		ind_erro = 2;
		goto Interrupcao2;
	}
}

////////////////////////////////////////
////////////////SÍNTESE/////////////////
////////////////////////////////////////

///////////////////////////////////////////////////
///////////Retirando a vírgula do token////////////
///////////////////////////////////////////////////

for(i=0;i<Cod.size();i++)
{
	size_t Achado = Cod[i].find_first_of(",");
	if(Achado != string::npos)
	{
		Aux4 = Cod[i].substr(0, Achado);					//Retirando a vírgula dos argumentos que tem, como o COPY
		Cod[i] = Aux4;
	}
}


////////////////////////////////////////////////////////////////////////////
/////////////Verificando se os saltos são para regiões válidas//////////////
////////////////////////////////////////////////////////////////////////////


for(i=0;i<Cod.size();i++)
{
	if(Cod[i] == "JMP" || Cod[i] == "JMPN" || Cod[i] == "JMPP" || Cod[i] == "JMPZ")
	{
		for(int unsigned j=0;j<Rotulo.size();j++)
		{
			if(Cod[i+1] == Rotulo[j])
			{
				if(flag_labels[j] != 1)
				{
					for(int unsigned l=0;l<operando.size();l++)
					{																	//Além de encontrar o operando que deu erro, tem que ver se a instrução dele é a correspondente
						if(Cod[i+1] == operando[l])										//Caso contrário daria erro na linha se um operando fosse utilizado mais de uma vez 
						{
							for(int unsigned v=0;v<inst_op.size();v++)
							{
								if(Cod[i] == inst_op[v] && Cod[i+1] == operando[j] && v == j)
								{	
									nlinha = operando_ln[v];
								}
							}
						}
					}
					ind_erro = 2;									//Caso onde encontrou o rótulo, mas o salto é pra uma região inválida
					goto Interrupcao2;
				}
				achou = 1;
			}
		}
		if(achou != 1)												//Caso onde não encontrou o rótulo
		{
			for(int unsigned l=0;l<operando.size();l++)
					{
						if(Cod[i+1] == operando[l])
						{
							for(int unsigned v=0;v<inst_op.size();v++)
							{
								if(Cod[i] == inst_op[v] && Cod[i+1] == operando[l])
								{	
									nlinha = operando_ln[v];
								}
							}
						}
					}
			ind_erro = 2;
			goto Interrupcao2;										//Não encontrou o label refente ao operando
		}
		achou = 0;											//Reseta a flag;
	}
}


/////////////////////////////////////////////////////////
////Verificando se é um programa apenas ou um módulo/////
/////////////////////////////////////////////////////////

if(Cod.size() > 2)								//Para não tentar ler elementos que não existem caso o código tenha apenas 1 token
{
	if(Cod[1] == "BEGIN")						//Procura já de incío pra ver ser é módulo ou não
	{
	Modulo = 1;
	}
}

///////////////////////////////////////////////////////////
///////Verificando se todos os operando estão definidos//////
/////////////e se o CONST não está sendo alterado////////////
/////////////////////////////////////////////////////////////


			
			

for(i=0;i<Cod.size();i++)
{
	if(Cod[i] == "MULT" || Cod[i] == "DIV" || Cod[i] == "ADD" || Cod[i] == "SUB" || Cod[i] == "LOAD" || Cod[i] == "OUTPUT")
	{
		
		for(int unsigned j=0;j<Rotulo.size();j++)
		{
			if(Cod[i+1] == Rotulo[j])								//Procurando o operando na lista dos rótulos definidos
			{
				achou = 1;
			}
		}
		if(achou != 1)
		{
			for(int unsigned j=0;j<operando.size();j++)
			{
				if(operando[j] == Cod[i+1])								//Procurando o operando na lista dos operandos porque nela tem a linha onde ele pertencia
				{
					for(int unsigned v=0;v<inst_op.size();v++)
					{
						if(Cod[i] == inst_op[v] && Cod[i+1] == operando[j] && v == j)
						{	
							nlinha = operando_ln[v];
						}
					}
				}
			}
			ind_erro = 2;
			goto Interrupcao2;										//Não encontrou o label refente ao operando
		}
				
	}
	else if(Cod[i] == "STORE" || Cod[i] == "INPUT")
	{
		for(int unsigned j=0;j<Rotulo.size();j++)
		{
			if(Cod[i+1] == Rotulo[j])								//Primeiro verifica se o rótulo existe
			{
				achou = 1;
			}
		}
		if(achou != 1)
		{
			for(int unsigned j=0;j<operando.size();j++)
			{
				if(operando[j] == Cod[i+1])								//Senão estiver definido, erro e atribui a linha correspondente aquele operando
				{
					for(int unsigned v=0;v<inst_op.size();v++)
					{
						if(Cod[i] == inst_op[v] && Cod[i+1] == operando[j] && v == j)
						{	
							nlinha = operando_ln[v];
						}
					}
				}
			}
			ind_erro = 2;
			goto Interrupcao2;										//Não encontrou o label refente ao operando
		}
		for( int unsigned p=0;p<Rot_Const.size();p++)
		{
			if(Cod[i+1] == Rot_Const[p])								//Já que sabe-se que o Label foi definido, agora verifica se ele não é um CONST			
			{
				for(int unsigned j=0;j<operando.size();j++)
				{
					if(Cod[i+1] == operando[j] )								
					{
						for(int unsigned v=0;v<inst_op.size();v++)
						{
							if(Cod[i] == inst_op[v] && Cod[i+1] == operando[j] && v == j)
							{	
								nlinha = operando_ln[v];
							}
						}
					}
				}
			ind_erro = 2;
			goto Interrupcao2;	
			}
		}	
	}
	else if(Cod[i] == "COPY")
	{
		for(int unsigned j=0;j<Rotulo.size();j++)
		{
			if(Cod[i+1] == Rotulo[j])								//Procurando o operando na lista dos rótulos definidos
			{
				achou = 1;
			}
		}
		if (achou == 1)												//Se achou o primeiro, vai procurar o segundo
		{
			for(int unsigned j=0;j<Rotulo.size();j++)
			{
				if(Cod[i+2] == Rotulo[j])								//Procurando o operando na lista dos rótulos definidos
				{
					achou = 2;
				}
				else if(Cod[i+2] == "+")							//Caso onde tem soma
				{
					if(Cod[i+4] == Rotulo[j])								//Procurando o operando na lista dos rótulos definidos
					{
						achou = 2;
					}
				}
			}	
		}
		if(achou != 2)												//Quando é igual a dois é porque encontrou os dois operandos
		{
		
			for(int unsigned j=0;j<operando.size();j++)
			{
				if(Cod[i+1] == operando[j] )								//Procurando o operando na lista dos operandos porque nela tem a linha onde ele pertencia
				{
					for(int unsigned v=0;v<inst_op.size();v++)
					{
						if(Cod[i] == inst_op[v] && Cod[i+1] == operando[j] && v == j)
						{	
							nlinha = operando_ln[v];
						}
					}
				}
			}
			ind_erro = 2;
			goto Interrupcao2;										//Não encontrou o label refente ao operando
		}
		for( int unsigned p=0;p<Rot_Const.size();p++)
		{
			if(Cod[i+2] == Rot_Const[p])							//Se o segundo argumento do COPY é um dos rótulos do const, então ele está tentando modificar uma constante, erro
			{
				for(int unsigned j=0;j<operando.size();j++)
				{
					if(Cod[i+1] == operando[j] )								//Procurando o operando na lista dos operandos porque nela tem a linha onde ele pertencia
					{
						for(int unsigned v=0;v<inst_op.size();v++)
						{
							if(Cod[i] == inst_op[v] && Cod[i+1] == operando[j] && v == j)
							{	
								nlinha = operando_ln[v];
							}
						}
					}
				}
			ind_erro = 2;
			goto Interrupcao2;	
			}
		}
	}
	achou = 0;														//Resetando flag
}
			
			
////////////////////////////////////////////////
////Substituindo operandos definidos por EQU////
////////////////////////////////////////////////
			
for(i=0;i<Cod.size();i++)
	{
		for(int unsigned k=0;k<var_EQU.size();k++)
		{
			if(Cod[i] == var_EQU[k])
			{
				Cod[i] = val_EQU[k];								
			}
		}
	}	

////////////////////////////////////////////////////
////Verificando se está tentando acessar posição////
////////////////Não alocada pelo SPACE//////////////
////////////////////////////////////////////////////

for(i=0;i<Cod.size();i++)
{
	if(Cod[i] == "+")
	{
		for(unsigned int z = 0; z<Label_Space.size(); z++)
		{
			if(Cod[i-1] == Label_Space[z])
			{
				Aux13 = stoi(Cod[i+1]);
				Aux14 = stoi(Arg_Space[z]);
				if(Aux13 > Aux14)
				{
					for(int unsigned j=0;j<operando.size();j++)
					{
						if(Cod[i-1] == operando[j] )								//Procurando o operando na lista dos operandos porque nela tem a linha onde ele pertencia
						{
							for(int unsigned v=0;v<inst_op.size();v++)
							{
								if(Cod[i-2] == inst_op[v] && Cod[i-1] == operando[j] && v == j || Cod[i-3] == inst_op[v] && Cod[i-1] == operando[j] && v == j || Cod[i-4] == inst_op[v] && Cod[i-1] == operando[j] && v == j )
								{	
									nlinha = operando_ln[v];
									ind_erro = 2;
									goto Interrupcao2;
								}
							}
						}
					}
				}
			}
		}
	}
}


////////////////////////////////////////////////
/////////Verificando Divisão por zero///////////
////////////////////////////////////////////////



for(i=0;i<Cod.size();i++)
{
	if(Cod[i] == "DIV")
	{
		for(unsigned int k=0;k<Rot_Const.size();k++)
		{
			if(Cod[i+1] == Rot_Const[k] && zero[k] == 1)
			{	
				for(int unsigned j=0;j<operando.size();j++)
				{
					if(Cod[i+1] == operando[j] )								//Procurando o operando na lista dos operandos porque nela tem a linha onde ele pertencia
					{
						for(int unsigned v=0;v<inst_op.size();v++)
						{
							if(Cod[i] == inst_op[v] && Cod[i+1] == operando[j] && v == j)
							{	
								nlinha = operando_ln[v];
							}
						}
					}
				}
			ind_erro = 2;
			goto Interrupcao2;
			}
		}	
	}
}


//////////Primeira Passagem///////////////

x = 0;
for(i=0;i<Cod.size();i++)
{	
	if(Cod[i] == "CONST")
	{
		x--;
	}
	if(Cod[i] == "+")										//Se encontrar um vetor, tem que descontar a contagem
	{
		x = x-2;
	}
	size_t Achado = Cod[i].find_first_of(":");
	if(Achado != string::npos && Modulo == 0)					//Montando tabela de Símbolos encontrando os endereços dos labels e tirando eles do código
	{	
		Aux3 = Cod[i].substr(0, Achado);
		Rotulos2.push_back(Aux3);
		End_Rotulo.push_back(x);
		x--;													//Para poder pular o token contado pelo label
		goto Continuar4;
	}
	else if(Achado != string::npos && Modulo == 1)
	{
		if(Cod[i+1] == "BEGIN")
		{
			x = 0;											//Quando encontra um label apontando para BEGIN e EXTERN o contador x se mantém zerado
			goto proximo;
		}
		if(Cod[i+1] == "EXTERN")
		{
			Aux3 = Cod[i].substr(0, Achado);
			Rot_Tab_Uso.push_back(Aux3);
			x = 0;									//Assumindo que o EXTERN e o PUBLIC estarão no início
			goto proximo;
		}
		else
		{
			Aux3 = Cod[i].substr(0, Achado);
			Rotulos2.push_back(Aux3);
			End_Rotulo.push_back(x);
			x--;
		}
	}
	if(teve_public == 1)
	{
		Aux3 = Cod[i]; 
		Rot_Tab_Def.push_back(Aux3);
		End_Tab_Def.push_back(x);	
		teve_public = 0;

		goto proximo;
	}
	if(Cod[i] == "PUBLIC")
	{
		x = 0;
		teve_public = 1;
		goto proximo;
	
	}

	Continuar4:
	x++;
	proximo:
	i++;
	i--;
}


/////////Segunda Passagem///////////////


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
		Cod[i] = "0";
		if(Cod[i] != Cod.back())					//Verificando se não é o último elemento
		{
			if(!isalpha(Cod[i+1][0]))				//Preenchendo com 0's onde ficava a diretiva SPACE e onde ficava o operando
			{
				Cod[i+1] = "0";
			}
		}
		goto Recomecar;
	}
	if(Cod[i] == "CONST")
	{
		Cod.erase(Cod.begin() + i);
		Cod.erase(Cod.begin() + i - 1);
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
	Recomecar2:
	size_t Achado = Cod[i].find_first_of(":");
	if(Achado != string::npos)
	{
		Cod.erase(Cod.begin() + i);
		goto Recomecar2;
	}
}

/////////////////////////////////////////////////
//////Criando a informaçao de realocação/////////
////////////////////////////////////////////////


for(i=0;i<Cod.size();i++)										//Para isso basta verificar todos os operandos
{
	if(Cod[i] == "+")
	{
		y = y-2;
	}
	for(int unsigned j=0;j<operando.size();j++)
	{
		if(operando[j] == Cod[i])								//Procurando operando na tabela, quando encontra olha a instrução correspondente
		{
			if(inst_op[j] == "ADD")								//Verifica qual é a instrução do opcode para verificar seu opcode correspondente e assim analisar o código objeto
			{
				if(Cod[i-1] == "1")								
				{
					realocacao.push_back(y);						//Salva o endereço deste operando para a tabela de realocação
				}
			}
			if(inst_op[j] == "SUB")
			{
				if(Cod[i-1] == "2")								
				{
					realocacao.push_back(y);						//Salva o endereço deste operando para a tabela de realocação
				}
			}
			if(inst_op[j] == "MULT")
			{
				if(Cod[i-1] == "3")								
				{
					realocacao.push_back(y);						//Salva o endereço deste operando para a tabela de realocação
				}
			}
			if(inst_op[j] == "DIV")
			{
				if(Cod[i-1] == "4")								
				{
					realocacao.push_back(y);						//Salva o endereço deste operando para a tabela de realocação
				}
			}
			if(inst_op[j] == "JMP")
			{
				if(Cod[i-1] == "5")								
				{
					realocacao.push_back(y);						//Salva o endereço deste operando para a tabela de realocação
				}
			}
			if(inst_op[j] == "JMPN")
			{
				if(Cod[i-1] == "6")								
				{
					realocacao.push_back(y);						//Salva o endereço deste operando para a tabela de realocação
				}
			}
			if(inst_op[j] == "JMPP")
			{
				if(Cod[i-1] == "7")								
				{
					realocacao.push_back(y);						//Salva o endereço deste operando para a tabela de realocação
				}
			}
			if(inst_op[j] == "JMPZ")
			{
				if(Cod[i-1] == "8")								
				{
					realocacao.push_back(y);						//Salva o endereço deste operando para a tabela de realocação
				}
			}
			if(inst_op[j] == "COPY")
			{
				if(Cod[i-1] == "9")								
				{
					realocacao.push_back(y);						//Salva o endereço deste operando para a tabela de realocação
				}
			}
			if(inst_op[j] == "LOAD")
			{
				if(Cod[i-1] == "10")								
				{
					realocacao.push_back(y);						//Salva o endereço deste operando para a tabela de realocação
				}
			}
			if(inst_op[j] == "STORE")
			{
				if(Cod[i-1] == "11")								
				{
					realocacao.push_back(y);						//Salva o endereço deste operando para a tabela de realocação
				}
			}
			if(inst_op[j] == "INPUT")
			{
				if(Cod[i-1] == "12")								
				{
					realocacao.push_back(y);						//Salva o endereço deste operando para a tabela de realocação
				}
			}
			if(inst_op[j] == "OUTPUT")
			{
				if(Cod[i-1] == "13")								
				{
					realocacao.push_back(y);						//Salva o endereço deste operando para a tabela de realocação
				}
			}	
		}
	}
	y++;
}



/////////////////////////////////////////////////////////
/////Substituindo as variáveis da tabela de Uso/////////
////////////////////////////////////////////////////////

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
			Cod[i] = "0";										//Zero porque está definido em outro módulo
			Tab_Uso.push_back(Rot_Tab_Uso[j]);					//Salvando o nome da variável
			if(teve_soma == 1)									//Descontando os tokens para o caso de vetores
			{
				End_Tab_Uso.push_back(i-2);						//Salvando o local onde a variável está sendo utilizada e descontando os tokens
			}
			else
			{
				End_Tab_Uso.push_back(i);
			}
		}
	}
	
}


/////////Substituindo Variáveis pelos seus endereços///////

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



/////////////////////////////////////////////////////////
/////Atualizando os valores da Tabela de definição///////
/////////Necessario apenas se for um modulo//////////////
/////////////////////////////////////////////////////////

if(Modulo == 1)
{
	


for(i=0;i<Rotulos2.size();i++)
{
	
	for(int unsigned j=0; j<Rot_Tab_Def.size(); j++)
	{
		if(Rot_Tab_Def[j] == Rotulos2[i])						//Procura os rotulos da tabela de definição para atuliazar os endereços
		{
			End_Tab_Def[j] = End_Rotulo[i];
		}
	}
}

///////////////////////////////////////////////////////
///Substituindo os labels que trabalham com vetores////
///////////////////////////////////////////////////////

}

for(i=0;i<Cod.size();i++)
{
	if(Cod[i] == "+")
	{
		Aux7 = stoi(Cod[i+1]);							//Passando para string para somar com o endereço que está em int 
		Aux9 = stoi(Cod[i-1]);	
		Aux7 = Aux7 + Aux9;
		Cod[i-1] = to_string(Aux7);
		Cod.erase(Cod.begin() + i);						//Apagando o +
		Cod.erase(Cod.begin() + i);					//Apagando o valor somado
	}	


//
//for(i=0; i<Rotulos2.size();i++)
//{
//	cout<<endl<<endl<<"Rotulo: "<<Rotulos2[i]<<" Endereco: "<<End_Rotulo[i]<<endl;
//}
//
//
//for(i=0;i<Rot_Tab_Def.size();i++)
//{
//	cout<<endl<<endl<<" Definicao: "<<Rot_Tab_Def[i]<<" Endereco: "<<End_Tab_Def[i]<<endl;
//}
//
//for(i=0;i<Tab_Uso.size();i++)
//{
//	cout<<endl<<endl<<" Uso: "<<Tab_Uso[i]<<" Endereco: "<<End_Tab_Uso[i]<<endl;
//}
}


///////////////////////////////////////////////////////////////
/////////////////Gerando Código Objeto/////////////////////////
///////////////////////////////////////////////////////////////


if(Modulo == 1)
{
	ofstream fout;
	fout.open(Nome_Original + ".obj");
	fout<<"TABLE USE"<<endl;
	for(i=0;i<Tab_Uso.size();i++)
	{
		fout<<Tab_Uso[i]<<" "<<End_Tab_Uso[i]<<endl;
	}
	fout<<endl<<"TABLE DEFINITION"<<endl;
	for(i=0;i<Rot_Tab_Def.size();i++)
	{
		fout<<Rot_Tab_Def[i]<<" "<<End_Tab_Def[i]<<endl;
	}
	fout<<endl<<"RELATIVE"<<endl;
	for(i=0;i<realocacao.size();i++)
	{
		fout<<realocacao[i]<<" ";
	}
	fout<<endl<<endl<<"CODE"<<endl;
	for(i=0;i<Cod.size();i++)
	{
		fout<<Cod[i]<<" ";
	}			
}
else
{
			ofstream fout;
		fout.open(Nome_Original + ".obj");
		
			
		for(i=0;i<Cod.size();i++)
		{
			fout<<Cod[i]<<" ";
		}

}







/////////////////////////////////////////////////////////////////
	/////////Verificando se os Jumps não pulam para seções///////////
	///////////////////////inválidas/////////////////////////////////
	/////////////////////////////////////////////////////////////////
			

//		if(ha_rotulo == 0)
//		{
//			if(tokens[0] == "JMP" || tokens[0] == "JMPN" || tokens[0] == "JMPP" || tokens[0] == "JMPZ")
//			{
//				for(i=0; i<Rotulo.size(); i++)
//				{
//					if(Rotulo[i] == tokens[1])
//					{
//						if(flag_labels[i] == 1)
//						{
//							goto continua_3;
//						}
//					}
//				}
//				ind_erro = 1;						//Se for uma das instruções acima só continua se encontrar o rótulo e ele estiver na SECTION TEXT		
//				goto Interrupcao;
//				
//			}
//		}
//		if(teve_rotulo == 0)
//		{
//			if(tokens[1] == "JMP" || tokens[1] == "JMPN" || tokens[1] == "JMPP" || tokens[1] == "JMPZ")
//			{
//				for(i=0; i<Rotulo.size(); i++)
//				{
//					if(Rotulo[i] == tokens[2])
//					{
//						if(flag_labels[i] == 1)
//						{
//							goto continua_3;
//						}
//					}
//				}
//				ind_erro = 1;						//Se for uma das instruções acima só continua se encontrar o rótulo e ele estiver na SECTION TEXT		
//				goto Interrupcao;
//			}
//		}
//		continua_3:
//		
//		
//		///////////////////////////////////////////////
//		//////////////Divisão por zero/////////////////	
//		///////////////////////////////////////////////
//		
//		if(ha_rotulo == 0)
//		{
//			if(tokens[0] == "DIV")
//			{
//				if (tokens[1] == "0")
//				{
//					ind_erro = 0;
//					goto Interrupcao;
//				}
//				
//			}
//		}
//		

goto Final2;
Interrupcao2:
	linha_erro = to_string(nlinha);
	cout<<tipo_erro[ind_erro]<< " na linha "<<linha_erro<<endl;
	exit(0);
	
	Final2:
i++;
i--;


}


int main(int argc, char* argv[])
{
    // DeclaraÃ§Ã£o de VariÃ¡veis
    string Nome_Arquivo_Entrada;                    // arquivo de entrada .asm
    string Nome_Original;
	

    if (argc != 2)
    {
        cerr << "\n \n Numero de argumentos invalido..." << endl;
        exit(1);
    }
    
    // Argumentos de Entrada
	Nome_Original = argv[1];
	Nome_Arquivo_Entrada = Nome_Original + ".asm";              

	Montador(Nome_Arquivo_Entrada, Nome_Original);
    

    return 0;
  
}




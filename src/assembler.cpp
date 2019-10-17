#include "assembler.h"

using namespace std;

Assembler::Assembler() : error_line_()
{
    error_count_ = 0;
    n_line_ = 0;
}

Assembler::~Assembler() {}

string Assembler::erase_comments(string line)
{
    size_t Achado = line.find_first_of(";");

    if(Achado != string::npos)
    {
        line = line.substr(0, Achado);
        return(line);
    }
    else
    {
        return(line);
    }
}


string Assembler::upper_case(string line)
{   
    int restante, i = 0;
    
    restante = line.size();             //No início falta a line toda
    while (restante>0)
    {   
        line[i] = toupper(line[i]);     //Passa caracter por caracter
        i++;
        restante = line.size() - i;     //Verifica se já terminou o programa    
    }
    return(line);
}


vector<string> Assembler::get_tokens(string line)
{
    string buffer;
    stringstream ss(line);
    vector<string> tokens;
    
    while (ss >> buffer)
    {
        tokens.push_back(buffer);
    }

    return(tokens);
}


void Assembler::mount(const std::string& input_asm_file, const std::string& original_name)
{
    ofstream fout;
    // ofstream fouta;
    
    string line;
    string buf; 
    string Aux_EQU;
    string Aux;
    // string Aux_JUMP;
    string Aux3;
    string Aux4;
    string Aux5;
    string Aux6;
    // string type;
    
    //Vetores String//
    vector<string> tokens;
    vector<string> tokens_carac;
    // vector<string> completo;
    // vector<string> Inst_var;
    // vector<string> Restante;
    // vector<string> Nomes_macro;
    // vector<string> line_completa;
    // vector<string> var_equ;
    // vector<string> val_equ;
    // vector<string> final;
    vector<string> val_EQU;
    vector<string> var_EQU;
    vector<string> printa;
    vector<string> Rotulo;
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
    // vector<int> Linhas_text;
    // vector<int> Linhas_data;
    vector<int> label_line_vec;
    vector<int> flag_labels;
    vector<int> end_label;
    vector<int> End_Tab_Uso;
    vector<int> End_Tab_Def;
    vector<int> operando_ln;
    vector<int> zero;
    vector<int> realocacao;

    int x;  // não sei oq isso faz

    int unsigned i, ntokens = 0;
    int tamanho_anterior_tokens = 0, label_line = 0, Aux2, number = 0, Aux7 = 0, Aux9 = 0, y = 0;

    //FLAGS//
    int has_text = 0, before_text = 0, section_text = 0, section_data = 0;
    int inst_if = 0, inst_equ = 0, has_if = 0, n_label = 0, if_valido = 0, has_plus = 0;
    int has_label = 1, find_something = 0, section_bss = 0, had_label = 0, module1 = 0;
    int teve_public = 0, have_find = 0, module2 = 0, find_end = 0, find_stop = 0;
    int remove_line = 0, Aux13 = 0, Aux14 = 0;

    // Vetor com os 3 tipos de erro
    error_type_.push_back("Erro Lexico");
    error_type_.push_back("Erro Sintatico");
    error_type_.push_back("Erro Semantico");


    ifstream source_code(input_asm_file);
    
    if(source_code.is_open())
    {
        while(getline(source_code,line))
        {   
            remove_line = 0;                                    //Preciso para apagar o \n no pré-processamento 
            has_label = 0;                                      //Nova line, é preciso verificar novamente se há rótulo
            inst_if = 0;                                        //Nova line, é preciso verificar novamente se tem IF
            tamanho_anterior_tokens = tokens.size();            //Quantidade total de tokens antes da line atual
            line = erase_comments(line);
            line = upper_case(line);
            Pre_Cod.push_back(line);
            
            
            /////////////////////////////////////////
            ////Verificando se a line está vazia////
            /////////////////////////////////////////
            
            if(line.empty())
            {
                continue;
            }
            
            ///////////////////////////////////////
            // Separando os tokens pelos espaços //
            ///////////////////////////////////////
            
            stringstream ss(line);
            
            while (ss >> buf)
            {
                
                tokens.push_back(buf);              //Pilha com todos os tokens
            }   
            n_line_ = n_line_+1;  
            ntokens = tokens.size() - tamanho_anterior_tokens;          //Número de tokens da line
            
            
            
            /////////////////////////////////////////////////
            /////////////////Reirando os Tab's///////////////
            /////////////////////////////////////////////////
            
            char espaco[] = "   ";
            for (i = 0; i < strlen(espaco); ++i)
           {
              line.erase (remove(line.begin(), line.end(), espaco[i]), line.end());
           }
            
            
            ///////////////////////////////////////////////////////////////////////////////////////////
            size_t Achado = line.find_first_of(":");           //Verifica se tem rótulo na line atual
            ///////////////////////////////////////////////////////////////////////////////////////////
            
            ////////////////////////////////////////////////////////////
            ////////////Procurando por characteres inválidos////////////
            ////////////////////////////////////////////////////////////
            
            for(i=0;i<tokens.size();i++)
            {
                
                if (tokens[i].find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ01234567890_:+,") != string::npos)
                    {
                        error_count_ = 0;
                        goto Interrupcao;
                    }
            }
            
            
            for(i=0; i<line.size();i++)
            {
                
                if(line[i] == 0X3A )           //source_code Hexa de :
                {
                    n_label++;
                
                }
            }
            if(n_label > 1)                     //Se tem mais de um : é porque tem mais de um rótulo, então erro sintático
            {
                error_count_ = 1;
                goto Interrupcao;
            }
            
            n_label = 0;                        //Zera contador de : porque já foi verificado que não há dois rótulos na mesma line

            for(i=0; i<tokens.size(); i++)
            {
                Cod.push_back(tokens[i]);
            }
            //////////////////////////////////////
            //Verifica se há rótulos nesta line//
            ////////e salva eles em Rótulo////////
            //////////////////////////////////////
            
            
                
                if(Achado != string::npos)
                    {
                        if(had_label == 1 && n_line_ == label_line + 1)  //Se já teve um rótulo sozinho e a line seguinte está tendo um novo label, erro
                        {
                            error_count_ = 2;                           //Se tiver rótulo
                            goto Interrupcao;
                        }
                        Aux = tokens[0].substr(0, Achado);                  //Retirando o : do token do nome da variavel
                        tokens[0] = Aux;
                        if(isalpha(Aux[0]))                         //verifica se inicia com letra
                        {
                            for(i=0;i<Rotulo.size();i++)
                            {
                                if(Aux == Rotulo[i])                    //Rótulo definido novamente
                                {
                                    error_count_ = 2;
                                    goto Interrupcao;
                                }
                            }
                            Rotulo.push_back(Aux);                                              //Salvando nome do Rotulo
                            
                            label_line_vec.push_back(n_line_);                                     //Salvando line do Rotulo para tabela de Simbolos
                            has_label = 1;
                            
                            if(ntokens == 1)                            //Nesta line só tem o rótulo
                            {
                                n_line_--;
                                had_label = 1;                        //Indicar que já existe um rótulo, se houver outro na próxima line, deve dar erro
                                label_line = n_line_;                  //Linha onde teve o rótulo sozinho
                            }
                        }
                        else
                        {
                            
                                error_count_ = 0;
                                goto Interrupcao;
                        }
                    }
                    
                    if(n_line_ == label_line + 1)          //Caso na line seguinte não haja rótulo, zera a flag
                    {
                        had_label = 0;
                    }
                    
                    
            /////////////////////////////////////////////////////
            ///////Verificando se a SECTION é válida/////////////
            /////////////////////////////////////////////////////
            
            
            if(tokens[ntokens-tokens.size()] == "SECTION")                  //Se for SECTION, deve ser um dos 3 abaixo
            {
                if(tokens[ntokens-tokens.size()+1] != "TEXT" && tokens[ntokens-tokens.size()+1] != "DATA" &&  tokens[ntokens-tokens.size()+1] != "BSS")
                {
                    
                    error_count_ = 1;
                    goto Interrupcao;
                }
            }
            
            /////////////////////////////////////////////////////
            //Localizando a instruções em alguma das seções//////
            /////////////////////////////////////////////////////
            
            
                if(n_line_ == 1)                     //Caso estiver no início, índice dos vetores estava dando negativo
                {
                    if(tokens[0] != tokens.back())
                    {
                        if(tokens[1] == "BEGIN")
                        {
                            module2 = 1;
                            before_text = 1;
                            goto continuar;                     }
                        
                        if(tokens[ntokens-tokens.size()] == "SECTION" && tokens[ntokens-tokens.size()+1] == "TEXT")     //Verifica se o código começa com SECTION TEXT
                        {
                            section_text = 1;                                 //Encontrou o SECTION TEXT
                            before_text = 0;                                 //Não é preciso mais procurar por EQU, já entrou em SECTION TEXT
                            has_text = 1;                                  //Indicar que teve SECTION TEXT em algum momento
                            goto continuar;
                            
                        }
                        else if(tokens[ntokens-tokens.size()+1] == "EQU")                   //Verifica se tem EQU antes do SECTION TEXT
                            {
                                before_text = 1;     
                                inst_equ = 1;           
                                size_t Achado = tokens[0].find_first_of(":");
                                Aux_EQU = tokens[i].substr(0, Achado);                  //Retirando o : do token do nome da variavel
                                tokens[i] = Aux_EQU;                                //Salvando de volta na pilha
                                var_EQU.push_back(tokens[ntokens-tokens.size()]);                   //Salvando nome 
                                val_EQU.push_back(tokens[ntokens-tokens.size()+2]); 
                                remove_line = 1;
                                while(!Pre_Cod.empty() && Pre_Cod[i] != "\n")
                                    {
                                        Pre_Cod.pop_back();                             //Como está no início, não vai ter \n para ele parar
                                    }       
                                for(i=0; i<ntokens;i++)
                                {                                                       //Retirando a instrução EQU
                                    Cod.pop_back();
                                }
                                goto continuar;
                            }
                    }
                    else if(tokens[0] == "BEGIN")                   //Caso na line inicial tenha apenas 1 token, então pode ser um módulo sem label no BEGIN
                        {
                            before_text = 1;
                            goto continuar;
                        }
                        else
                        {
                            error_count_ = 2;
                            
                            goto Interrupcao;
                        }
                        
                        error_count_ = 2;                           //Se forem esssas instruções ou diretivas, então o tipo de erro é outro porque elas estão escritas corretamente             
                        
                        goto Interrupcao;
                }
                continuar:
                if(before_text == 1 && n_line_ > 1)                                   //Caso onde há parâmetros antes do SECTION TEXT
                {   
                    if(tokens[ntokens-tokens.size()] == "SECTION" && tokens[ntokens-tokens.size()+1] == "TEXT")     //Verifica se o código começa com SECTION TEXT
                    {
                        section_text = 1;                                 //Encontrou o SECTION TEXT
                        before_text = 0;                                 //Não é preciso mais procurar por EQU, já entrou em SECTION TEXT
                        has_text = 1;
                        goto continuar_2;
                        
                    }
                    else if(tokens[ntokens-tokens.size()+1] == "EQU")                   //Verifica se tem mais EQU's antes do SECTION TEXT
                        {           
                            while(!Pre_Cod.empty() && Pre_Cod[i] != "\n")
                            {
                                Pre_Cod.pop_back();                             //Como está no início, não vai ter \n para ele parar
                            }
                            remove_line = 1;
                            before_text = 1; 
                            inst_equ = 1;                                               
                            size_t Achado = tokens[0].find_first_of(":");
                            Aux_EQU = tokens[0].substr(0, Achado);                  //Retirando o : do token do nome da variavel
                            tokens[0] = Aux_EQU;                                //Salvando de volta na pilha
                            var_EQU.push_back(tokens[ntokens-tokens.size()]);                   //Salvando nome 
                            val_EQU.push_back(tokens[ntokens-tokens.size()+2]);
                            
                            for(i=0; i<ntokens;i++)
                            {
                                Cod.pop_back();
                            }
                            goto continuar_2;
                        
                        }
                        if(has_label == 0)
                        {
                            if(tokens[0] == "MULT" || tokens[0] == "DIV" || tokens[0] == "ADD" || tokens[0] == "SUB" || tokens[0] == "JMP" || tokens[0] == "JMPN" || tokens[0] == "JMPP" || tokens[0] == "JMPZ" || tokens[0] == "COPY" || tokens[0] == "LOAD" || tokens[0] == "STORE" || tokens[0] == "INPUT" || tokens[0] == "OUTPUT" || tokens[0] == "STOP" || tokens[0] == "IF" || tokens[0] == "PUBLIC" || tokens[0] == "EXTERN" || tokens[0] == "CONST" || tokens[0] == "SPACE" || tokens[0] == "BEGIN" || tokens[0] == "END")
                            {
                                error_count_ = 2;                           //Se forem esssas instruções ou diretivas, então o tipo de erro é outro porque elas estão escritas corretamente             
                                
                                goto Interrupcao;
                            }
                        }
                        if(had_label == 0)
                        {
                            if(tokens[1] == "MULT" || tokens[1] == "DIV" || tokens[1] == "ADD" || tokens[1] == "SUB" || tokens[1] == "JMP" || tokens[1] == "JMPN" || tokens[1] == "JMPP" || tokens[1] == "JMPZ" || tokens[1] == "COPY" || tokens[1] == "LOAD" || tokens[1] == "STORE" || tokens[1] == "INPUT" || tokens[1] == "OUTPUT" || tokens[1] == "STOP" || tokens[1] == "IF" || tokens[1] == "PUBLIC" || tokens[1] == "EXTERN" || tokens[1] == "CONST" || tokens[1] == "SPACE" || tokens[1] == "BEGIN" || tokens[1] == "END")
                            {
                                error_count_ = 2;                           //Se forem esssas instruções ou diretivas, então o tipo de erro é outro porque elas estão escritas corretamente             
                                
                                goto Interrupcao;
                            }
                        }
                        
                        else
                        {
                            error_count_ = 1;                               //Se não for nem instrução nem diretiva no lugar errado, então é sintático mesmo o erro
                            goto Interrupcao;
                        }
                }
                continuar_2:
                if ((has_text == 0 && tokens[ntokens-tokens.size()] == "SECTION" && tokens[ntokens-tokens.size()+1] == "DATA" )|| (has_text == 0 && tokens[ntokens-tokens.size()] == "SECTION" && tokens[ntokens-tokens.size()+1] == "BSS" ))
                {
                        error_count_ = 2;
                        goto Interrupcao;
                }
                else if (tokens[ntokens-tokens.size()] == "SECTION" && tokens[ntokens-tokens.size()+1] == "DATA")
                    {
                            section_text = 0;                             //Acabou a SECTION TEXT
                            section_data = 1;                             //Encontrou a SECTION DATA
                            section_bss = 0;
                    } 
                else if(tokens[ntokens-tokens.size()] == "SECTION" && tokens[ntokens-tokens.size()+1] == "BSS")
                {
                    section_text = 0;
                    section_data = 0;
                    section_bss = 1;
                }
                    
                //////////////////////////
                //////Tratando IF/////////
                //////////////////////////
                if(section_text == 1 && tokens[0] == "IF")
                {
                    inst_if = 1;                                                        //A instrução atual é um if
                    has_if = 1;                                                        //A instrução anterior foi um if
                    remove_line = 1;
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
                            if(var_EQU[p] == tokens[1])             //Se achar o nome da variável na pilha
                            {
                                tokens[1] = val_EQU[p];                         //Substitui ela pelo seu respectivo valor
                                find_something++;
                                
                            }
                        }
                        if(find_something == 0)                                 //Depois de verificar toda a pilha
                            {                                               //Senão achar o nome na pilha é porque não foi definido, então é erro semântico
                                error_count_ = 2;
                                goto Interrupcao;
                            }
                        find_something = 0;
                    
                    if(tokens[1] == "1")
                    {
                        if_valido = 1;
                    }
                }
                
                else if(section_text == 0 && tokens[0] == "IF")
                {
                    error_count_ = 2;                                       //Instrução no campo errado
                    goto Interrupcao;   
                }
                
                
                    ///////////////////////////////////////////////////////
                    ////Verificando se a line seguinte vai fazer parte////
                    ///////////////////////////////////////////////////////
                
                if(has_if == 1 && if_valido == 1 && inst_if == 0)                      //Se teve IF, mas ao mesmo tempo não é mais a instrução IF
                {
                    for(i=0; i<tokens.size();i++)
                    {
                            printa.push_back(tokens[i]);                        //Printa na tela
                            printa.push_back(" ");
                    }
                    printa.push_back("\n");
                    if_valido = 0;                                  //Próximo if ainda vai precisar ser tratado
                    has_if = 0;                                    //If já foi tratado
                }
                else if(has_if == 1 && if_valido == 0 && inst_if == 0)
                {
                    for(i=0;i<ntokens;i++)
                    {
                        Cod.pop_back();
                    }
                    remove_line = 1;
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
                    if_valido = 0;                                  //Caso o IF não seja válido, simplesmente pulará a instrução
                    has_if = 0;
                    
                }
                else if(inst_equ == 0 && inst_if == 0)      //Senão for a instrução IF ou EQU e ainda se teve IF e 
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
                inst_if = 0;                    //Reinicia as flags, senão caso encontrasse equ ou if, não printaria mais nada
                //////////////////////////////////
                ///////Reiniciando tokens/////////
                //////////////////////////////////

        //////////////////////////////////////////////////////////////
        //////////Verificando quantidade de operandos/////////////////
        //////////////////////////////////////////////////////////////
        
        ////////////////Diretiva EQU antes da TEXT////////////////////
        
            if(before_text == 1)
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
                    error_count_ = 1;                           
                    goto Interrupcao;
                }
            }
        //////Instruções e diretivas no campo TEXT////////
        
            if(section_text == 1)
            {
                if(tokens[0] == "SECTION")              //Primeiro caso onde verifica se o SECTION TEXT tem apenas esses dois tokens                            
                {
                    if(tokens[1] == "TEXT")
                    {
                        if(tokens.size() > 2)
                        {
                            
                            error_count_ = 1;                           
                            goto Interrupcao;
                        }
                        goto correto;                   
                    }
                }
                if(has_label == 0)
                {
                    if(tokens[0] == "MULT" || tokens[0] == "DIV" || tokens[0] == "ADD" || tokens[0] == "SUB" || tokens[0] == "JMP" || tokens[0] == "JMPN" || tokens[0] == "JMPP" || tokens[0] == "JMPZ" || tokens[0] == "LOAD" || tokens[0] == "STORE" || tokens[0] == "INPUT" || tokens[0] == "OUTPUT" || tokens[0] == "IF" || tokens[0] == "PUBLIC" || tokens[0] == "EXTERN" || tokens[0] == "BEGIN" || tokens[0] == "END")
                    {
                        if(tokens.size() == 2)              //Se não tiver apenas 2 tokens, só existe mais uma possibilidade
                        {   
                        
                            goto correto;
                        }
                        else if(tokens.size() == 4)
                            {
                                if(tokens[2] == "+" || tokens[2] == "-")        //Se tiver 4 tokens e o terceiro não for de + ou -, então está errado
                                {
                                    goto correto;
                                }
                            }
                                
                                error_count_ = 1;                           
                                goto Interrupcao;       
                    }
                    else if(tokens[0] == "COPY")                
                    {
                        if(tokens.size() == 3)                              //Se não tiver 3 tokens, deve ter 5 e o quarto deve ser um + ou -
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
                            error_count_ = 1;                                   
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
                    else                                        //Se não for nenhuma das opções acima, então não se conhece a instrução
                    {
                        
                        error_count_ = 1;                       
                        goto Interrupcao;
                    }
                    
                }
                else if(had_label == 0)                       //Considerando que exista rótulo nesta line é preciso garantir que ele não esteja sozinho caso contrário ele vai tentar ler tokens onde não existem e vai dar erro
                    {
                        if(tokens[1] == "MULT" || tokens[1] == "DIV" || tokens[1] == "ADD" || tokens[1] == "SUB" || tokens[1] == "JMP" || tokens[1] == "JMPN" || tokens[1] == "JMPP" || tokens[1] == "JMPZ" || tokens[1] == "LOAD" || tokens[1] == "STORE" || tokens[1] == "INPUT" || tokens[1] == "OUTPUT" || tokens[1] == "IF" || tokens[1] == "PUBLIC" || tokens[1] == "EXTERN" || tokens[1] == "BEGIN")
                        {
                            if(tokens[1] == "EXTERN" || tokens[1] == "BEGIN")           //Essas diretivas não podem ter somas por exemplo FAT: BEGIN + 3
                            {
                                if(tokens.size() == 2)
                                {
                                    goto correto;
                                }
                                else
                                {
                                
                                    error_count_ = 1;                           
                                    goto Interrupcao;   
                                }
                            }
                            if(tokens.size() == 3)              //Se não tiver apenas 3 tokens, só existe mais uma possibilidade
                            {
                                goto correto;
                            }
                            else if(tokens.size() == 5)
                                {
                                    if(tokens[4] == "+" || tokens[4] == "-")        //Se tiver 5 tokens e o terceiro não for de + ou -, então está errado
                                    {
                                        goto correto;
                                    }
                                }
                                else
                                {
                                    error_count_ = 1;                           
                                    goto Interrupcao;
                                }       
                        }
                    }
                        else if(tokens[1] == "COPY")                
                        {
                            if(tokens.size() == 4)                              //Se não tiver 4 tokens, deve ter 6 e o quinto deve ser um + ou -
                            {
                                goto correto;
                            }
                            else if(tokens.size() == 6)                         //Caso onde tiver soma em apenas um dos argumentos
                                {
                                    if(tokens[4] == "+" || tokens[3] == "+")
                                    {
                                        goto correto;
                                    }
                                }
                            else if(tokens.size() == 8)                         //Caso com dois argumentos com soma
                                {
                                    if(tokens[3] == "+" && tokens[6] == "+")
                                    {
                                        goto correto;
                                    }
                                }
                                else
                                {
                                    error_count_ = 1;                                   
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
                                error_count_ = 1;
                                goto Interrupcao;
                            }
                        }
                        else 
                        {
                            error_count_ = 1;           //Se não for nenhuma das opções acima, então não se conhece a instrução ou então é PUBLIC que não deveria ter rótulo        
                            goto Interrupcao;
                        }
                    }
                
        
        
                        
        /////////////////DIRETIVAS DA SECTION DATA//////////////////
        if(section_data == 1)
        {
            if(tokens[0] == "SECTION")              //Primeiro caso onde verifica se o SECTION TEXT tem apenas esses dois tokens                            
            {
                if(tokens[1] == "DATA")
                {
                    if(tokens.size() > 2)
                    {
                        error_count_ = 1;                           
                        goto Interrupcao;
                    }
                    goto correto;                   /////////////////////////////////////////////////////
                }                                   //////Desta maneira não há como diferenciar//////////
            }                                       ////////uma instrução no lugar errado ou uma ////////
            if(has_label == 0)                      ////////////////instrução desconhecida///////////////
            {                                       /////////////////////////////////////////////////////
                if(tokens[0] == "MULT" || tokens[0] == "DIV" || tokens[0] == "ADD" || tokens[0] == "SUB" || tokens[0] == "JMP" || tokens[0] == "JMPN" || tokens[0] == "JMPP" || tokens[0] == "JMPZ" || tokens[0] == "LOAD" || tokens[0] == "STORE" || tokens[0] == "INPUT" || tokens[0] == "OUTPUT" || tokens[0] == "IF" || tokens[0] == "PUBLIC" || tokens[0] == "EXTERN" || tokens[0] == "EQU" || tokens[0] == "STOP")
                {
                    error_count_ = 2;                           //Se forem esssas instruções ou diretivas, então o tipo de erro é outro porque elas estão escritas corretamente             
                    goto Interrupcao;
                }
                if(tokens[0] == "CONST" )   
                {
                    if(had_label == 1)
                    {
                        Aux6 = Rotulo.back();           //Se o rótulo para a diretiva CONST foi definido na line anterior, então é esse label que deve ser salvo para não ser alterado
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
                            Aux2 = stoi(tokens[1]);                 //Transformando o argumento do SPACE em int
                            for(int t=1;t<Aux2;t++)
                            {   
                                Cod.push_back("0");                 //Alocando os espaços com 0's
                            }
                        }
                        goto correto;                               //No SPACE pode ou não estar especificado a quantidade
                    }
                }
                else
                {
                error_count_ = 1;                           //Se for quaisquer outros tokens (instruções/diretivas do TEXT) ou de tamanho diferente                 
                goto Interrupcao;
                }   
            }
            
            if(had_label == 0)
            {
                if(tokens[1] == "MULT" || tokens[1] == "DIV" || tokens[1] == "ADD" || tokens[1] == "SUB" || tokens[1] == "JMP" || tokens[1] == "JMPN" || tokens[1] == "JMPP" || tokens[1] == "JMPZ" || tokens[1] == "LOAD" || tokens[1] == "STORE" || tokens[1] == "INPUT" || tokens[1] == "OUTPUT" || tokens[1] == "IF" || tokens[1] == "PUBLIC" || tokens[1] == "EXTERN" || tokens[1] == "EQU" || tokens[1] == "STOP")
                        {
                            error_count_ = 2;                           //Se forem esssas instruções ou diretivas, então o tipo de erro é outro porque elas estão escritas corretamente             
                            goto Interrupcao;
                        }
                if(tokens[1] == "CONST")    
                {
                    Aux6 = tokens[0];
                    Rot_Const.push_back(Aux6);      //Salvando o rótulo correspondente a diretiva const para que ele não seja alterado
                    if(tokens[2] == "0")
                    {
                        zero.push_back(1);          //Identificando se o CONST é zero ou não para o caso da divisão por zero
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
                        Label_Space.push_back(tokens[0]);           //Fazendo uma tabela com todos os labels dos SPACE's 
                        if(tokens.size() == 3)
                        {
                            Arg_Space.push_back(tokens[2]);         //Salvando o argumento dos SPACE's
                            Aux2 = stoi(tokens[2]);                 //Transformando o argumento do SPACE em int
                            for(int t=2;t<Aux2;t++)
                            {   
                                Cod.push_back("0");                 //Alocando os espaços com 0's
                            }
                        }
                        else
                        {
                            Arg_Space.push_back("1");               //Não ter argumento é o mesmo que ter argumento igual a 1
                        }
                        goto correto;
                    }
                }
                else
                {
                error_count_ = 1;                           
                goto Interrupcao;
                }
            }       
        }
        ////////////DIRETIVAS DA SECTION BSS////////////////
//      if(section_bss == 1)
//      {   
//          if(tokens[0] == "SECTION")              //Primeiro caso onde verifica se o SECTION TEXT tem apenas esses dois tokens                            
//          {
//              if(tokens[1] == "BSS")
//              {
//                  if(tokens.size() > 2)
//                  {   
//                      error_count_ = 1;                           
//                      goto Interrupcao;
//                  }
//                  goto correto;                   
//              }
//          }
//          if(has_label == 0)
//          {
//              if(tokens[0] == "MULT" || tokens[0] == "DIV" || tokens[0] == "ADD" || tokens[0] == "SUB" || tokens[0] == "JMP" || tokens[0] == "JMPN" || tokens[0] == "JMPP" || tokens[0] == "JMPZ" || tokens[0] == "LOAD" || tokens[0] == "STORE" || tokens[0] == "INPUT" || tokens[0] == "OUTPUT" || tokens[0] == "IF" || tokens[0] == "PUBLIC" || tokens[0] == "EXTERN" || tokens[0] == "EQU" || tokens[0] == "CONST")
//              {
//                  error_count_ = 2;                           //Se forem esssas instruções ou diretivas, então o tipo de erro é outro porque elas estão escritas corretamente             
//                  goto Interrupcao;
//              }
//              if(tokens[0] == "END")
//              {
//                  if(tokens.size() == 1)
//                  {
//                      goto correto;
//                  }
//                  else
//                  {
//                      error_count_ = 1;                                       
//                      goto Interrupcao;
//                  }
//              }
//              if(tokens[0] == "SPACE")
//              {
//                  if(tokens.size() == 1 || tokens.size() == 2)
//                  {
//                      if(tokens.size() == 2)
//                      {
//                          Aux2 = stoi(tokens[1]);                 //Transformando o argumento do SPACE em int
//                          for(int t=1;t<Aux2;t++)
//                          {   
//                              Cod.push_back("0");                 //Alocando os espaços com 0's
//                          }
//                      }
//                      goto correto;                               //No SPACE pode ou não estar especificado a quantidade
//                  }
//              }
//              else
//              {
//              error_count_ = 1;                           
//              goto Interrupcao;   
//              }
//          }
//          if(had_label == 0)
//          {
//              if(tokens[1] == "MULT" || tokens[1] == "DIV" || tokens[1] == "ADD" || tokens[1] == "SUB" || tokens[1] == "JMP" || tokens[1] == "JMPN" || tokens[1] == "JMPP" || tokens[1] == "JMPZ" || tokens[1] == "LOAD" || tokens[1] == "STORE" || tokens[1] == "INPUT" || tokens[1] == "OUTPUT" || tokens[1] == "IF" || tokens[1] == "PUBLIC" || tokens[1] == "EXTERN" || tokens[1] == "EQU" || tokens[1] == "CONST")
//              {
//                  error_count_ = 2;                           //Se forem esssas instruções ou diretivas, então o tipo de erro é outro porque elas estão escritas corretamente             
//                  goto Interrupcao;
//              }
//              if(tokens[1] == "SPACE")
//              {
//                  if(tokens.size() == 2 || tokens.size() == 3)
//                  {
//                      if(tokens.size() == 3)
//                      {
//                          Aux2 = stoi(tokens[2]);                 //Transformando o argumento do SPACE em int
//                          for(int t=2;t<Aux2;t++)
//                          {   
//                              Cod.push_back("0");                 //Alocando os espaços com 0's
//                          }
//                      }
//                      goto correto;
//                  }
//              }
//              else
//              {
//              error_count_ = 1;                           
//              goto Interrupcao;
//              }
//          }
//      }
            
            correto:
        
        
        ///////////////////////////////////////////////////////
        /////Salvando a line em que cada operando estava//////
        ///////////////////////////////////////////////////////
            
        for(i=0;i<tokens.size();i++)
        {
        
            if((tokens[i] == "MULT") || (tokens[i] == "DIV") || (tokens[i] == "ADD") || (tokens[i] == "SUB") || 
               (tokens[i] == "LOAD") || (tokens[i] == "STORE") || (tokens[i] == "INPUT") || (tokens[i] == "OUTPUT") || 
               (tokens[i] == "JMP") || (tokens[i] == "JMPN") || (tokens[i] == "JMPZ") || (tokens[i] == "JMPP"))
            {
                Aux5 = tokens[i];
                inst_op.push_back(Aux5);                        //Salvando a instrução correspondente ao operando porque tava dando as lines erradas
                Aux5 = tokens[i+1];                     
                operando.push_back(Aux5);                   //Salvando cada operando
                operando_ln.push_back(n_line_);                  //Salvando a sua respectiva line
            }
            if(tokens[i] == "COPY")
            {
                Aux5 = tokens[i+1]; 
                size_t Achado = Aux5.find_first_of(",");
                if(Achado != string::npos)
                {
                
                    Aux5 = Aux5.substr(0, Achado);                  //Retirando a vírgula dos argumentos que tem, como o COPY
                }   
                operando.push_back(Aux5);
                operando_ln.push_back(n_line_);
                Aux5 = tokens[i+2];     
                operando.push_back(Aux5);
                operando_ln.push_back(n_line_);
                Aux5 = tokens[i];
                inst_op.push_back(Aux5);
                inst_op.push_back(Aux5);                        //Duas vezes porque há duas lines e dois operandos para esta instrução
            }   
            
        }

        ///////////////////////////////////////////////////////
        //////////Localizando os rótulos no código///////////// 
        ///////////////////////////////////////////////////////
        
        if(has_label == 1)
        {   
            if(before_text == 1)
            {
                flag_labels.push_back(0);
            }
            if(section_text == 1)
            {
                flag_labels.push_back(1);
            }
            if(section_data == 1)
            {
                flag_labels.push_back(2);
            }
            if(section_bss == 1)
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
        ////Procurando END para o caso se for module2 /////
        //////////////////////////////////////////////////
        
        if(module2 == 1)
        {
            for(i=0;i<tokens.size();i++)                            //se tiver encontrado begin no início, então vai procurar em todas as lines por um END
            {
                if(tokens[i] == "END")
                {
                    find_end = find_end+1;
                }
            }
        }
        else
        {
            for(i=0;i<tokens.size();i++)                            //Se não for módulo, deve ter STOP
            {
                if(tokens[i] == "STOP")
                {
                    find_stop++;
                }
            }
        }
    goto Final;

    Interrupcao:
    error_line_ = to_string(n_line_);
    cout << error_type_[error_count_] << " na line "<< error_line_ <<endl;
    exit(0);

    

    Final:
        if(remove_line == 0)
        {
            Pre_Cod.push_back("\n");                                //Quebra de line necessária para a próxima instrução
        }
        

        while(!tokens.empty())
                {
                    tokens.pop_back();                  //Limpando a pilha para as próximas lines a serem analisadas
                }
    }


        source_code.close();
    }
    
    
        fout.open(original_name + ".pre");
        
            
        for(i=0;i<Pre_Cod.size();i++)
        {
            fout<<Pre_Cod[i]<<" ";
        }


if(module2 == 1)
{
    if(find_end != 1)              //Se for módulo e não encontrar um e apenas um END no código inteiro, dá erro
    {
        error_count_ = 2;
        if(interruption2() == -1)
            return;
    }
}
else
{
    if(find_stop != 1)             //Se for módulo e não encontrar um e apenas um END no código inteiro, dá erro
    {
        error_count_ = 2;
        if(interruption2() == -1)
            return;
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
        Aux4 = Cod[i].substr(0, Achado);                    //Retirando a vírgula dos argumentos que tem, como o COPY
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
                    {                                                                   //Além de encontrar o operando que deu erro, tem que ver se a instrução dele é a correspondente
                        if(Cod[i+1] == operando[l])                                     //Caso contrário daria erro na line se um operando fosse utilizado mais de uma vez 
                        {
                            for(int unsigned v=0;v<inst_op.size();v++)
                            {
                                if(Cod[i] == inst_op[v] && Cod[i+1] == operando[j] && v == j)
                                {   
                                    n_line_ = operando_ln[v];
                                }
                            }
                        }
                    }
                    error_count_ = 2;                                   //Caso onde encontrou o rótulo, mas o salto é pra uma região inválida
                    if(interruption2() == -1)
                        return;
                }
                have_find = 1;
            }
        }
        if(have_find != 1)                                              //Caso onde não encontrou o rótulo
        {
            for(int unsigned l=0;l<operando.size();l++)
                    {
                        if(Cod[i+1] == operando[l])
                        {
                            for(int unsigned v=0;v<inst_op.size();v++)
                            {
                                if(Cod[i] == inst_op[v] && Cod[i+1] == operando[l])
                                {   
                                    n_line_ = operando_ln[v];
                                }
                            }
                        }
                    }
            error_count_ = 2;
            if(interruption2() == -1)
            return;                                      //Não encontrou o label refente ao operando
        }
        have_find = 0;                                          //Reseta a flag;
    }
}


/////////////////////////////////////////////////////////
////Verificando se é um programa apenas ou um módulo/////
/////////////////////////////////////////////////////////

if(Cod.size() > 2)                              //Para não tentar ler elementos que não existem caso o código tenha apenas 1 token
{
    if(Cod[1] == "BEGIN")                       //Procura já de incío pra ver ser é módulo ou não
    {
    module1 = 1;
    }
}

/////////////////////////////////////////////////////////////
///////Verificando se todos os operando estão definidos//////
/////////////e se o CONST não está sendo alterado////////////
/////////////////////////////////////////////////////////////



for(i=0;i<Cod.size();i++)
{
    if(Cod[i] == "MULT" || Cod[i] == "DIV" || Cod[i] == "ADD" || Cod[i] == "SUB" || Cod[i] == "LOAD" || Cod[i] == "OUTPUT")
    {
        
        for(int unsigned j=0;j<Rotulo.size();j++)
        {
            if(Cod[i+1] == Rotulo[j])                               //Procurando o operando na lista dos rótulos definidos
            {
                have_find = 1;
            }
        }
        if(have_find != 1)
        {
            for(int unsigned j=0;j<operando.size();j++)
            {
                if(operando[j] == Cod[i+1])                             //Procurando o operando na lista dos operandos porque nela tem a line onde ele pertencia
                {
                    for(int unsigned v=0;v < inst_op.size();v++)
                    {
                        if(Cod[i] == inst_op[v] && Cod[i+1] == operando[j] && v == j)
                        {   
                            n_line_ = operando_ln[v];
                        }
                    }
                }
            }
            error_count_ = 2;
            if(interruption2() == -1)
            return;                                      //Não encontrou o label refente ao operando
        }
                
    }
    else if(Cod[i] == "STORE" || Cod[i] == "INPUT")
    {
        for(int unsigned j=0;j<Rotulo.size();j++)
        {
            if(Cod[i+1] == Rotulo[j])                               //Primeiro verifica se o rótulo existe
            {
                have_find = 1;
            }
        }
        if(have_find != 1)
        {
            for(int unsigned j=0; j < operando.size(); j++)
            {
                if(operando[j] == Cod[i+1])                             //Senão estiver definido, erro e atribui a line correspondente aquele operando
                {
                    for(int unsigned v=0;v<inst_op.size();v++)
                    {
                        if(Cod[i] == inst_op[v] && Cod[i+1] == operando[j] && v == j)
                        {   
                            n_line_ = operando_ln[v];
                        }
                    }
                }
            }
            error_count_ = 2;
            if(interruption2() == -1)
            return;                                      //Não encontrou o label refente ao operando
        }
        for( int unsigned p=0;p<Rot_Const.size();p++)
        {
            if(Cod[i+1] == Rot_Const[p])                                //Já que sabe-se que o Label foi definido, agora verifica se ele não é um CONST         
            {
                for(int unsigned j=0;j<operando.size();j++)
                {
                    if(Cod[i+1] == operando[j] )                                
                    {
                        for(int unsigned v=0;v<inst_op.size();v++)
                        {
                            if(Cod[i] == inst_op[v] && Cod[i+1] == operando[j] && v == j)
                            {   
                                n_line_ = operando_ln[v];
                            }
                        }
                    }
                }
            error_count_ = 2;
            if(interruption2() == -1)
                return;  
            }
        }   
    }
    else if(Cod[i] == "COPY")
    {
        for(int unsigned j=0;j<Rotulo.size();j++)
        {
            if(Cod[i+1] == Rotulo[j])                               //Procurando o operando na lista dos rótulos definidos
            {
                have_find = 1;
            }
        }
        if (have_find == 1)                                             //Se have_find o primeiro, vai procurar o segundo
        {
            for(int unsigned j=0;j<Rotulo.size();j++)
            {
                if(Cod[i+2] == Rotulo[j])                               //Procurando o operando na lista dos rótulos definidos
                {
                    have_find = 2;
                }
                else if(Cod[i+2] == "+")                            //Caso onde tem soma
                {
                    if(Cod[i+4] == Rotulo[j])                               //Procurando o operando na lista dos rótulos definidos
                    {
                        have_find = 2;
                    }
                }
            }   
        }
        if(have_find != 2)                                              //Quando é igual a dois é porque encontrou os dois operandos
        {
        
            for(int unsigned j=0;j<operando.size();j++)
            {
                if(Cod[i+1] == operando[j] )                                //Procurando o operando na lista dos operandos porque nela tem a line onde ele pertencia
                {
                    for(int unsigned v=0;v<inst_op.size();v++)
                    {
                        if(Cod[i] == inst_op[v] && Cod[i+1] == operando[j] && v == j)
                        {   
                            n_line_ = operando_ln[v];
                        }
                    }
                }
            }
            error_count_ = 2;
            if(interruption2() == -1)
            return;                                      //Não encontrou o label refente ao operando
        }
        for( int unsigned p=0;p<Rot_Const.size();p++)
        {
            if(Cod[i+2] == Rot_Const[p])                            //Se o segundo argumento do COPY é um dos rótulos do const, então ele está tentando modificar uma constante, erro
            {
                for(int unsigned j=0;j<operando.size();j++)
                {
                    if(Cod[i+1] == operando[j] )                                //Procurando o operando na lista dos operandos porque nela tem a line onde ele pertencia
                    {
                        for(int unsigned v=0;v<inst_op.size();v++)
                        {
                            if(Cod[i] == inst_op[v] && Cod[i+1] == operando[j] && v == j)
                            {   
                                n_line_ = operando_ln[v];
                            }
                        }
                    }
                }
            error_count_ = 2;
            if(interruption2() == -1)
                return;  
            }
        }
    }
    have_find = 0;                                                      //Resetando flag
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

for(i = 0; i < Cod.size(); i++)
{
    if(Cod[i] == "+")
    {
        for(unsigned int z = 0; z < Label_Space.size(); z++)
        {
            if(Cod[i-1] == Label_Space[z])
            {
                Aux13 = stoi(Cod[i+1]);
                Aux14 = stoi(Arg_Space[z]);
                if(Aux13 > Aux14)
                {
                    for(int unsigned j = 0; j < operando.size(); j++)
                    {
                        if(Cod[i-1] == operando[j])                                //Procurando o operando na lista dos operandos porque nela tem a line onde ele pertencia
                        {
                            for(int unsigned v = 0; v < inst_op.size(); v++)
                            {
                                if(Cod[i-2] == inst_op[v] && Cod[i-1] == operando[j] && v == j || Cod[i-3] == inst_op[v] && Cod[i-1] == operando[j] && v == j || Cod[i-4] == inst_op[v] && Cod[i-1] == operando[j] && v == j )
                                {   
                                    n_line_ = operando_ln[v];
                                    error_count_ = 2;
                                    if(interruption2() == -1)
                                        return;
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
                    if(Cod[i+1] == operando[j] )                                //Procurando o operando na lista dos operandos porque nela tem a line onde ele pertencia
                    {
                        for(int unsigned v=0;v<inst_op.size();v++)
                        {
                            if(Cod[i] == inst_op[v] && Cod[i+1] == operando[j] && v == j)
                            {
                                n_line_ = operando_ln[v];
                            }
                        }
                    }
                }
            error_count_ = 2;
            if(interruption2() == -1)
                return;
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
    if(Cod[i] == "+")                                       //Se encontrar um vetor, tem que descontar a contagem
    {
        x = x-2;
    }
    size_t Achado = Cod[i].find_first_of(":");
    if(Achado != string::npos && module1 == 0)                   //Montando tabela de Símbolos encontrando os endereços dos labels e tirando eles do código
    {   
        Aux3 = Cod[i].substr(0, Achado);
        Rotulos2.push_back(Aux3);
        end_label.push_back(x);
        x--;                                                    //Para poder pular o token contado pelo label
        goto Continuar4;
    }
    else if(Achado != string::npos && module1 == 1)
    {
        if(Cod[i+1] == "BEGIN")
        {
            x = 0;                                          //Quando encontra um label apontando para BEGIN e EXTERN o contador x se mantém zerado
            goto proximo;
        }
        if(Cod[i+1] == "EXTERN")
        {
            Aux3 = Cod[i].substr(0, Achado);
            Rot_Tab_Uso.push_back(Aux3);
            x = 0;                                  //Assumindo que o EXTERN e o PUBLIC estarão no início
            goto proximo;
        }
        else
        {
            Aux3 = Cod[i].substr(0, Achado);
            Rotulos2.push_back(Aux3);
            end_label.push_back(x);
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

for(i = 0; i < Cod.size(); i++)
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
        if(Cod[i] != Cod.back())                    //Verificando se não é o último elemento
        {
            if(!isalpha(Cod[i+1][0]))               //Preenchendo com 0's onde ficava a diretiva SPACE e onde ficava o operando
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
                number = std::stol (Cod[i],nullptr,0);
                Cod[i] = to_string(number);
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


for(i=0;i<Cod.size();i++)                                       //Para isso basta verificar todos os operandos
{
    if(Cod[i] == "+")
    {
        y = y-2;
    }
    for(int unsigned j=0;j<operando.size();j++)
    {
        if(operando[j] == Cod[i])                               //Procurando operando na tabela, quando encontra olha a instrução correspondente
        {
            if(inst_op[j] == "ADD")                             //Verifica qual é a instrução do opcode para verificar seu opcode correspondente e assim analisar o código objeto
            {
                if(Cod[i-1] == "1")                             
                {
                    realocacao.push_back(y);                        //Salva o endereço deste operando para a tabela de realocação
                }
            }
            if(inst_op[j] == "SUB")
            {
                if(Cod[i-1] == "2")                             
                {
                    realocacao.push_back(y);                        //Salva o endereço deste operando para a tabela de realocação
                }
            }
            if(inst_op[j] == "MULT")
            {
                if(Cod[i-1] == "3")                             
                {
                    realocacao.push_back(y);                        //Salva o endereço deste operando para a tabela de realocação
                }
            }
            if(inst_op[j] == "DIV")
            {
                if(Cod[i-1] == "4")                             
                {
                    realocacao.push_back(y);                        //Salva o endereço deste operando para a tabela de realocação
                }
            }
            if(inst_op[j] == "JMP")
            {
                if(Cod[i-1] == "5")                             
                {
                    realocacao.push_back(y);                        //Salva o endereço deste operando para a tabela de realocação
                }
            }
            if(inst_op[j] == "JMPN")
            {
                if(Cod[i-1] == "6")                             
                {
                    realocacao.push_back(y);                        //Salva o endereço deste operando para a tabela de realocação
                }
            }
            if(inst_op[j] == "JMPP")
            {
                if(Cod[i-1] == "7")                             
                {
                    realocacao.push_back(y);                        //Salva o endereço deste operando para a tabela de realocação
                }
            }
            if(inst_op[j] == "JMPZ")
            {
                if(Cod[i-1] == "8")                             
                {
                    realocacao.push_back(y);                        //Salva o endereço deste operando para a tabela de realocação
                }
            }
            if(inst_op[j] == "COPY")
            {
                if(Cod[i-1] == "9")                             
                {
                    realocacao.push_back(y);                        //Salva o endereço deste operando para a tabela de realocação
                }
            }
            if(inst_op[j] == "LOAD")
            {
                if(Cod[i-1] == "10")                                
                {
                    realocacao.push_back(y);                        //Salva o endereço deste operando para a tabela de realocação
                }
            }
            if(inst_op[j] == "STORE")
            {
                if(Cod[i-1] == "11")                                
                {
                    realocacao.push_back(y);                        //Salva o endereço deste operando para a tabela de realocação
                }
            }
            if(inst_op[j] == "INPUT")
            {
                if(Cod[i-1] == "12")                                
                {
                    realocacao.push_back(y);                        //Salva o endereço deste operando para a tabela de realocação
                }
            }
            if(inst_op[j] == "OUTPUT")
            {
                if(Cod[i-1] == "13")                                
                {
                    realocacao.push_back(y);                        //Salva o endereço deste operando para a tabela de realocação
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
        has_plus = 1;
    }
    for(int unsigned j=0; j<Rot_Tab_Uso.size(); j++)
    {
    
        if(Cod[i] == Rot_Tab_Uso[j])
        {
            Cod[i] = "0";                                       //Zero porque está definido em outro módulo
            Tab_Uso.push_back(Rot_Tab_Uso[j]);                  //Salvando o nome da variável
            if(has_plus == 1)                                  //Descontando os tokens para o caso de vetores
            {
                End_Tab_Uso.push_back(i-2);                     //Salvando o local onde a variável está sendo utilizada e descontando os tokens
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
            Cod[i] = to_string(end_label[j]);
        }
    }
}



/////////////////////////////////////////////////////////
/////Atualizando os valores da Tabela de definição///////
/////////Necessario apenas se for um module2//////////////
/////////////////////////////////////////////////////////

if(module1 == 1)
{
    


for(i=0;i<Rotulos2.size();i++)
{
    
    for(int unsigned j=0; j<Rot_Tab_Def.size(); j++)
    {
        if(Rot_Tab_Def[j] == Rotulos2[i])                       //Procura os rotulos da tabela de definição para atuliazar os endereços
        {
            End_Tab_Def[j] = end_label[i];
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
        Aux7 = stoi(Cod[i+1]);                          //Passando para string para somar com o endereço que está em int 
        Aux9 = stoi(Cod[i-1]);  
        Aux7 = Aux7 + Aux9;
        Cod[i-1] = to_string(Aux7);
        Cod.erase(Cod.begin() + i);                     //Apagando o +
        Cod.erase(Cod.begin() + i);                 //Apagando o valor somado
    }   


//
//for(i=0; i<Rotulos2.size();i++)
//{
//  cout<<endl<<endl<<"Rotulo: "<<Rotulos2[i]<<" Endereco: "<<end_label[i]<<endl;
//}
//
//
//for(i=0;i<Rot_Tab_Def.size();i++)
//{
//  cout<<endl<<endl<<" Definicao: "<<Rot_Tab_Def[i]<<" Endereco: "<<End_Tab_Def[i]<<endl;
//}
//
//for(i=0;i<Tab_Uso.size();i++)
//{
//  cout<<endl<<endl<<" Uso: "<<Tab_Uso[i]<<" Endereco: "<<End_Tab_Uso[i]<<endl;
//}
}


///////////////////////////////////////////////////////////////
/////////////////Gerando Código Objeto/////////////////////////
///////////////////////////////////////////////////////////////


if(module1 == 1)
{
    ofstream fout;
    fout.open(original_name + ".obj");
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
        fout.open(original_name + ".obj");
        
            
        for(i=0;i<Cod.size();i++)
        {
            fout<<Cod[i]<<" ";
        }

}







    /////////////////////////////////////////////////////////////////
    /////////Verificando se os Jumps não pulam para seções///////////
    ///////////////////////inválidas/////////////////////////////////
    /////////////////////////////////////////////////////////////////
            

//      if(has_label == 0)
//      {
//          if(tokens[0] == "JMP" || tokens[0] == "JMPN" || tokens[0] == "JMPP" || tokens[0] == "JMPZ")
//          {
//              for(i=0; i<Rotulo.size(); i++)
//              {
//                  if(Rotulo[i] == tokens[1])
//                  {
//                      if(flag_labels[i] == 1)
//                      {
//                          goto continua_3;
//                      }
//                  }
//              }
//              error_count_ = 1;                       //Se for uma das instruções acima só continua se encontrar o rótulo e ele estiver na SECTION TEXT       
//              goto Interrupcao;
//              
//          }
//      }
//      if(had_label == 0)
//      {
//          if(tokens[1] == "JMP" || tokens[1] == "JMPN" || tokens[1] == "JMPP" || tokens[1] == "JMPZ")
//          {
//              for(i=0; i<Rotulo.size(); i++)
//              {
//                  if(Rotulo[i] == tokens[2])
//                  {
//                      if(flag_labels[i] == 1)
//                      {
//                          goto continua_3;
//                      }
//                  }
//              }
//              error_count_ = 1;                       //Se for uma das instruções acima só continua se encontrar o rótulo e ele estiver na SECTION TEXT       
//              goto Interrupcao;
//          }
//      }
//      continua_3:
//      
//      
//      ///////////////////////////////////////////////
//      //////////////Divisão por zero///////////////// 
//      ///////////////////////////////////////////////
//      
//      if(has_label == 0)
//      {
//          if(tokens[0] == "DIV")
//          {
//              if (tokens[1] == "0")
//              {
//                  error_count_ = 0;
//                  goto Interrupcao;
//              }
//              
//          }
//      }
//      

goto Final2;

Final2:
i++;
i--;
}

int Assembler::interruption2()
{
    error_line_ = to_string(n_line_);
    cout << error_type_[error_count_] <<  " na line " << error_line_ << endl;
    return (-1);
}

int Assembler::interruption()
{
    error_line_ = to_string(n_line_);
    cout << error_type_[error_count_] << " na line "<< error_line_ <<endl;
    return (-1);
}
// void end2()
// {
//     i++;
//     i--;
// }


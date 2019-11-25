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
//                      if(interruption() == -1)    return;
//                  }
//                  goto correto;                   
//              }
//          }
//          if(has_label == 0)
//          {
//              if(tokens[0] == "MULT" || tokens[0] == "DIV" || tokens[0] == "ADD" || tokens[0] == "SUB" || tokens[0] == "JMP" || tokens[0] == "JMPN" || tokens[0] == "JMPP" || tokens[0] == "JMPZ" || tokens[0] == "LOAD" || tokens[0] == "STORE" || tokens[0] == "INPUT" || tokens[0] == "OUTPUT" || tokens[0] == "IF" || tokens[0] == "PUBLIC" || tokens[0] == "EXTERN" || tokens[0] == "EQU" || tokens[0] == "CONST")
//              {
//                  error_count_ = 2;                           //Se forem esssas instruções ou diretivas, então o tipo de erro é outro porque elas estão escritas corretamente             
//                  if(interruption() == -1)    return;
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
//                      if(interruption() == -1)    return;
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
//              if(interruption() == -1)    return;   
//              }
//          }
//          if(had_label == 0)
//          {
//              if(tokens[1] == "MULT" || tokens[1] == "DIV" || tokens[1] == "ADD" || tokens[1] == "SUB" || tokens[1] == "JMP" || tokens[1] == "JMPN" || tokens[1] == "JMPP" || tokens[1] == "JMPZ" || tokens[1] == "LOAD" || tokens[1] == "STORE" || tokens[1] == "INPUT" || tokens[1] == "OUTPUT" || tokens[1] == "IF" || tokens[1] == "PUBLIC" || tokens[1] == "EXTERN" || tokens[1] == "EQU" || tokens[1] == "CONST")
//              {
//                  error_count_ = 2;                           //Se forem esssas instruções ou diretivas, então o tipo de erro é outro porque elas estão escritas corretamente             
//                  if(interruption() == -1)    return;
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
//              if(interruption() == -1)    return;
//              }
//          }
//      }

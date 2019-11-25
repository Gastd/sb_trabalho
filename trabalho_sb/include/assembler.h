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


class Assembler
{
public:
    Assembler();

    void mount(const std::string& input_asm_file, const std::string& original_name);

    ~Assembler();

private:
    std::string upper_case(std::string line);
    std::string erase_comments(std::string line);
    std::vector<std::string> get_tokens(std::string line);
    int interruption2(int line, const char* file);
    int interruption(int line, const char* file);
    int loop_continue(unsigned int i);
    int preprocessor(unsigned int& i);
    void printPreCode(const std::string& Nome_Original, const std::vector<std::string>& Pre_Cod);

    std::ifstream source_code_;

    int error_count_, n_line_, before_text_, section_text_, has_text_, remove_line_;
    int inst_equ_, has_label_, had_label_, find_stop_, find_end_, module2_, section_bss_;
    int section_data_;
    unsigned int ntokens_;
    std::string error_line_, Aux_EQU_, original_name_, input_asm_file_;

    std::vector<int> flag_labels_, operando_ln_, zero_, end_label_;
    std::vector<std::string> error_type_, Pre_Cod_, Cod_, var_EQU_, val_EQU_, tokens_, inst_op_;
    std::vector<std::string> operando_, Arg_Space_, Label_Space_, Rot_Const_, label_;
};

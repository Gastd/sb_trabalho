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
    int interruption2();
    int interruption();

    int error_count_, n_line_;
    std::string error_line_;
    std::vector<std::string> error_type_;
};

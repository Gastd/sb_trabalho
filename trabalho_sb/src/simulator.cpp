/**
 * Unversidade de Brasilia
 * Instituto de Ciências Exatas
 * Departamento de Ciencia da Computaçao
 * Software Básico - 2/2019
 * 
 * Trabalho I - Simulador de Assembly Inventado
 * Porfessor: Bruno Macciavelo
 * @author Gabriel F P Araujo
 */

#include "simulator.h"

using namespace std;

SBASM::SBASM() : finish_sim_(false),
memory_filled_(false), pc(0x0),
ri(0x0), opcode(0x0) //, rs(0x0), rt(0x0),
// rd(0x0), shamt(0x0), funct(0x0),
// kte16(0x0), kte26(0x0)
{
    // Initialize memory with zeros
    int i = 0;
    for (i = 0; i < MEM_SIZE; ++i)
        mem[i] = 0;

    // Initialize registers with zeros
    for (i = 0; i < 32; ++i)
    {
        R[i] = 0;
    }

    // Init $sp and $gp
    R[28] = GLOBAL_START; // $gp
    R[29] = STACK_START;  // $sp
}

SBASM::~SBASM()
{
}

void SBASM::loadMemory(std::string text_path, std::string data_path, bool print_instructions)
{
    print_instructions_ = print_instructions;

    std::ifstream infile;
    int8_t *memory_pointer;
    char buffer[4];

    // loading .text
    infile.open(text_path, std::ios::binary|std::ios::in);
    if(infile.is_open())
    {
        memory_pointer = (int8_t *) &mem[TEXT_START >> 2];
        // printf("memory_pointer = %p\n", memory_pointer);
        while (infile.read(buffer, 4))
        {
            memory_pointer[0] = buffer[0];
            memory_pointer[1] = buffer[1];
            memory_pointer[2] = buffer[2];
            memory_pointer[3] = buffer[3];
            // printf("memory_pointer = %p with %01x %01x %01x %01x\n", 
            //     memory_pointer, buffer[0], buffer[1], buffer[2], buffer[3]);
            memory_pointer += 4;
        }
        memory_filled_ = true;
        // printf("mem[0] = %0x\n", mem[0]);
        infile.close();
    }
    else
    {
        std::cout << "Error opening text file" << std::endl;
    }

    // loading .data
    infile.open(data_path, std::ios::binary|std::ios::in);
    if(infile.is_open())
    {
        memory_pointer = (int8_t *) &mem[DATA_START >> 2];
        // printf("memory_pointer = %p\n", memory_pointer);
        while(infile.read(buffer, 4))
        {
            memory_pointer[0] = buffer[0];
            memory_pointer[1] = buffer[1];
            memory_pointer[2] = buffer[2];
            memory_pointer[3] = buffer[3];
            memory_pointer += 4;
        }
        // printf("mem[2048] = %0x\n", mem[2048]);
        infile.close();
    }
    else
    {
        std::cout << "Error opening data file" << std::endl;
    }
}

void SBASM::loadMemory(std::string text_path)
{
    std::ifstream infile;
    string line;
    vector<int> tokens;

    // loading .text
    infile.open(text_path, std::ios::binary|std::ios::in);
    if(infile.is_open())
    {
        getline(infile,line);
        string buf;
        int i = 0;
        stringstream ss(line);
        while (ss >> buf)
        {
            mem[i] = stoi(buf);
            // cout <<"i = " << i << " mem[i] = " << mem[i] << endl;
            tokens.push_back(stoi(buf));
            i++;
        } 
        memory_filled_ = true;
        // for (int i = 0; i < tokens.size(); i++)
        // {
        //     cout << tokens[i] << endl;
        // }
        // printf("mem[0] = %0x\n", mem[0]);
        infile.close();
    }
    else
    {
        std::cout << "Error opening text file" << std::endl;
    }

    pc = 0;
}

void SBASM::run()
{
    if (!memory_filled_)
    {
        std::cout << "Memory not initialized" << std::endl;
        return;
    }
    while((!finish_sim_))  /*pc <= 2k words*/
    {
        fetch();
        decode();
        execute();
    }
}

void SBASM::step()
{
    if (!memory_filled_)
    {
        std::cout << "Memory not initialized" << std::endl;
        return;
    }
    if( pc > TEXT_LIMIT)
    {
        std::cout << "PC out of range" << std::endl;
        return;
    }

    fetch();
    decode();
    execute();
}

void SBASM::fetch()
{
    // cout << "pc " << pc << std::endl;
    // cout << "mem[pc] " << mem[pc] << std::endl;
    int val;
    // cin >> val;
    ri = mem[pc];
    // pc += 4;
}

void SBASM::decode()
{
    // int16_t aux; // Auxiliary variable used to extend the signal of kte16
    opcode = ri;
    // printf("pc = %d, ri = %d\n", pc, ri);
    // rs = (ri & 0x3E00000) >> 21;
    // rt = (ri & 0x1F0000) >> 16;
    // rd = (ri & 0xF800) >> 11;
    // shamt = (ri & 0x7C0) >> 6;
    // funct = (ri & 0x3F);

    // aux = (ri & 0xFFFF);
    // kte16 = aux;
    // kte26 = (ri & 0x3FFFFFF);
/*    printf("pc = %0x, ri = %0x, hi = %0x, \
            lo = %0x, opcode = %0x, rs = %0x, \
            rt = %0x, rd = %0x, shamt = %0x, \
            funct = %0x, kte16 = %0x, kte26 = %0x\n",
            pc, ri, hi, lo,
            opcode, rs, rt,
            rd, shamt, funct,
            kte16, kte26);*/
}

void SBASM::execute()
{
    switch(opcode)
    {
        case ADD:
        {
            acc += mem[mem[pc+1]];
            cout << "ADD: Accumulador = " << acc << endl;
            pc += 2;
            break;
        }
        case SUB:
        {
            acc -= mem[mem[pc+1]];
            cout << "SUB: Accumulador = " << acc << endl;
            pc += 2;
            break;
        }
        case MUL:
        {
            acc *= mem[mem[pc+1]];
            cout << "MUL: Accumulador = " << acc << endl;
            pc += 2;
            break;
        }
        case DIV:
        {
            acc /= mem[mem[pc+1]];
            cout << "DIV: Accumulador = " << acc << endl;
            pc += 2;
            break;
        }
        case JMP:
        {
            pc = mem[pc+1];
            cout << "JMP: acc = " << acc << endl;
            cout << "JMP: pc = " << pc << endl;
            break;
        }
        case JMPN:
        {
            if (acc < 0)   pc = mem[pc+1];
            else pc += 2;
            cout << "JMPN: acc = " << acc << endl;
            cout << "JMPN: pc = " << pc << endl;
            break;
        }
        case JMPP:
        {
            if (acc > 0)   pc = mem[pc+1];
            else pc += 2;
            cout << "JMPP: acc = " << acc << endl;
            cout << "JMPP: pc = " << pc << endl;
            break;
        }
        case JMPZ:
        {
            if (acc == 0)   pc = mem[pc+1];
            else pc += 2;
            cout << "JMPZ: acc = " << acc << endl;
            cout << "JMPZ: pc = " << pc << endl;
            break;
        }
        case COPY:
        {
            mem[mem[pc+2]] = mem[mem[pc+1]];
            cout << "COPY: MEM[" << mem[pc+2] << "] = " << mem[mem[pc+2]] << endl;
            pc += 3;
            break;
        }
        case LOAD:
        {
            acc = mem[mem[pc+1]];
            cout << "LOAD: Accumulador = " << acc << endl;
            pc += 2;
            break;
        }
        case STORE:
        {
            mem[mem[pc+1]] = acc;
            cout << "STORE: MEM[" << mem[pc+1] << "] = " << acc << endl;
            pc += 2;
            break;
        }
        case INPUT:
        {
            int val;
            cout << "Favor, inserir um valor númerico" << endl;
            cin >> val;
            mem[mem[pc+1]] = val;
            cout << "INPUT: MEM[" << mem[pc+1] << "] = " << val << endl;
            pc += 2;
            break;
        }
        case OUTPUT:
        {
            cout << "O valor numerico é: " << mem[mem[pc+1]] << endl;
            pc += 2;
            break;
        }
        case STOP:
        {
            cout << "Parada de programa" << endl;
            finish_sim_ = true;
            return;
        }
    }
}

void SBASM::printExecute()
{
    if(print_instructions_)
    {
        switch(opcode)
        {
            case ADD:
            {
                // printf("")
                // break;
            }
        }
    }
}

void SBASM::dump_mem(uint16_t start, uint16_t end, char format)
{
    // uint16_t kte, i, size;
    // size = (end - start)/4;
    // for(kte = 0, i = 0; i < size; i++, kte += 4)
    // {
    //     int32_t dado = lw((start + kte), 0);
    //     if(format == 'h')
    //         printf("mem[%d] = 0x%08x\n", i, dado);
    //     else if(format == 'd')
    //         printf("mem[%d] = %d\n", i, dado);
    //     else
    //         printf("Format not supported\n");
    // }
}

void SBASM::dump_reg(char format)
{
    // if(format == 'h')
    // {
    //     for(int i = 0; i < 32; ++i)
    //         printf("R[%d] = 0x%08x\n", i, R[i]);
    //     printf("pc = 0x%08x\n", pc);
    //     printf("hi = 0x%08x\n", hi);
    //     printf("lo = 0x%08x\n", lo);
    // }
    // else if(format == 'd')
    // {
    //     for(int i = 0; i < 32; ++i)
    //         printf("R[%d] = %d\n", i, R[i]);
    //     printf("pc = %d\n", pc);
    //     printf("hi = %d\n", hi);
    //     printf("lo = %d\n", lo);
    // }
    // else
    //     printf("Format not supported\n");
}


int16_t SBASM::lw(uint16_t address, int16_t kte)
{
    if ((address + kte) % 4)
    {
        printf("Word address não múltiplo de 4\n");
        return -0x0;
    }
    uint16_t memory_address = (address + kte) / 4;
    uint16_t *point_address = &mem[memory_address];

    return point_address[0];
}

int16_t SBASM::lh(uint16_t address, int16_t kte)
{
    uint16_t memory_address = (address + kte) / 4;
    int8_t idx = (address + kte) % 4;
    if ((address + kte) % 2)
    {
        printf("Halfword address não múltiplo de 2\n");
        return -0x0;
    }
    int16_t *point_address = (int16_t *) &mem[memory_address];

    return point_address[idx / 2];
}

uint16_t SBASM::lhu(uint16_t address, int16_t kte)
{
    uint16_t memory_address = (address + kte) / 4;
    int8_t idx = (address + kte) % 4;
    if ((address + kte) % 2)
    {
        printf("Halfword address não múltiplo de 2\n");
        return -0x0;
    }
    uint16_t *point_address = (uint16_t *) &mem[memory_address];

    return 0x0000FFFF & point_address[idx / 2];
}

int16_t SBASM::lb(uint16_t address, int16_t kte)
{
    uint16_t memory_address = (address + kte) / 4;
    int8_t idx = (address + kte) % 4;
    int8_t *point_address = (int8_t *) &mem[memory_address];

    return point_address[idx];
}

int16_t SBASM::lbu(uint16_t address, int16_t kte)
{
    uint16_t memory_address = (address + kte) / 4;
    int8_t idx = (address + kte) % 4;
    int8_t *point_address = (int8_t *) &mem[memory_address];

    return 0x000000FF & point_address[idx];
}

void SBASM::sw(uint16_t address, int16_t kte, int16_t dado)
{
    uint16_t memory_address = (address + kte) / 4;
    if ((address + kte) % 4)
    {
        printf("Word address não múltiplo de 4\n");
        return;
    }
    uint16_t *point_address = &mem[memory_address];

    point_address[0] = dado;
}

void SBASM::sh(uint16_t address, int16_t kte, uint16_t dado)
{
    uint16_t memory_address = (address + kte) / 4;
    int8_t idx = (address + kte) % 4;
    if ((address + kte) % 2)
    {
        printf("Halfword address não múltiplo de 2\n");
        return;
    }
    int16_t *point_address = (int16_t *) &mem[memory_address];

    point_address[idx / 2] = dado;
}

void SBASM::sb(uint16_t address, int16_t kte, int8_t dado)
{
    uint16_t memory_address = (address + kte) / 4;
    int8_t idx = (address + kte) % 4;
    int8_t *point_address = (int8_t *) &mem[memory_address];

    point_address[idx] = dado;
}

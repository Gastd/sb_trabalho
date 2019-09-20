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

SBASM::SBASM() : finish_sim_(false),
memory_filled_(false), pc(0x0),
ri(0x0), hi(0x0), lo(0x0),
opcode(0x0) //, rs(0x0), rt(0x0),
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

void SBASM::run()
{
    if (!memory_filled_)
    {
        std::cout << "Memory not initialized" << std::endl;
        return;
    }
    while((pc <= TEXT_LIMIT) and (!finish_sim_))  /*pc <= 2k words*/
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
    ri = mem[pc >> 2];
    pc += 4;
}

void SBASM::decode()
{
    int16_t aux; // Auxiliary variable used to extend the signal of kte16
    opcode = (ri & 0xFC000000) >> 26;
    rs = (ri & 0x3E00000) >> 21;
    rt = (ri & 0x1F0000) >> 16;
    rd = (ri & 0xF800) >> 11;
    shamt = (ri & 0x7C0) >> 6;
    funct = (ri & 0x3F);

    aux = (ri & 0xFFFF);
    kte16 = aux;
    kte26 = (ri & 0x3FFFFFF);
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
        case EXT:
        {
            switch(funct)
            {
                case ADD:
                {
                    R[rd] = R[rs] + R[rt];
                    printExecute();
                    break;
                }
                case ADDU:
                {
                    R[rd] = R[rs] + R[rt];
                    printExecute();
                    break;
                }
                case SUB:
                {
                    R[rd] = R[rs] - R[rt];
                    printExecute();
                    break;
                }
                case MULT:
                {
                    int64_t mult1 = R[rs];
                    int64_t mult2 = R[rt];
                    int64_t resl = mult1 * mult2;
                    hi = (resl & 0xFFFFFFFF00000000) >> 32;
                    lo = (resl & 0x00000000FFFFFFFF);
                    printExecute();
                    break;
                }
                case DIV:
                {
                    lo = R[rs] / R[rt];
                    hi = R[rs] % R[rt];
                    printExecute();
                    break;
                }
                case AND:
                {
                    R[rd] = R[rs] & R[rt];
                    printExecute();
                    break;
                }
                case OR:
                {
                    R[rd] = R[rs] | R[rt];
                    printExecute();
                    break;
                }
                case XOR:
                {
                    R[rd] = R[rs] ^ R[rt];
                    printExecute();
                    break;
                }
                case NOR:
                {
                    R[rd] = ~(R[rs] | R[rt]);
                    printExecute();
                    break;
                }
                case SLT:
                {
                    R[rd] = (R[rs] < R[rt]) ? 1 : 0;
                    printExecute();
                    break;
                }
                case JR:
                {
                    pc = R[rs];
                    printExecute();
                    break;
                }
                case SLL:
                {
                    R[rd] = R[rt] << shamt;
                    printExecute();
                    break;
                }
                case SRL:
                {
                    R[rd] = uint32_t(R[rt]) >> shamt;
                    printExecute();
                    break;
                }
                case SRA:
                {
                    R[rd] = R[rt] >> shamt;
                    printExecute();
                    break;
                }
                case SYSCALL:
                {
                    if(R[2] == 10)
                    {
                        printf("\nSYSCALL EXIT\n");
                        finish_sim_ = true;
                    }
                    if(R[2] == 1)
                    {
                        // printf("SYSCALL PRINT INT\n");
                        printf("%d", (int32_t)R[4]);
                    }
                    if(R[2] == 4)
                    {
                        // printf("SYSCALL PRINT STRING\n");
                        // byte address => /4
                        int8_t offset = R[4] % 4;
                        int8_t *ptr = (int8_t *) &mem[R[4] >> 2];
                        printf("%s", (char *) &ptr[offset]);
                    }
                    break;
                }

                case MFHI:
                {
                    R[rd] = hi;
                    printExecute();
                    break;
                }
                case MFLO:
                {
                    R[rd] = lo;
                    printExecute();
                    break;
                }
            }
            break;
        }
        case LW:
        {
            R[rt] = lw(R[rs], kte16);
            printExecute();
            break;
        }
        case LB:
        {
            R[rt] = lb(R[rs], kte16);
            printExecute();
            break;
        }
        case LBU:
        {
            R[rt] = lbu(R[rs], kte16);
            printExecute();
            break;
        }
        case LH:
        {
            R[rt] = lh(R[rs], kte16);
            printExecute();
            break;
        }
        case LHU:
        {
            R[rt] = lhu(R[rs], kte16);
            printExecute();
            break;
        }
        case LUI:
        {
            R[rt] = (kte16 << 16);
            printExecute();
            break;
        }
        case SW:
        {
            sw(R[rs], kte16, R[rt]);
            printExecute();
            break;
        }
        case SB:
        {
            sb(R[rs], kte16, R[rt]);
            printExecute();
            break;
        }
        case SH:
        {
            sh(R[rs], kte16, R[rt]);
            printExecute();
            break;
        }
        case BEQ:
        {
            if(R[rs] == R[rt])
                pc += (kte16 << 2);
            printExecute();
            break;
        }
        case BNE:
        {
            if(R[rs] != R[rt])
                pc += (kte16 << 2);
            printExecute();
            break;
        }
        case BLEZ:
        {
            if(R[rs] <= 0)
                pc += (kte16 << 2);
            printExecute();
            break;
        }
        case BGTZ:
        {
            if(R[rs] > 0)
                pc += (kte16 << 2);
            printExecute();
            break;
        }
        case ADDI:
        {
            R[rt] = R[rs] + kte16;
            printExecute();
            break;
        }
        case SLTI:
        {
            R[rt] = (R[rs] < kte16) ? 1 : 0;
            printExecute();
            break;
        }
        case SLTIU:
        {
            R[rt] = (((uint32_t) R[rs]) < ((uint32_t) kte16)) ? 1 : 0;
            printExecute();
            break;
        }
        case ANDI:
        {
            R[rt] = R[rs] & (kte16 & 0xFFFF);
            printExecute();
            break;
        }
        case ORI:
        {
            R[rt] = R[rs] | (kte16 & 0xFFFF);
            printExecute();
            break;
        }
        case XORI:
        {
            R[rt] = R[rs] ^ (kte16 & 0xFFFF);
            printExecute();
            break;
        }
        case J:
        {
            // word adress => *4
            pc = kte26 << 2;
            printExecute();
            break;
        }
        case JAL:
        {
            R[31] = pc;
            // word adress => *4
            pc = kte26 << 2;
            printExecute();
            break;
        }
        case ADDUI:
        {
            R[rt] = R[rs] + kte16; // Looks like ADDI
            printExecute();
            break;
        }
    }
}

void SBASM::printExecute()
{
    if(print_instructions_)
    {
        switch(opcode)
        {
            case EXT:
            {
                switch(funct)
                {
                    case ADD:
                    {
                        printf("R[%d] = R[%d] + R[%d]\n", rd, rs, rt);
                        break;
                    }
                    case ADDU:
                    {
                        printf("R[%d] = R[%d] + R[%d]\n", rd, rs, rt);
                        break;
                    }
                    case SUB:
                    {
                        printf("R[%d] = R[%d] - R[%d]\n", rd, rs, rt);
                        break;
                    }
                    case MULT:
                    {
                        printf("{hi,lo} = R[%d] * R[%d]\n", rs, rt);
                        break;
                    }
                    case DIV:
                    {
                        printf("lo = R[%d] / R[%d] \nhi = R[%d] %% R[%d]\n", rs, rt, rs, rt);
                        break;
                    }
                    case AND:
                    {
                        printf("R[%d] = R[%d] & R[%d]\n", rd, rs, rt);
                        break;
                    }
                    case OR:
                    {
                        printf("R[%d] = R[%d] | R[%d]\n", rd, rs, rt);
                        break;
                    }
                    case XOR:
                    {
                        printf("R[%d] = R[%d] ^ R[%d]\n", rd, rs, rt);
                        break;
                    }
                    case NOR:
                    {
                        printf("R[%d] = not(R[%d] | R[%d])\n", rd, rs, rt);
                        break;
                    }
                    case SLT:
                    {
                        printf("R[%d] = (R[%d] < R[%d]) ? 1 : 0\n", rd, rs, rt);
                        break;
                    }
                    case JR:
                    {
                        printf("pc = R[%d]\n", rs);
                        break;
                    }
                    case SLL:
                    {
                        printf("R[%d] = R[%d] << %d\n", rd, rt, shamt);
                        break;
                    }
                    case SRL:
                    {
                        printf("R[%d] = R[%d] >> %d\n", rd, rt, shamt);
                        break;
                    }
                    case SRA:
                    {
                        printf("R[%d] = R[%d] >> %d\n", rd, rt, shamt);
                        break;
                    }
                    case MFHI:
                    {
                        printf("R[%d] = hi\n", rd);
                        break;
                    }
                    case MFLO:
                    {
                        printf("R[%d] = lo\n", rd);
                        break;
                    }
                }
                break;
            }
            case LW:
            {
                printf("memory_address = %d\n", R[rs]+kte16);
                printf("R[%d] = lw(R[%d], %d)\n", rt, rs, kte16);
                break;
            }
            case LB:
            {
                printf("memory_address = %d\n", R[rs]+kte16);
                printf("R[%d] = lb(R[%d], %d)\n", rt, rs, kte16);
                break;
            }
            case LBU:
            {
                printf("memory_address = %d\n", R[rs]+kte16);
                printf("R[%d] = lbu(R[%d], %d)\n", rt, rs, kte16);
                break;
            }
            case LH:
            {
                printf("memory_address = %d\n", R[rs]+kte16);
                printf("R[%d] = lh(R[%d], %d)\n", rt, rs, kte16);
                break;
            }
            case LHU:
            {
                printf("memory_address = %d\n", R[rs]+kte16);
                printf("R[%d] = lhu(R[%d], %d)\n", rt, rs, kte16);
                break;
            }
            case LUI:
            {
                printf("R[%d] = %d\n", rt, kte16);
                break;
            }
            case SW:
            {
                printf("memory_address = %d\n", R[rs]+kte16);
                printf("sw(R[%d], %d, R[%d])\n", rs, kte16, rt);
                break;
            }
            case SB:
            {
                printf("memory_address = %d\n", R[rs]+kte16);
                printf("sb(R[%d], %d, R[%d])\n", rs, kte16, rt);
                break;
            }
            case SH:
            {
                printf("memory_address = %d\n", R[rs]+kte16);
                printf("sh(R[%d], %d, R[%d])\n", rs, kte16, rt);
                break;
            }
            case BEQ:
            {
                printf("if(R[%d] == R[%d]) pc += (%d<<2);\n", rs, rt, kte16);
                break;
            }
            case BNE:
            {
                printf("if(R[%d] != R[%d]) pc += (%d<<2);\n", rs, rt, kte16);
                break;
            }
            case BLEZ:
            {
                printf("if(R[%d] <= 0) pc += (%d<<2);\n", rs, kte16);
                break;
            }
            case BGTZ:
            {
                printf("if(R[%d] >= 0) pc += (%d<<2);\n", rs, kte16);
                break;
            }
            case ADDI:
            {
                printf("R[%d] = (R[%d] + %d)\n", rt, rs, kte16);
                break;
            }
            case SLTI:
            {
                printf("R[%d] = (R[%d] < %d) ? 1 : 0\n", rt, rs, kte16);
                break;
            }
            case SLTIU:
            {
                printf("R[%d] = (R[%d] < %d) ? 1 : 0\n", rt, rs, (kte16 & 0xFFFF));
                break;
            }
            case ANDI:
            {
                printf("R[%d] = R[%d] & %d\n", rt, rs, (kte16&0xFFFF));
                break;
            }
            case ORI:
            {
                printf("R[%d] = R[%d] | %d\n", rt, rs, (kte16&0xFFFF));
                break;
            }
            case XORI:
            {
                printf("R[%d] = R[%d] ^ %d\n", rt, rs, (kte16&0xFFFF));
                break;
            }
            case J:
            {
                printf("J %d\n", kte26<<2);
                break;
            }
            case JAL:
            {
                printf("JAL %d\n", kte26<<2);
                break;
            }
            case ADDUI:
            {
                printf("R[%d] = R[%d] + %d\n", rt, rs, kte16);
                break;
            }
        }
    }
}

void SBASM::dump_mem(uint32_t start, uint32_t end, char format)
{
    uint32_t kte, i, size;
    size = (end - start)/4;
    for(kte = 0, i = 0; i < size; i++, kte += 4)
    {
        int32_t dado = lw((start + kte), 0);
        if(format == 'h')
            printf("mem[%d] = 0x%08x\n", i, dado);
        else if(format == 'd')
            printf("mem[%d] = %d\n", i, dado);
        else
            printf("Format not supported\n");
    }
}

void SBASM::dump_reg(char format)
{
    if(format == 'h')
    {
        for(int i = 0; i < 32; ++i)
            printf("R[%d] = 0x%08x\n", i, R[i]);
        printf("pc = 0x%08x\n", pc);
        printf("hi = 0x%08x\n", hi);
        printf("lo = 0x%08x\n", lo);
    }
    else if(format == 'd')
    {
        for(int i = 0; i < 32; ++i)
            printf("R[%d] = %d\n", i, R[i]);
        printf("pc = %d\n", pc);
        printf("hi = %d\n", hi);
        printf("lo = %d\n", lo);
    }
    else
        printf("Format not supported\n");
}


int32_t SBASM::lw(uint32_t address, int16_t kte)
{
    if ((address + kte) % 4)
    {
        printf("Word address não múltiplo de 4\n");
        return -0x0;
    }
    uint32_t memory_address = (address + kte) / 4;
    uint32_t *point_address = &mem[memory_address];

    return point_address[0];
}

int32_t SBASM::lh(uint32_t address, int16_t kte)
{
    uint32_t memory_address = (address + kte) / 4;
    int8_t idx = (address + kte) % 4;
    if ((address + kte) % 2)
    {
        printf("Halfword address não múltiplo de 2\n");
        return -0x0;
    }
    int16_t *point_address = (int16_t *) &mem[memory_address];

    return point_address[idx / 2];
}

uint32_t SBASM::lhu(uint32_t address, int16_t kte)
{
    uint32_t memory_address = (address + kte) / 4;
    int8_t idx = (address + kte) % 4;
    if ((address + kte) % 2)
    {
        printf("Halfword address não múltiplo de 2\n");
        return -0x0;
    }
    uint16_t *point_address = (uint16_t *) &mem[memory_address];

    return 0x0000FFFF & point_address[idx / 2];
}

int32_t SBASM::lb(uint32_t address, int16_t kte)
{
    uint32_t memory_address = (address + kte) / 4;
    int8_t idx = (address + kte) % 4;
    int8_t *point_address = (int8_t *) &mem[memory_address];

    return point_address[idx];
}

int32_t SBASM::lbu(uint32_t address, int16_t kte)
{
    uint32_t memory_address = (address + kte) / 4;
    int8_t idx = (address + kte) % 4;
    int8_t *point_address = (int8_t *) &mem[memory_address];

    return 0x000000FF & point_address[idx];
}

void SBASM::sw(uint32_t address, int16_t kte, int32_t dado)
{
    uint32_t memory_address = (address + kte) / 4;
    if ((address + kte) % 4)
    {
        printf("Word address não múltiplo de 4\n");
        return;
    }
    uint32_t *point_address = &mem[memory_address];

    point_address[0] = dado;
}

void SBASM::sh(uint32_t address, int16_t kte, uint16_t dado)
{
    uint32_t memory_address = (address + kte) / 4;
    int8_t idx = (address + kte) % 4;
    if ((address + kte) % 2)
    {
        printf("Halfword address não múltiplo de 2\n");
        return;
    }
    int16_t *point_address = (int16_t *) &mem[memory_address];

    point_address[idx / 2] = dado;
}

void SBASM::sb(uint32_t address, int16_t kte, int8_t dado)
{
    uint32_t memory_address = (address + kte) / 4;
    int8_t idx = (address + kte) % 4;
    int8_t *point_address = (int8_t *) &mem[memory_address];

    point_address[idx] = dado;
}

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

#ifndef SBASM_SIMULATOR_
#define SBASM_SIMULATOR_

#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <string>

#define MEM_SIZE        4096   // Simulated memory's size
#define TEXT_START      0x0000 // .text base address
#define TEXT_LIMIT      0x1FFF // .text limit address
#define DATA_START      0x2000 // .data base address
#define GLOBAL_START    0x1800 // global pointer $gp
#define STACK_START     0x3FFC // stack base address, stack pointer $sp

class SBASM
{
public:
    SBASM();

    void run();
    void loadMemory(std::string, std::string, bool print_instructions);
    void loadMemory(std::string);
    void step();
    // Aux function
    void dump_mem(uint16_t start, uint16_t end, char format);
    void dump_reg(char format);

    ~SBASM();
    
private:
    // void openFile(std::string);
    
    uint16_t mem[MEM_SIZE];  // Memory array
    bool finish_sim_, print_instructions_, memory_filled_;
    uint16_t pc, ri; // hi, lo;
    int16_t acc;    // Accumulator
    // fields of R, I and J formats
    int16_t opcode;//, rs, rt, rd, shamt, funct, kte16;
    // uint16_t kte26;

    void printExecute();

    // simulating SBASM
    void fetch();
    void decode();
    void execute();

    // Registers
    int16_t R[32];  // Registers array

    enum OPCODES
    {
        ADD=1,    SUB=2,    MUL=3,    DIV=4,
        JMP=5,    JMPN=6,   JMPP=7,   JMPZ=8,
        COPY=9,   LOAD=10,  STORE=11, INPUT=12,
        OUTPUT=13,STOP=14
    };

    // enum FUNCT
    // {
    //     ADD=0x20,   SUB=0x22,   MULT=0x18,  DIV=0x1A,   AND=0x24,
    //     OR=0x25,    XOR=0x26,   NOR=0x27,   SLT=0x2A,   JR=0x08,
    //     SLL=0x00,   SRL=0x02,   SRA=0x03,   SYSCALL=0x0C,
    //     MFHI=0x10,  MFLO=0x12,  ADDU=0x21
    // };


    // simulated memory access instructions
    int16_t lw(uint16_t address, int16_t kte);              // Simulated instruction lw, load word
    int16_t lh(uint16_t address, int16_t kte);              // Simulated instruction lh, load halfword
    uint16_t lhu(uint16_t address, int16_t kte);            // Simulated instruction lhu, load unsigned halfword
    int16_t lb(uint16_t address, int16_t kte);              // Simulated instruction lb, load byte
    int16_t lbu(uint16_t address, int16_t kte);             // Simulated instruction lbu, load unsigned byte
    void sw(uint16_t address, int16_t kte, int16_t dado);   // Simulated instruction sw, store word
    void sh(uint16_t address, int16_t kte, uint16_t dado);  // Simulated instruction sh, store halfword
    void sb(uint16_t address, int16_t kte, int8_t dado);    // Simulated instruction sb, load byte
};

#endif

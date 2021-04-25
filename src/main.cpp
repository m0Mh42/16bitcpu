//
// 16 bit cpu emulation
//

#include <iostream>
#include <cstring>
#include <sstream>
#include <cctype>
#include "file.hpp"

#define ip reg[IP]
#define sp reg[SP]

using namespace std;

typedef enum {
    NOP = 0,
    LDA,
    LDB,
    LDC,
    LDD,
    PSA,
    PSB,
    PSC,
    PSD,
    HLT
} InstructionSet;

enum InstructionCode {
    NOP_S = 0,
    LDA_S,
    LDB_S,
    LDC_S,
    LDD_S,
    PSA_S,
    PSB_S,
    PSC_S,
    PSD_S,
    HLT_S,
    INV_S = 0xFFFF
};

typedef enum {
    A, B, C, D,
    IP, LOS,
    REG_C
} Registers; 

enum RegistersCode {
    A_S = 0, B_S, C_S, D_S,
    IP_S, LOS_S,
    INV_RS = 0xFFFF
};

unsigned short int ram[1024 * 32]; // from 0x0000 to 0x3FFF is program
                                   // from 0x4000 to 0x7FFF is memory
unsigned short int reg[REG_C];

bool run = true;

InstructionCode instrhash(string const& instr){
    if (instr == "NOP") return NOP_S;
    if (instr == "LDA") return LDA_S;
    if (instr == "LDB") return LDB_S;
    if (instr == "LDC") return LDC_S;
    if (instr == "LDD") return LDD_S;
    if (instr == "PSA") return PSA_S;
    if (instr == "PSB") return PSB_S;
    if (instr == "PSC") return PSC_S;
    if (instr == "PSD") return PSD_S;
    if (instr == "HLT") return HLT_S;
    return INV_S;
};

RegistersCode reghash(string const& reg){
    if (reg == "A") return A_S;
    if (reg == "B") return B_S;
    if (reg == "C") return C_S;
    if (reg == "D") return D_S;
    return INV_RS;
};

bool IsDigit(string const& input){
    for (char const& c : input){
        if (isdigit(c) == 0) return false;
    }
    return true;
}

bool IsHex(string const& input){
    string xnum;
    for (int i = 0; i < 4; i++){
        xnum.push_back(input[i + 2]);
    }
    for (char const& c : xnum){
        if (isxdigit(c) == 0) return false;
    }
    return true;
}

void CompileErr(int instr){
    cout << "Error in Program" << endl;
    cout << "Instruction " << instr << endl;
    exit (255);
}

void Compile(string* program, int& program_length){
    ip = 0xFFFF;
    for (int i = 0; i < program_length; i++){
            string instr = *(program + i); 
            if (IsDigit(instr)){
                stringstream num(instr);
                short int unum;
                num >> unum;
                ram[++ip] = unum;
                continue;
            } 
            if (IsHex(instr)){
                unsigned short int num;
                num = stoi(instr, 0, 16);
                ram[++ip] = num;
            } 
            else if (instrhash(instr) != INV_S){
                switch(instrhash(instr)){
                    case NOP_S:
                        ram[++ip] = NOP;
                        break;
                    case LDA_S:
                        if (IsDigit(*(program + i + 1)) || IsHex(*(program + i + 1))){
                            ram[++ip] = LDA;
                            break;
                        } else {
                            CompileErr(i);
                        }
                    case LDB_S:
                        if (IsDigit(*(program + i + 1)) || IsHex(*(program + i + 1))){
                            ram[++ip] = LDB;
                            break;
                        } else {
                            CompileErr(i);
                            break;
                        }
                    case LDC_S:
                        if (IsDigit(*(program + i + 1)) || IsHex(*(program + i + 1))){
                            ram[++ip] = LDC;
                            break;
                        } else {
                            CompileErr(i);
                        }
                        break;
                    case LDD_S:
                        if (IsDigit(*(program + i + 1)) || IsHex(*(program + i + 1))){
                            ram[++ip] = LDD;
                            break;
                        } else {
                            CompileErr(i);
                        }
                        break;
                    case PSA_S:
                        if (IsDigit(*(program + i + 1)) || IsHex(*(program + i + 1))){
                            ram[++ip] = PSA;
                            break;
                        } else {
                            CompileErr(i);
                        }
                    case PSB_S:
                        if (IsDigit(*(program + i + 1)) || IsHex(*(program + i + 1))){
                            ram[++ip] = PSB;
                            break;
                        } else {
                            CompileErr(i);
                        }
                    case PSC_S:
                        if (IsDigit(*(program + i + 1)) || IsHex(*(program + i + 1))){
                            ram[++ip] = PSC;
                            break;
                        } else {
                            CompileErr(i);
                        }
                    case PSD_S:
                        if (IsDigit(*(program + i + 1)) || IsHex(*(program + i + 1))){
                            ram[++ip] = PSD;
                            break;
                        } else {
                            CompileErr(i);
                        }
                    case HLT_S:
                        ram[++ip] = HLT;
                        break;
                    case INV_S:
                        break;
                }
            }
        }
}

void eval(short int instr){
    switch(instr){
        case NOP:
            break;
        case LDA:
            reg[A] = ram[++ip];
            break;
        case LDB:
            reg[B] = ram[++ip];
            break;
        case LDC:
            reg[C] = ram[++ip];
            break;
        case LDD:
            reg[D] = ram[++ip];
            break;
        case PSA: {
            unsigned short int addr = ram[++ip];
            ram[addr] = reg[A];
            break;
                  }
        case PSB: {
            unsigned short int addr = ram[++ip];
            ram[addr] = reg[B];
            break;
                  }
        case PSC: {
            unsigned short int addr = ram[++ip];
            ram[addr] = reg[C];
            break;
                  }
        case PSD: {
            unsigned short int addr = ram[++ip];
            ram[addr] = reg[D];
            break;
                  }
        case HLT:
            run = false;
            cout << "HLT" << endl;
    }
}

short int fetch(){
    return ram[ip];
}

void execute(){
    eval(fetch());
    ip++;
}

int main(int argc, char* argv[]){
    if (argc < 2){
        cout << argv[0] << " [program]";
        exit(126);
    }

    string* program;
    
    File file_o;
    program = file_o.Read(argv[1]);

    Compile(program, file_o.program_length);

    ip = 0x0000;
    while (run){
        eval(fetch());
        ip++;
    }

    cout << "0x4000 " << ram[0x4000] << endl;
    cout << "0x4001 " << ram[0x4001] << endl;
    cout << "0x4002 " << ram[0x4002] << endl;
    cout << "0x4003 " << ram[0x4003] << endl;

    return 0;
}


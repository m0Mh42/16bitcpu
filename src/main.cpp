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
    LDA, // 1
    LDB, // 2
    LDC, // 3
    LDD, // 4
    PSA, // 5
    PSB, // 6
    PSC, // 7
    PSD, // 8
    JMP, // 9
    LMA, // 10
    LMB, // 11
    LMC, // 12
    LMD, // 13
    HLT  // 14
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
    JMP_S,
    LMA_S,
    LMB_S,
    LMC_S,
    LMD_S,
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

bool debug = false;

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
    if (instr == "JMP") return JMP_S;
    if (instr == "LMA") return LMA_S;
    if (instr == "LMB") return LMB_S;
    if (instr == "LMC") return LMC_S;
    if (instr == "LMD") return LMD_S;
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

void CheckNumSyntax(string* program, int i, InstructionSet instr){
    if (IsDigit(*(program + i + 1)) || IsHex(*(program + i + 1))){
        ram[++ip] = instr;
    } else {
        CompileErr(i);
    }
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
                        CheckNumSyntax(program, i, LDA);
                        break;
                    case LDB_S:
                        CheckNumSyntax(program, i, LDB);
                        break;
                    case LDC_S:
                        CheckNumSyntax(program, i, LDC);
                        break;
                    case LDD_S:
                        CheckNumSyntax(program, i, LDD);
                        break;
                    case PSA_S:
                        CheckNumSyntax(program, i, PSA);
                        break;
                    case PSB_S:
                        CheckNumSyntax(program, i, PSB);
                        break;
                    case PSC_S:
                        CheckNumSyntax(program, i, PSC);
                        break;
                    case PSD_S:
                        CheckNumSyntax(program, i, PSD);
                        break;
                    case JMP_S:
                        CheckNumSyntax(program, i, JMP);
                        break;
                    case LMA_S:
                        CheckNumSyntax(program, i, LMA);
                        break;
                    case LMB_S:
                        CheckNumSyntax(program, i, LMB);
                        break;
                    case LMC_S:
                        CheckNumSyntax(program, i, LMC);
                        break;
                    case LMD_S:
                        CheckNumSyntax(program, i, LMD);
                        break;
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
            if (debug){
                cout << "PUSH " << reg[A] << " TO REG A" << endl;
            }
            break;
        case LDB:
            reg[B] = ram[++ip];
            if (debug){
                cout << "PUSH " << reg[B] << " TO REG B" << endl;
            }
            break;
        case LDC:
            reg[C] = ram[++ip];
            if (debug){
                cout << "PUSH " << reg[C] << " TO REG C" << endl;
            }
            break;
        case LDD:
            reg[D] = ram[++ip];
            if (debug){
                cout << "PUSH " << reg[D] << " TO REG D" << endl;
            }
            break;
        case PSA: {
            unsigned short int addr = ram[++ip];
            ram[addr] = reg[A];
            if (debug){
                cout << "PUSH " << reg[A] << " TO ADDR " << addr << endl;
            }
            break;
                  }
        case PSB: {
            unsigned short int addr = ram[++ip];
            ram[addr] = reg[B];
            if (debug){
                cout << "PUSH " << reg[B] << " TO ADDR " << addr << endl;
            }
            break;
                  }
        case PSC: {
            unsigned short int addr = ram[++ip];
            ram[addr] = reg[C];
            if (debug){
                cout << "PUSH " << reg[C] << " TO ADDR " << addr << endl;
            }
            break;
                  }
        case PSD: {
            unsigned short int addr = ram[++ip];
            ram[addr] = reg[D];
            if (debug){
                cout << "PUSH " << reg[D] << " TO ADDR " << addr << endl;
            }
            break;
                  }
        case JMP: {
            unsigned short int addr = ram[++ip];
            ip = addr;
            if (debug){
                cout << "JUMP TO ADDR " << addr << endl;
            }
            break;
                  }
        case LMA: {
            unsigned short int addr = ram[++ip];
            reg[A] = ram[addr];
            if (debug){
                cout << "PUSH " << reg[A] << " TO REG A" << endl;
            }
            break;
                  }
        case LMB: {
            unsigned short int addr = ram[++ip];
            reg[B] = ram[addr];
            if (debug){
                cout << "PUSH " << reg[B] << " TO REG B" << endl;
            }
            break;
                  }
        case LMC: {
            unsigned short int addr = ram[++ip];
            reg[C] = ram[addr];
            if (debug){
                cout << "PUSH " << reg[C] << " TO REG C" << endl;
            }
            break;
                  }
        case LMD: {
            unsigned short int addr = ram[++ip];
            reg[D] = ram[addr];
            if (debug){
                cout << "PUSH " << reg[D] << " TO REG D" << endl;
            }
            break;
                  }
        case HLT:
            run = false;
            cout << "HLT" << endl;
            cout << "IP: " << ip << endl;
    }
}

short int fetch(){
    return ram[ip];
}

void execute(){
    eval(fetch());
    ip++;
}

void CheckIP(){
    if (ip >= 0x7FFF){
        cout << "Program Leak." << endl;
        run = false;
    }
}

int main(int argc, char* argv[]){
    if (argc < 2){
        cout << argv[0] << " [program]";
        exit(126);
    }
    cout << "ARGC: " << argc << endl;
    if (argc > 2 && strcmp(argv[2], "-debug") == 0){
        // debug
        cout << "DEBUG MODE ON" << endl;
        debug = true;
    }

    string* program;

    File file_o;
    program = file_o.Read(argv[1]);

    Compile(program, file_o.program_length);

    ip = 0;
    while (run){
        eval(fetch());
        ip++;
        CheckIP();
    }

    cout << "Exiting..." << endl;

    return 0;
}


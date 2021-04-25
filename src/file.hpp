#include <iostream>
#include <fstream>

using namespace std;

class File {
    public:
        string* Read(string filename);
        int program_length;
    private:
        ifstream Openfile(string filename);
};

inline string* File::Read(string filename){
    ifstream file = Openfile(filename);
    string instr;
    int ip = -1;
    string* program = new string[1024 * 16];
    if (program == nullptr){
        cout << "Memory Error.\n";
        exit(128);
    }
    while (file >> instr){
        ip++;
        *(program + ip) = instr;
        if (ip == 1024 * 16 - 1){
            cout << "Program Limit Reached.\n";
            cout << "Only Executing First 16484 Instructions." << endl;
            break;
        }
    }
    program_length = ip + 1;
    return program;
}

inline ifstream File::Openfile(string filename){
    ifstream file;
    file.open(filename, ios::in | ios::binary);
    return file;
}


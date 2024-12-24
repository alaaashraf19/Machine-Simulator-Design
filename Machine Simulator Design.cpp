#include <iostream>
#include<vector>
#include<string>
#include<map>
#include <bits/stdc++.h>
#include<sstream>
#include<fstream>
#include<iomanip>

using namespace std;

class machine {
protected:
    int size;
    vector<string> data;
public:
    machine(int _size) : size(_size), data(_size, "00") {}

    void load() {
        fstream file;
        file.open("inp.txt", ios::in);
        if (file.is_open()) {
            string line;
            data.clear();
            while (getline(file, line)) {
//            cout<< line<<endl;
                while (!(line.empty())) {
                    string x;
                    for (int i = 0; i < 2; ++i) {
                        size_t index = line.find("x") + 1;
                        x += line[index];
                        line.erase(0, index + 1);
                        if (line[0] == ' ')
                            line = line.substr(1);
                    }
                    data.push_back(x);
                }
            }
            file.close();
        } else {
            cout << "error while opening the file";
        }
    }

    string read(int address) { // read value of address
        if (address >= 0 && address < 256) {
            return data[address];
        } else {
            return "error";
        }
    }

    void write(int address, const string &value) { // to modify value of address
        if (address >= 0 && address < size) {
            data[address] = value;
        } else {
            cout << "error";
        }
    }


};

class memory : public machine {
protected:
    map<int, string> mem;

public:
    memory(int _size) : machine(_size) {
        for (int i = 0; i < 256; ++i) {
            mem[i] = "00";
        }
    }

    void load_memory() {
        load();
        for (int i = 0; i < data.size(); ++i) {
            mem[i] = data[i];
        }
    }

    void print_memory() {
        for (auto &it: mem) {
            cout << "memory address: " << hex << it.first << " value: " << it.second << endl;
        }
    }

    void set_mem(const string &address, const string &val) {
        int dec = stoi(address, nullptr, 10);
        mem[dec] = val;
    }

    void get_mem(const string &address) {
        int dec = stoi(address, nullptr, 10);
        cout << "The value in the memory address " << address << " is: " << mem[dec] << '\n';
    }

    string get_m(int address) {
        return mem[address];
    }
};


class Central : public memory {
    memory *mem;
    string registers[16];
    int pc;
    string ir;

public:
    Central(memory *n, int _size) : mem(n), memory(_size), pc(0) {
        for (int i = 0; i < 16; ++i) {
            registers[i] = "00";
        }
    }

    void fetch() {
        while (pc < data.size()) {
            // Fetch the instruction pointed by the program counter
            string instruction1 = mem->get_m(pc);
            pc++;
            string instruction2 = mem->get_m(pc);
            pc++;
            ir = instruction1 + instruction2;
            // Decode the instruction
            decode(ir);
        }
    }


    void decode(const string &instruction) {

        int opcode = stoi(instruction.substr(0, 1), nullptr, 16);
        int reg = stoi(instruction.substr(1, 1), nullptr, 16);
        int operands = stoi(instruction.substr(2), nullptr, 16);
        execute(reg, opcode, operands);
        return;
    }

    void execute(int regs, int opcode, int operands) {
        if (opcode == 1) {
            registers[regs] = mem->get_m(operands);
            cout << "Loaded value from memory  " << hex << operands << " to register: " << hex << regs << endl;
        } else if (opcode == 2) {
            registers[regs] = (convert_to_hex(operands));
            cout << "Loaded address bit pattern from memory to register number" << regs << ": " << hex << operands
                 << endl;
        } else if (opcode == 3) {
            if (operands != 0) {
                mem->set_mem(to_string(operands), registers[regs]);
                cout << "the value in register " << regs << " will be put in the memory address" << operands << endl;
            } else {
                string str = registers[regs];
                if (str.length() == 1) {
                    str.insert(0, "0");
                }
                cout << "the value in register is: " << str << endl;
            }
        } else if (opcode == 4) { //40RS copy content of register R in Register S
            string neue_operands = convert_to_hex(operands);
            string dest, src;
            dest += neue_operands[1];
            src += neue_operands[0];
            int x, y;
            x = stoi(dest, nullptr, 16);
            y = stoi(src, nullptr, 16);
            registers[x] = registers[y];
        } else if (opcode == 5) {
            string neue_operands = convert_to_hex(operands);
            string dest, src;
            dest += neue_operands[1];
            src += neue_operands[0];
            int x, y;
            x = stoi(dest, nullptr, 16);
            y = stoi(src, nullptr, 16);
            int temp = stoi(registers[x], nullptr, 16) + stoi(registers[y], nullptr, 16);
            registers[regs] = convert_to_hex(temp);
        } else if (opcode == 11) {
            if (registers[regs] == registers[0]) {
                pc = operands;
            } else
                return;
        } else if (opcode == 12) {
            pc = data.size();
        }
    }

    void print_reg() {
        string z;
        for (int i = 0; i < 16; ++i) {
            z = registers[i];
            if (registers[i].length() == 1) {
                z.insert(0, "0");
            }
            cout << hex << z << '\n';
        }
    }
    void set_reg(const string &address, const string &val) {
        int temp = stoi(address, nullptr, 10);
        registers[temp] = val;
    }
    void get_reg(const string &address) {
        int temp = stoi(address, nullptr, 10);
        cout << "The value in the register address " << address << " is: " << registers[temp]<<'\n';
        }
    string convert_to_hex(int dec) {
        stringstream ss;
        ss << hex << dec;
        string x = ss.str();
        if (dec <= 15)
            x = "0" + x;
        return x;
    }

};
int main() {
    machine m1(1);
    m1.load();
    memory x(256);
    x.load_memory();

    int n;
    do {
        cout << "====================\n";
        cout << "choose what you want: \n"
             << "1->print memory before machine cycle\n"
             << "2->print memory after machine cycle\n"
             << "3->print register\n"
             << "4->set a specific value to an address\n"
             << "5->print a specific value of a memory address\n"
             << "6->get value of an address\n"
             << "0->exit\n"
             << ": ";
        cin >> n;

        cout << "====================\n";
        if (n == 1) {
            x.print_memory();
            cout << "=================\n";
        } else if (n == 4) {
            string v, v1;
            cout << "enter string address of memory: \n";
            cin >> v;
            cout << "enter value of this address: \n";
            cin >> v1;
            x.set_mem(v, v1);
        } else if (n == 5) {
            int h;
            cin >> h;
            string j = x.get_m(h);
            cout << j;
        } else if (n == 6) {
            string h1;
            cin >> h1;
            x.get_mem(h1);
        }
        Central c1(&x, 256);
        c1.fetch();
        if (n == 2) {
            x.print_memory();
        } else if (n == 3) {
            cout << "=================reg\n";
            c1.print_reg();
        }

    } while (n != 0);

    return 0;
}
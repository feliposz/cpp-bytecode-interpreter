#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
using namespace std;

// Instruction set
enum class OpCode
{
    LOAD_VALUE,
    ADD_TWO_VALUES,
    PRINT_ANSWER,
    STORE_NAME,
    LOAD_NAME
};

struct Instruction
{
    OpCode Code;
    int Argument;
};

struct Program
{
    vector<Instruction> Instructions;
    vector<int> NumberConstants;
    vector<string> Names;
};

void Run(const Program &p, const bool debug)
{
    vector<int> Stack;
    unordered_map<string, int> Variables;

    for (auto Instruction : p.Instructions)
    {
        switch (Instruction.Code)
        {
        case OpCode::LOAD_VALUE:
            {
                int ConstantIndex = Instruction.Argument;
                if (debug) cout << "LOAD_VALUE " << ConstantIndex << endl;
                int Value = p.NumberConstants[ConstantIndex];
                Stack.push_back(Value);
            }
            break;
        case OpCode::STORE_NAME:
            {
                int NameIndex = Instruction.Argument;
                if (debug) cout << "STORE_NAME " << NameIndex << endl;
                int Value = Stack.back();
                Stack.pop_back();
                Variables[p.Names[NameIndex]] = Value;
            }
            break;
        case OpCode::LOAD_NAME:
            {
                int NameIndex = Instruction.Argument;
                if (debug) cout << "LOAD_NAME " << NameIndex << endl;
                int Value = Variables[p.Names[NameIndex]];
                Stack.push_back(Value);
            }
            break;
        case OpCode::ADD_TWO_VALUES:
            {
                if (debug) cout << "ADD_TWO_VALUES" << endl;
                int First = Stack.back();
                Stack.pop_back();
                int Second = Stack.back();
                Stack.pop_back();
                Stack.push_back(First + Second);
            }
            break;
        case OpCode::PRINT_ANSWER:
            {
                if (debug) cout << "PRINT_ANSWER" << endl;
                int Value = Stack.back();
                Stack.pop_back();
                cout << Value << endl;
            }
            break;
        }
        if (debug) cout << "    Stack size: " << Stack.size() << endl;
    }
}

int main()
{
    // a = 1
    // b = 2
    // print a + b
    const Program p {
        {
            {OpCode::LOAD_VALUE, 0},
            {OpCode::STORE_NAME, 0},
            {OpCode::LOAD_VALUE, 1},
            {OpCode::STORE_NAME, 1},
            {OpCode::LOAD_NAME, 0},
            {OpCode::LOAD_NAME, 1},
            {OpCode::ADD_TWO_VALUES},
            {OpCode::PRINT_ANSWER},
        },
        {1, 2},
        {"a", "b"}
    };

    Run(p, true);
}

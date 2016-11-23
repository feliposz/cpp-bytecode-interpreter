#include <iostream>
#include <vector>
using namespace std;

// Instruction set
enum class OpCode
{
    LOAD_VALUE,
    ADD_TWO_VALUES,
    PRINT_ANSWER
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
};

void Run(const Program &p, const bool debug)
{
    vector<int> Stack;

    for (auto Instruction : p.Instructions)
    {
        switch (Instruction.Code)
        {
        case OpCode::LOAD_VALUE:
            {
                int Index = Instruction.Argument;
                if (debug) cout << "LOAD_VALUE " << Index << endl;
                int Value = p.NumberConstants[Index];
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
    // print 5 + 7
    const Program p {
        {
            {OpCode::LOAD_VALUE, 0},
            {OpCode::LOAD_VALUE, 1},
            {OpCode::ADD_TWO_VALUES, 0},
            {OpCode::PRINT_ANSWER, 0},
        },
        {5, 7}
    };

    Run(p, true);

    // print 7 + 5 + 8
    const Program p2 {
        {
            {OpCode::LOAD_VALUE, 0},
            {OpCode::LOAD_VALUE, 1},
            {OpCode::ADD_TWO_VALUES, 0},
            {OpCode::LOAD_VALUE, 2},
            {OpCode::ADD_TWO_VALUES, 0},
            {OpCode::PRINT_ANSWER, 0},
        },
        {7, 5, 8}
    };

    Run(p2, true);
}

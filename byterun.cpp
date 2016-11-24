#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
using namespace std;

// Instruction set
enum class OpCode
{
    LOAD_CONST,
    STORE_FAST,
    LOAD_FAST,
    COMPARE_OP,
    POP_JUMP_IF_FALSE,
    RETURN_VALUE,
};

enum CompareOperator
{
    EQUAL,
    NOT_EQUAL,
    LESS_THAN,
    GREATER_THAN,
    LESS_THAN_OR_EQUAL,
    GREATER_THAN_OR_EQUAL,
};

struct Instruction
{
    OpCode Code;
    int Argument;
};

enum class ObjectType
{
    NONE,
    INTEGER,
    FLOAT,
    STRING,
};

struct Object
{
    ObjectType Type;
    struct
    {
        int Integer;
        float Float;
        string String;
    };
    Object() : Type {ObjectType::NONE} {}
    Object(int i) : Type {ObjectType::INTEGER}, Integer {i} {}
    Object(float f) : Type {ObjectType::FLOAT}, Float {f} {}
    Object(string s) : Type {ObjectType::STRING} { String = s; }
};

struct Program
{
    vector<Instruction> Instructions;
    vector<Object> Constants;
    vector<string> Names;
};

void ReturnObject(Object o)
{
    switch (o.Type)
    {
    case ObjectType::NONE:
        break;
    case ObjectType::INTEGER:
        {
            cout << "    Return (Integer): " << o.Integer << endl;
        }
        break;
    case ObjectType::FLOAT:
        {
            cout << "    Return (Float): " << o.Float << endl;
        }
        break;
    case ObjectType::STRING:
        {
            cout << "    Return (String): \"" << o.String << "\"" << endl;
        }
        break;
    default:
        {
            cerr << "Not implemented!";
        }
        break;
    }
}

bool CompareObject(CompareOperator Operator, Object Left, Object Right)
{
    bool Result = false;
    if (Left.Type == ObjectType::INTEGER && Right.Type == ObjectType::INTEGER)
    {
        switch (Operator)
        {
        case EQUAL: Result = (Left.Integer == Right.Integer); break;
        case NOT_EQUAL: Result = (Left.Integer != Right.Integer); break;
        case LESS_THAN: Result = (Left.Integer < Right.Integer); break;
        case GREATER_THAN: Result = (Left.Integer > Right.Integer); break;
        case LESS_THAN_OR_EQUAL: Result = (Left.Integer <= Right.Integer); break;
        case GREATER_THAN_OR_EQUAL: Result = (Left.Integer >= Right.Integer); break;
        default: break; // TODO: log invalid operator
        }
    }
    else if (Left.Type == ObjectType::FLOAT && Right.Type == ObjectType::FLOAT)
    {
        switch (Operator)
        {
        case EQUAL: Result = (Left.Float == Right.Float); break;
        case NOT_EQUAL: Result = (Left.Float != Right.Float); break;
        case LESS_THAN: Result = (Left.Float < Right.Float); break;
        case GREATER_THAN: Result = (Left.Float > Right.Float); break;
        case LESS_THAN_OR_EQUAL: Result = (Left.Float <= Right.Float); break;
        case GREATER_THAN_OR_EQUAL: Result = (Left.Float >= Right.Float); break;
        default: break; // TODO: log invalid operator
        }
    }
    else if (Left.Type == ObjectType::STRING && Right.Type == ObjectType::STRING)
    {
        switch (Operator)
        {
        case EQUAL: Result = (Left.String == Right.String); break;
        case NOT_EQUAL: Result = (Left.String != Right.String); break;
        case LESS_THAN: Result = (Left.String < Right.String); break;
        case GREATER_THAN: Result = (Left.String > Right.String); break;
        case LESS_THAN_OR_EQUAL: Result = (Left.String <= Right.String); break;
        case GREATER_THAN_OR_EQUAL: Result = (Left.String >= Right.String); break;
        default: break; // TODO: log invalid operator
        }
    }
    else
    {
        cerr << "Type Mismatch!" << endl;
    }
    return Result;
}

bool IsTrueValue(Object o)
{
    bool Result = false;
    switch (o.Type)
    {
    case ObjectType::NONE:
        {
            Result = false;
        }
        break;
    case ObjectType::INTEGER:
        {
            Result = o.Integer != 0;
        }
        break;
    case ObjectType::FLOAT:
        {
            Result = o.Integer != 0.0f;
        }
        break;
    case ObjectType::STRING:
        {
            Result = o.String.size() != 0;
        }
        break;
    }
    return Result;
}

void Run(const Program &p, const bool debug)
{

    vector<Object> Stack;
    unordered_map<string, Object> Variables;

    auto InstructionPointer = 0;

    while (InstructionPointer < p.Instructions.size())
    {

        auto Instruction = p.Instructions[InstructionPointer];

        // TODO: Check stack underflow!
        // TODO: Check undefined variables!

        switch (Instruction.Code)
        {
        case OpCode::LOAD_CONST:
            {
                int ConstantIndex = Instruction.Argument;
                if (debug) cout << "LOAD_CONST " << ConstantIndex << endl;
                Object Value = p.Constants[ConstantIndex];
                Stack.push_back(Value);
            }
            break;
        case OpCode::STORE_FAST:
            {
                int NameIndex = Instruction.Argument;
                if (debug) cout << "STORE_NAME " << NameIndex << endl;
                Object Value = Stack.back();
                Stack.pop_back();
                Variables[p.Names[NameIndex]] = Value;
            }
            break;
        case OpCode::LOAD_FAST:
            {
                int NameIndex = Instruction.Argument;
                if (debug) cout << "LOAD_NAME " << NameIndex << endl;
                Object Value = Variables[p.Names[NameIndex]];
                Stack.push_back(Value);
            }
            break;
        case OpCode::COMPARE_OP:
            {
                CompareOperator Operator = (CompareOperator)Instruction.Argument;
                if (debug) cout << "COMPARE_OP " << Operator << endl;
                Object Right = Stack.back();
                Stack.pop_back();
                Object Left = Stack.back();
                Stack.pop_back();
                bool Result = CompareObject(Operator, Left, Right);
                Stack.push_back(Result ? Object {1} : Object {0});
            }
            break;
        case OpCode::POP_JUMP_IF_FALSE:
            {
                if (debug) cout << "POP_JUMP_IF_FALSE" << endl;
                Object Value = Stack.back();
                Stack.pop_back();
                if (!IsTrueValue(Value))
                {
                    InstructionPointer = Instruction.Argument;
                    continue;
                }
            }
            break;
        case OpCode::RETURN_VALUE:
            {
                if (debug) cout << "RETURN_VALUE" << endl;
                Object Value = Stack.back();
                Stack.pop_back();
                ReturnObject(Value);
                InstructionPointer = p.Instructions.size();
                // TODO: Find a better way to gracefully return...
            }
            break;
        default:
            {
                // TODO: log invalid instruction
            }
            break;
        }

        if (debug) cout << "    Stack size: " << Stack.size() << endl;

        InstructionPointer++;
    }
}

int main()
{
    // x = 3
    // if x < 5:
    //     return 'yes'
    // else:
    //     return 'no'
    const Program p  {
        {
            {OpCode::LOAD_CONST, 1},
            {OpCode::STORE_FAST, 0},
            {OpCode::LOAD_FAST, 0},
            {OpCode::LOAD_CONST, 2},
            {OpCode::COMPARE_OP, LESS_THAN},
            {OpCode::POP_JUMP_IF_FALSE, 8},
            {OpCode::LOAD_CONST, 3},
            {OpCode::RETURN_VALUE},
            {OpCode::LOAD_CONST, 4}, // << JUMP
            {OpCode::RETURN_VALUE},
            {OpCode::LOAD_CONST, 0},
            {OpCode::RETURN_VALUE},
        },
        {
            Object {},
            Object {3},
            Object {5},
            Object {"yes"},
            Object {"no"},
        },
        {
            "x"
        }
    };

    Run(p, true);
}

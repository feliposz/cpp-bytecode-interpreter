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
    SETUP_LOOP,
    BINARY_ADD,
    JUMP_ABSOLUTE,
    POP_BLOCK,
    LOAD_GLOBAL,
    CALL_FUNCTION,
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
    UNIT,
};

struct Object
{
    ObjectType Type;
    struct
    {
        int Integer;
        float Float;
        string String;
        void* Unit;
    };
    Object() : Type {ObjectType::NONE} {}
    Object(int i) : Type {ObjectType::INTEGER}, Integer {i} {}
    Object(float f) : Type {ObjectType::FLOAT}, Float {f} {}
    Object(string s) : Type {ObjectType::STRING} { String = s; }
    Object(void *u) : Type {ObjectType::UNIT} { Unit = u; }
};

struct Unit
{
    vector<Instruction> Instructions;
    vector<Object> Constants;
    vector<string> LocalNames;
    vector<string> GlobalNames;
};

struct Program
{
    string EntryPoint;
    unordered_map<string, Object> Globals;
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

bool CompareObject(const CompareOperator Operator, const Object& Left, const Object& Right)
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

Object AddObjects(const Object& Left, const Object& Right)
{
    Object Result {};
    if (Left.Type == ObjectType::INTEGER && Right.Type == ObjectType::INTEGER)
    {
        Result.Type = ObjectType::INTEGER;
        Result.Integer = Left.Integer + Right.Integer;
    }
    else if (Left.Type == ObjectType::FLOAT && Right.Type == ObjectType::FLOAT)
    {
        Result.Type = ObjectType::FLOAT;
        Result.Float = Left.Float + Right.Float;
    }
    else if (Left.Type == ObjectType::STRING && Right.Type == ObjectType::STRING)
    {
        Result.Type = ObjectType::STRING;
        Result.String = Left.String + Right.String;
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

Object RunUnit(Program& p, const Unit* u, const bool debug, int NumArguments = 0, vector<Object> Arguments = {})
{
    Object Result;
    vector<Object> Stack;
    unordered_map<string, Object> Variables;

    auto InstructionPointer = 0;

    while (InstructionPointer < u->Instructions.size())
    {
        bool BreakLoop = false;
        bool IncrementPointer = true;
        auto Instruction = u->Instructions[InstructionPointer];

        // TODO: Check stack underflow!
        // TODO: Check undefined variables!
        // TODO: Use actual frames
        // TODO: Find a better way to manage globals

        switch (Instruction.Code)
        {
        case OpCode::LOAD_CONST:
            {
                int ConstantIndex = Instruction.Argument;
                if (debug) cout << "LOAD_CONST " << ConstantIndex << endl;
                Object Value = u->Constants[ConstantIndex];
                Stack.push_back(Value);
            }
            break;
        case OpCode::STORE_FAST:
            {
                int NameIndex = Instruction.Argument;
                if (debug) cout << "STORE_NAME " << NameIndex << endl;
                Object Value = Stack.back();
                Stack.pop_back();
                Variables[u->LocalNames[NameIndex]] = Value;
            }
            break;
        case OpCode::LOAD_FAST:
            {
                int NameIndex = Instruction.Argument;
                if (debug) cout << "LOAD_NAME " << NameIndex << endl;
                Object Value = Variables[u->LocalNames[NameIndex]];
                Stack.push_back(Value);
            }
            break;
        case OpCode::LOAD_GLOBAL:
            {
                int NameIndex = Instruction.Argument;
                if (debug) cout << "LOAD_GLOBAL " << NameIndex << endl;
                string key = u->GlobalNames[NameIndex];
                Object Value = p.Globals[key];
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
                if (debug) cout << "Left: " << Left.Integer << " Right: " << Right.Integer << endl;
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
                    IncrementPointer = false;
                }
            }
            break;
        case OpCode::RETURN_VALUE:
            {
                if (debug) cout << "RETURN_VALUE" << endl;
                Object Value = Stack.back();
                Stack.pop_back();
                ReturnObject(Value);
                BreakLoop = true;
                Result = Value;
            }
            break;
        case OpCode::SETUP_LOOP:
            {
                int RelativeJump = Instruction.Argument;
                if (debug) cout << "SETUP_LOOP " << RelativeJump << endl;
                Stack.push_back(Object {InstructionPointer + 1 + RelativeJump});
            }
            break;
        case OpCode::POP_BLOCK:
            {
                if (debug) cout << "POP_BLOCK" << endl;
                Object Jump = Stack.back();
                Stack.pop_back();
                InstructionPointer = Jump.Integer;
                IncrementPointer = false;
            }
            break;
        case OpCode::JUMP_ABSOLUTE:
            {
                int AbsoluteJump = Instruction.Argument;
                if (debug) cout << "JUMP_ABSOLUTE" << AbsoluteJump << endl;
                InstructionPointer = AbsoluteJump;
                IncrementPointer = false;
            }
            break;
        case OpCode::BINARY_ADD:
            {
                if (debug) cout << "BINARY_ADD" << endl;
                Object Left = Stack.back();
                Stack.pop_back();
                Object Right = Stack.back();
                Stack.pop_back();
                Stack.push_back(AddObjects(Left, Right));
            }
            break;
        case OpCode::CALL_FUNCTION:
            {
                int CallNumArguments = Instruction.Argument;
                if (debug) cout << "CALL_FUNCTION " << NumArguments << endl;
                vector<Object> CallArguments {};
                // TODO: Check order for arguments (probably reversed here...)
                for (int i = 0; i < CallNumArguments; i++)
                {
                    CallArguments.push_back(Stack.back());
                    Stack.pop_back();
                }
                Object u = Stack.back();
                Stack.pop_back();
                if (u.Type == ObjectType::UNIT)
                {
                    Object ResultUnit = RunUnit(p, (const Unit *)u.Unit, debug, CallNumArguments, CallArguments);
                    Stack.push_back(ResultUnit);
                }
                else
                {
                    cerr << "Not callable unit!" << endl;
                    BreakLoop = true;
                }
            }
            break;
        default:
            {
                cerr << "Instruction not implemented!" << endl;
                BreakLoop = true;
                IncrementPointer = false;
            }
            break;
        }

        if (debug)
        {
            cout << "    Stack size: " << Stack.size() << endl;
        }

        if (IncrementPointer)
        {
            InstructionPointer++;
        }

        if (BreakLoop)
        {
            break;
        }
    }

    return Result;
}

void RunProgram(Program& p, const bool Debug)
{
    Object& Entry = p.Globals[p.EntryPoint];
    if (Entry.Type == ObjectType::UNIT)
    {
        RunUnit(p, (const Unit *)Entry.Unit, Debug);
    }
    else
    {
        cerr << "Not a valid entry point!" << endl;
    }
}

int main()
{

    // def bar(y):
    //     z = y + 3
    //     return z
    //
    Unit BarUnit
    {
        {
            {OpCode::LOAD_FAST, 0},
            {OpCode::LOAD_CONST, 1},
            {OpCode::BINARY_ADD},
            {OpCode::STORE_FAST, 1},
            {OpCode::LOAD_FAST, 1},
            {OpCode::RETURN_VALUE},
        },
        {
            Object {},
            Object {3},
        },
        {
            "y",
            "z",
        },
        {},
    };

    // def foo():
    //     a = 1
    //     b = 2
    //     return a + bar(b)
    Unit FooUnit
    {
        {
            {OpCode::LOAD_CONST, 1},
            {OpCode::STORE_FAST, 0},
            {OpCode::LOAD_CONST, 2},
            {OpCode::STORE_FAST, 1},
            {OpCode::LOAD_FAST, 0},
            {OpCode::LOAD_GLOBAL, 0},
            {OpCode::LOAD_FAST, 1},
            {OpCode::CALL_FUNCTION, 1},
            {OpCode::BINARY_ADD},
            {OpCode::RETURN_VALUE},
        },
        {
            Object {},
            Object {1},
            Object {2},
        },
        {
            "a",
            "b",
        },
        {
            "bar"
        },
    };

    Program p {"foo"};

    p.Globals["foo"] = Object {&FooUnit};
    p.Globals["bar"] = Object {&BarUnit};

    RunProgram(p, true);
}

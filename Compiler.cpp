#include <string>
using namespace std;

// Enum to define different types of tokens that the tokenizer can generate.
enum Token
{
    token_end_of_file = -1, // Indicates the end of the input stream, signaling that all input has been processed.
    token_definition = -2,  // Represents a function or variable definition, identified by valid return types followed by parentheses.
    token_extern = -3,      // Represents an external declaration, such as "extern int x;".
    toke_identifier = -4,   // Represents identifiers, including variable names or function names.
    token_number = -5       // Represents numeric tokens, holding their values (e.g., integers or floating-point numbers).
};

// Static variables used to store intermediate values during the tokenization process.
static string Identifier; // Stores the identifier currently being processed (e.g., variable or function names).
static double numberVal;  // Stores the numeric value of a token if it represents a number.
static string FunctionReturnType[5] = {"float", "int", "double", "boolean", "char"}; // List of valid return types for function definitions.
static string Parenthesis = "()"; // Represents function parentheses, used to identify function definitions.

// Function to fetch the next token from the input stream.
static int getTok() {
    static int LastChar = ' '; // Keeps track of the last character read from the input.

    // Skip over any whitespace characters in the input stream.
    while (isspace(LastChar))
        LastChar = getchar();

    // Handle alphabetic characters, which may form identifiers or keywords.
    if (isalpha(LastChar)) {
        Identifier = LastChar; // Start building the identifier with the current character.
        while (isalpha(LastChar = getchar())) {
            Identifier += LastChar; // Append subsequent alphabetic characters.
        }

        // Check if the identifier matches any valid function return type and contains parentheses.
        for (int i = 0; i < 5; i++) { // Iterate through the predefined list of return types.
            if (Identifier == FunctionReturnType[i] && Identifier.find(Parenthesis) != string::npos) {
                return token_definition; // Token represents a function definition.
            }
        }

        // Check if the identifier is "extern", marking it as an external declaration.
        if (Identifier == "extern") {
            return token_extern;
        }

        return toke_identifier; // Return as a general identifier if no special case matches.
    }

    // Handle numeric tokens, including integers and floating-point numbers.
    if (isdigit(LastChar) || LastChar == '.') {
        string CurrentNumber; // Temporarily store the numeric token being processed.
        do {
            CurrentNumber += LastChar; // Build the numeric token character by character.
            LastChar = getchar();
        } while (isdigit(LastChar) || LastChar == '.');

        numberVal = strtod(CurrentNumber.c_str(), nullptr); // Convert the string representation to a numeric value.
        return token_number; // Return the numeric token.
    }

    // Handle comment lines starting with '#'. Treat them as non-token content to be skipped.
    if (LastChar == '#') {
        // Skip characters until the end of the line or input is reached.
        do {
            LastChar = getchar();
        } while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');
        
        // After processing the comment, fetch the next token.
        if (LastChar != EOF) {
            return getTok();
        }
    }

    // Handle end-of-file condition.
    if (LastChar == EOF) {
        return token_end_of_file;
    }

    // For unhandled characters, return their ASCII code.
    int ThisChar = LastChar;
    LastChar = getchar();
    return ThisChar;
}

class Export{
public:
    virtual ~Export() = default;
};

class NumberExport : public Export{
    double val;

public:
    NumberExport(double val) : Val(Val) {};
};

class VariableNameExport : public Export{
    string VariableName;
public:
    VariableNameExport(string &Name) : Name(Name){}
};

class BinaryOperator : public Export{
    char Operator;
    unique_ptr<Export> LeftSide;
    unique_ptr<Export> RightSide;
public :
    BinaryOperator(char Operator, unique_ptr<Export> LeftSide, unique_ptr<Export> RightSide) : Operator(Operator), LeftSide(move(LeftSide)), RightSide(move(RightSide)){};
};

class FunctionCallExport : public Export{
    string FunctionCall;
    vector<unique_ptr<Export>> Arguments;
public:
    FunctionCallExport(string FunctionCall,vector<unique_ptr<Export>> Arguments) : FunctionCall(FunctionCall), Arguments(move(Arguments)){}
};

class FunctionPrototype : public Export{
    string FunctionName;
    vector<unique_ptr> FunctionBody;
public:
    FunctionPrototype(string FunctionName,vector<unique_ptr> FunctionBody) : FunctionBody(move(FunctionBody)), FunctionName(FunctionName){}
};



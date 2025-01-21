#include <string>
using namespace std;

// Enum to define different types of tokens that the tokenizer can generate.
enum Token
{
    token_end_of_file = -1, // Represents the end of the file, indicating that all input has been processed.

    token_definition = -2, // Represents a function or variable definition, identified by specific return types.

    token_extern = -3, // Handles external declarations such as "extern int x;".

    toke_identifier = -4, // Represents an identifier, such as variable names or function names.

    token_number = -5 // Holds the numeric value of a token if it's a number (e.g., 42 or 3.14).
};

// Static variables to store intermediate values during tokenization.
static string Identifier; // Stores the current identifier being processed.
static double numberVal; // Stores the numeric value if the token is a number.
static string FunctionReturnType[5] = {"float", "int", "double", "boolean", "char"}; // List of valid return types for functions.
static string Parenthesis = "()"; // Represents function parentheses, used to identify function definitions.

// Function to get the next token from the input.
static int getTok() {
    static int LastChar = ' '; // Stores the last character read from the input.

    // Skip any whitespace characters in the input.
    while (isspace(LastChar))
        LastChar = getchar();

    // Handle alphabetic characters (identifiers or keywords).
    if (isalpha(LastChar)) {
        Identifier = LastChar; // Start building the identifier.
        while (isalpha(LastChar = getchar())) {
            Identifier += LastChar; // Append subsequent alphabetic characters.
        }

        // Check if the identifier matches a valid function return type and is followed by parentheses.
        for (int i = 0; i < 5; i++) { // Loop through valid function return types.
            if (Identifier == FunctionReturnType[i] && Identifier.find(Parenthesis) != string::npos) {
                return token_definition; // It's a function definition.
            }
        }

        // Check if the identifier is "extern" to mark it as an external declaration.
        if (Identifier == "extern") {
            return token_extern;
        }

        return toke_identifier; // Otherwise, return it as a general identifier.
    }

    // Handle numeric tokens, including integers and floating-point numbers.
    if (isdigit(LastChar) || LastChar == '.') {
        string CurrentNumber; // Stores the numeric token being processed.
        do {
            CurrentNumber += LastChar; // Build the numeric token character by character.
            LastChar = getchar();
        } while (isdigit(LastChar) || LastChar == '.');

        numberVal = strtod(CurrentNumber.c_str(), nullptr); // Convert the string to a numeric value.
        return token_number; // Return the numeric token.
    }
}
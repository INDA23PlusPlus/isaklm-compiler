#pragma once

#include <iostream>
#include "lexer.h"
#include "parser.h"

struct Function
{
	std::string header;
	std::string body;
};


bool match_any_branches(ParseTree branch, std::vector<NodeSymbol> symbols)
{
	for (int i = 0; i < symbols.size(); ++i)
	{
		if (branch.branches[0].node.symbol == symbols[i])
		{
			return true;
		}
	}

	return false;
}

bool match_branches(ParseTree branch, std::vector<NodeSymbol> symbols)
{
	if (branch.branches.size() != symbols.size())
	{
		return false;
	}

	for (int i = 0; i < symbols.size(); ++i)
	{
		if (branch.branches[i].node.symbol != symbols[i])
		{
			return false;
		}
	}

	return true;
}

std::string generate_code(ParseTree branch, std::vector<Function>& functions)
{
	std::string code = "";

	NodeSymbol symbol = branch.node.symbol;
	std::string content = branch.node.content;

	
	switch (symbol)
	{
	case STATEMENTS:
		if (match_branches(branch, { STATEMENT, STATEMENTS }))
		{
			code += generate_code(branch.branches[0], functions);
			code += generate_code(branch.branches[1], functions);
		}
		break;

	case STATEMENT:
		code += generate_code(branch.branches[0], functions);
		if (match_any_branches(branch, { VARIABLE_DECLARATION, ARRAY_DECLARATION, VARIABLE_ASSIGNMENT, ARRAY_ASSIGNMENT, EXIT, BREAK, FUNCTION_CALL, INPUT_CALL, PRINT_CALL, FUNCTION_RETURN }))
		{
			code += ";";
		}
		if (!match_branches(branch, { FUNCTION_DEFINITION }))
		{
			code += "\n";
		}
		break;

	case VARIABLE_DECLARATION:
		code += "int ";
		code += generate_code(branch.branches[0], functions);
		code += " = ";
		code += generate_code(branch.branches[2], functions);
		break;

	case ARRAY_DECLARATION:
		code += "int ";
		code += generate_code(branch.branches[0], functions);
		code += "[";
		code += generate_code(branch.branches[3], functions);
		code += "]";
		break;

	case FUNCTION_DEFINITION:
		functions.push_back({ generate_code(branch.branches[0], functions), "{\n" + generate_code(branch.branches[2], functions) + "}" });
		break;

	case FUNCTION_HEADER:
		code += "int ";
		code += generate_code(branch.branches[1], functions);
		code += "(";
		code += generate_code(branch.branches[3], functions);
		code += ")";
		break;

	case PARAMETERS:
		if (match_branches(branch, { PARAMETER, SEMICOLON, PARAMETERS }))
		{
			code += generate_code(branch.branches[0], functions);
			code += ", ";
			code += generate_code(branch.branches[2], functions);
		}
		else
		{
			code += generate_code(branch.branches[0], functions);
		}
		break;

	case PARAMETER:
		code += generate_code(branch.branches[2], functions);
		code += generate_code(branch.branches[0], functions);
		break;

	case FUNCTION_RETURN:
		code += "return ";
		code += generate_code(branch.branches[1], functions);
		break;

	case VARIABLE_ASSIGNMENT:
		code += generate_code(branch.branches[0], functions);
		code += " = ";
		code += generate_code(branch.branches[2], functions);
		break;

	case ARRAY_ASSIGNMENT:
		code += generate_code(branch.branches[0], functions);
		code += " = ";
		code += generate_code(branch.branches[2], functions);
		break;

	case ARRAY_ACCESS:
		code += generate_code(branch.branches[0], functions);
		code += "[";
		code += generate_code(branch.branches[2], functions);
		code += "]";
		break;

	case FUNCTION_CALL:
		code += generate_code(branch.branches[0], functions);
		code += "(";
		code += generate_code(branch.branches[2], functions);
		code += ")";
		break;

	case ARGUMENTS:
		code += generate_code(branch.branches[0], functions);
		code += generate_code(branch.branches[1], functions);
		break;

	case NEW_ARGUMENT:
		if (match_branches(branch, { SEMICOLON, ARITHMETIC_EXPRESSION, NEW_ARGUMENT }))
		{
			code += ", ";
			code += generate_code(branch.branches[1], functions);
			code += generate_code(branch.branches[2], functions);
		}
		break;

	case ARITHMETIC_EXPRESSION:
		if (match_branches(branch, { OPEN, ARITHMETIC_EXPRESSION, ARITHMETIC_OPERATOR, ARITHMETIC_EXPRESSION, CLOSE }))
		{
			code += "(";
			code += generate_code(branch.branches[1], functions);
			code += generate_code(branch.branches[2], functions);
			code += generate_code(branch.branches[3], functions);
			code += ")";
		}
		else
		{
			code += generate_code(branch.branches[0], functions);
		}
		break;

	case NUMERIC_LITERAL:
		code += generate_code(branch.branches[0], functions);
		break;

	case ARITHMETIC_OPERATOR:
		code += generate_code(branch.branches[0], functions);
		break;

	case ITERATION:
		if (match_branches(branch, { LOOP, OPEN_SCOPE, STATEMENTS, CLOSE_SCOPE }))
		{
			code += "while (true)\n";
			code += "{\n";
			code += generate_code(branch.branches[2], functions);
			code += "}";
		}
		else if (match_branches(branch, { WHILE, BOOLEAN_EXPRESSION, OPEN_SCOPE, STATEMENTS, CLOSE_SCOPE }))
		{
			code += "while (";
			code += generate_code(branch.branches[1], functions);
			code += ")\n";
			code += "{\n";
			code += generate_code(branch.branches[3], functions);
			code += "}";
		}
		else
		{
			code += "{\n";
			code += generate_code(branch.branches[2], functions);
			code += "while (";
			code += generate_code(branch.branches[4], functions);
			code += ")\n";
			code += "{\n";
			code += generate_code(branch.branches[6], functions);
			code += "}\n";
			code += "}";
		}
		
		break;

	case IF_STATEMENT:
		code += "if (";
		code += generate_code(branch.branches[1], functions);
		code += ")\n";
		code += "{\n";
		code += generate_code(branch.branches[3], functions);
		code += "}";
		break;

	case BOOLEAN_EXPRESSION:
		if (match_branches(branch, { NOT, OPEN, BOOLEAN_EXPRESSION, CLOSE }))
		{
			code += "!(";
			code += generate_code(branch.branches[1], functions);
			code += ")";
		}
		else if (match_branches(branch, { OPEN, BOOLEAN_EXPRESSION, LOGICAL_OPERATOR, BOOLEAN_EXPRESSION, CLOSE }))
		{
			code += "(";
			code += generate_code(branch.branches[1], functions);
			code += generate_code(branch.branches[2], functions);
			code += generate_code(branch.branches[3], functions);
			code += ")";
		}
		else
		{
			code += generate_code(branch.branches[0], functions);
			code += generate_code(branch.branches[1], functions);
			code += generate_code(branch.branches[2], functions);
		}
		break;

	case LOGICAL_OPERATOR:
		code += generate_code(branch.branches[0], functions);
		break;

	case COMPARISON_OPERATOR:
		code += generate_code(branch.branches[0], functions);
		break;

	case EXIT:
		code += "break";
		break;

	case CHAIN_SCOPE:
		code += "{\n";
		code += generate_code(branch.branches[2], functions);
		code += "}\n";
		code += "END_OF_CHAIN: ;";
		break;

	case BREAK:
		code += "goto END_OF_CHAIN";
		break;

	case INPUT_CALL:
		code += "input(";
		code += generate_code(branch.branches[2], functions);
		code += ")";
		break;

	case PRINT_CALL:
		code += "std::cout << ";
		code += generate_code(branch.branches[2], functions);
		break;

	case IDENTIFIER:
		code += content;
		break;

	case CONSTANT:
		code += content;
		break;

	case STRING:
		code += "\"";
		code += content;
		code += "\"";
		break;

	case INT:
		code += "int ";
		break;

	case VEC:
		code += "int* ";
		break;

	case PLUS:
		code += " + ";
		break;

	case MINUS:
		code += " - ";
		break;

	case MULTIPLY:
		code += " * ";
		break;

	case DIVIDE:
		code += " / ";
		break;

	case MODULO:
		code += " % ";
		break;

	case AND:
		code += " && ";
		break;

	case OR:
		code += " || ";
		break;

	case EQUALS:
		code += " == ";
		break;

	case NOT_EQUALS:
		code += " != ";
		break;

	case GREATER:
		code += " > ";
		break;

	case LESS:
		code += " < ";
		break;

	case GREATER_OR_EQUAL:
		code += " >= ";
		break;

	case LESS_OR_EQUAL:
		code += " <= ";
		break;
	}
	

	return code;
}

std::string output_code(ParseTree parse_tree)
{
	std::vector<Function> functions;

	Function main_function = { "int main()", "{\n" + generate_code(parse_tree, functions) + "return 0;\n}" };


	std::string code = "#include <iostream>\n\n";

	code += "int input(std::string message)\n";
	code += "{\n";
	code += "while (true)\n";
	code += "{\n";
	code += "std::cout << message;\n";
	code += "std::string input; std::cin >> input;\n";
	code += "int x = std::atoi(input.c_str());\n";
	code += "if (x != 0 || input == \"0\") { return x; }\n";
	code += "std::cout << \"Error: Enter integer.\" << '\\n';\n";
	code += "}\n";
	code += "}\n\n";

	for (Function function : functions)
	{
		code += function.header;
		code += ";";
		code += "\n";
	}

	code += "\n";

	for (Function function : functions)
	{
		code += function.header;
		code += "\n";
		code += function.body;
		code += "\n";
	}

	code += "\n";

	code += main_function.header;
	code += "\n";
	code += main_function.body;


	return code;
}

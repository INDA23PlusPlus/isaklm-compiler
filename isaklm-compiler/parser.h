#pragma once

#include <vector>
#include <string>
#include "lexer.h"

enum NodeSymbol
{
	STATEMENTS,
	STATEMENT,
	END_OF_STATEMENTS,
	VARIABLE_DECLARATION,
	ARRAY_DECLARATION,
	FUNCTION_DEFINITION,
	FUNCTION_HEADER,
	PARAMETERS,
	PARAMETER,
	FUNCTION_RETURN,
	VARIABLE_ASSIGNMENT,
	ARRAY_ASSIGNMENT,
	ARRAY_ACCESS,
	FUNCTION_CALL,
	ARGUMENTS,
	NEW_ARGUMENT,
	ARITHMETIC_EXPRESSION,
	NUMERIC_LITERAL,
	ARITHMETIC_OPERATOR,
	ITERATION,
	IF_STATEMENT,
	BOOLEAN_EXPRESSION,
	LOGICAL_OPERATOR,
	COMPARISON_OPERATOR,
	EXIT,
	CHAIN_SCOPE,
	BREAK,
	INPUT_CALL,
	PRINT_CALL,
	IDENTIFIER,
	CONSTANT,
	INT,
	VEC,
	DEF,
	RETURN,
	COLON,
	SEMICOLON,
	COLON_EQUALS,
	EQUALS,
	NOT_EQUALS,
	GREATER,
	LESS,
	GREATER_OR_EQUAL,
	LESS_OR_EQUAL,
	ASSIGN,
	PLUS,
	MINUS,
	MULTIPLY,
	DIVIDE,
	MODULO,
	AND,
	OR,
	NOT,
	OPEN_SCOPE,
	CLOSE_SCOPE,
	OPEN,
	CLOSE,
	OPEN_BRACKET,
	CLOSE_BRACKET,
	WHILE,
	LOOP,
	IF,
	CHAIN,
	INPUT,
	PRINT,
	STRING
};

struct Node
{
	NodeSymbol symbol;
	std::string content;
};

struct ParseTree
{
	Node node;
	std::vector<ParseTree> branches;
};


std::string symbol_to_string(NodeSymbol symbol)
{
	switch (symbol)
	{
	case STATEMENTS: return "statements";
	case STATEMENT: return "statement";
	case END_OF_STATEMENTS: return "end_of_statements";
	case VARIABLE_DECLARATION: return "variable_declaration";
	case ARRAY_DECLARATION: return "array_declaration";
	case FUNCTION_DEFINITION: return "function_definition";
	case FUNCTION_HEADER: return "function_header";
	case PARAMETERS: return "parameters";
	case PARAMETER: return "parameter";
	case FUNCTION_RETURN: return "function_return";
	case VARIABLE_ASSIGNMENT: return "variable_assignment";
	case ARRAY_ASSIGNMENT: return "array_assignment";
	case ARRAY_ACCESS: return "array_access";
	case FUNCTION_CALL: return "function_call";
	case ARGUMENTS: return "arguments";
	case NEW_ARGUMENT: return "new_argument";
	case ARITHMETIC_EXPRESSION: return "arithmetic_expression";
	case NUMERIC_LITERAL: return "numeric_literal";
	case ARITHMETIC_OPERATOR: return "arithmetic_operator";
	case ITERATION: return "iteration";
	case IF_STATEMENT: return "if_statement";
	case BOOLEAN_EXPRESSION: return "boolean_expression";
	case LOGICAL_OPERATOR: return "logical_operator";
	case COMPARISON_OPERATOR: return "comparison_operator";
	case EXIT: return "exit";
	case CHAIN_SCOPE: return "chain_scope";
	case BREAK: return "break";
	case INPUT_CALL: return "input_call";
	case PRINT_CALL: return "print_call";
	case IDENTIFIER: return "identifier";
	case CONSTANT: return "constant";
	case INT: return "int";
	case VEC: return "vec";
	case DEF: return "def";
	case RETURN: return "return";
	case COLON: return ":";
	case SEMICOLON: return ";";
	case COLON_EQUALS: return ":=";
	case EQUALS: return "=";
	case NOT_EQUALS: return "!=";
	case GREATER: return ">";
	case LESS: return "<";
	case GREATER_OR_EQUAL: return ">=";
	case LESS_OR_EQUAL: return "<=";
	case ASSIGN: return "->";
	case PLUS: return "+";
	case MINUS: return "-";
	case MULTIPLY: return "*";
	case DIVIDE: return "/";
	case MODULO: return "%";
	case AND: return "&";
	case OR: return "|";
	case NOT: return "!";
	case OPEN_SCOPE: return "{";
	case CLOSE_SCOPE: return "}";
	case OPEN: return "(";
	case CLOSE: return ")";
	case OPEN_BRACKET: return "[";
	case CLOSE_BRACKET: return "]";
	case WHILE: return "while";
	case LOOP: return "loop";
	case IF: return "if";
	case CHAIN: return "chain";
	case INPUT: return "input";
	case PRINT: return "print";
	case STRING: return "string";
	}

	return "something else";
}

void print_tree(ParseTree branch, int recursion_depth)
{
	std::string indentation = "";

	for (int i = 0; i < recursion_depth; ++i)
	{
		indentation += "  ";
	}


	NodeSymbol symbol = branch.node.symbol;
	std::string content = "";

	if (symbol == IDENTIFIER || symbol == CONSTANT || symbol == STRING)
	{
		content = " \"" + branch.node.content + "\"";
	}


	if (branch.branches.size() > 0)
	{
		std::cout << indentation << "SYMBOL: " << symbol_to_string(symbol) << content << '\n';
		std::cout << indentation << "{" << '\n';

		for (ParseTree branch : branch.branches)
		{
			print_tree(branch, recursion_depth + 1);
		}

		std::cout << indentation << "}" << '\n';
	}
	else
	{
		std::cout << indentation << "SYMBOL: " << symbol_to_string(symbol) << content << '\n';
	}
}


bool parse_symbol(NodeSymbol symbol, ParseTree& branch, std::vector<Token> tokens, int& token_index);

bool lookahead(std::vector<NodeSymbol> symbols, std::vector<Token> tokens, int token_index) // should only input terminal symbols
{
	for (int i = 0; i < symbols.size(); ++i)
	{
		int token_offset = token_index + i;

		if (token_offset >= tokens.size() || symbol_to_string(symbols[i]) != tokens[token_offset].type)
		{
			return false;
		}
	}

	return true;
}

bool search_for_token(std::string token_type, ParseTree& branch, std::vector<Token> tokens, int& token_index)
{
	if (token_index >= tokens.size())
	{
		return false;
	}

	Token token = tokens[token_index];

	if (token_type == token.type)
	{
		branch.node.content = token.content;

		++token_index;

		return true;
	}

	return false;
}

bool parse_symbols(std::vector<NodeSymbol> symbols, ParseTree& parent_branch, std::vector<Token> tokens, int& token_index)
{
	std::vector<ParseTree> branches = {};

	for (NodeSymbol symbol : symbols)
	{
		ParseTree branch;

		if (parse_symbol(symbol, branch, tokens, token_index))
		{
			branches.push_back(branch);
		}
		else
		{
			return false;
		}
	}

	parent_branch.branches = branches;

	return true;
}

bool parse_symbol(NodeSymbol symbol, ParseTree& branch, std::vector<Token> tokens, int& token_index)
{
	branch.node.symbol = symbol;
	branch.node.content = "";


	if (symbol == END_OF_STATEMENTS)
	{
		return true;
	}


	std::string token_type = "none";

	switch (symbol)
	{
	case IDENTIFIER:
	case CONSTANT:
	case INT:
	case VEC:
	case DEF:
	case RETURN:
	case COLON:
	case SEMICOLON:
	case COLON_EQUALS:
	case EQUALS:
	case NOT_EQUALS:
	case GREATER:
	case LESS:
	case GREATER_OR_EQUAL:
	case LESS_OR_EQUAL:
	case ASSIGN:
	case PLUS:
	case MINUS:
	case MULTIPLY:
	case DIVIDE:
	case MODULO:
	case AND:
	case OR:
	case NOT:
	case OPEN_SCOPE:
	case CLOSE_SCOPE:
	case OPEN:
	case CLOSE:
	case OPEN_BRACKET:
	case CLOSE_BRACKET:
	case WHILE:
	case LOOP:
	case IF:
	case EXIT:
	case CHAIN:
	case BREAK:
	case INPUT:
	case PRINT:
	case STRING:

		token_type = symbol_to_string(symbol);
	}


	if (token_type != "none")
	{
		if (search_for_token(token_type, branch, tokens, token_index))
		{
			return true;
		}
	}
	else
	{
		std::vector<NodeSymbol> symbols = {};

		switch (symbol)
		{
		case STATEMENTS:
			if (lookahead({ CLOSE_SCOPE }, tokens, token_index) || token_index >= tokens.size()) { symbols = { END_OF_STATEMENTS }; }
			else { symbols = { STATEMENT, STATEMENTS }; }
			break;

		case STATEMENT:
			if (lookahead({ IDENTIFIER, COLON_EQUALS }, tokens, token_index)) { symbols = { VARIABLE_DECLARATION }; }
			else if (lookahead({ IDENTIFIER, COLON, OPEN_BRACKET }, tokens, token_index)) { symbols = { ARRAY_DECLARATION }; }
			else if (lookahead({ DEF }, tokens, token_index)) { symbols = { FUNCTION_DEFINITION }; }
			else if (lookahead({ IDENTIFIER, ASSIGN }, tokens, token_index)) { symbols = { VARIABLE_ASSIGNMENT }; }
			else if (lookahead({ IDENTIFIER, OPEN_BRACKET }, tokens, token_index)) { symbols = { ARRAY_ASSIGNMENT }; }
			else if (lookahead({ WHILE }, tokens, token_index)) { symbols = { ITERATION }; }
			else if (lookahead({ LOOP }, tokens, token_index)) { symbols = { ITERATION }; }
			else if (lookahead({ IF }, tokens, token_index)) { symbols = { IF_STATEMENT }; }
			else if (lookahead({ EXIT }, tokens, token_index)) { symbols = { EXIT }; }
			else if (lookahead({ CHAIN }, tokens, token_index)) { symbols = { CHAIN_SCOPE }; }
			else if (lookahead({ BREAK }, tokens, token_index)) { symbols = { BREAK }; }
			else if (lookahead({ IDENTIFIER, OPEN }, tokens, token_index)) { symbols = { FUNCTION_CALL }; }
			else if (lookahead({ INPUT }, tokens, token_index)) { symbols = { INPUT_CALL }; }
			else if (lookahead({ PRINT }, tokens, token_index)) { symbols = { PRINT_CALL }; }
			else if (lookahead({ RETURN }, tokens, token_index)) { symbols = { FUNCTION_RETURN }; }
			break;

		case VARIABLE_DECLARATION:
			symbols = { IDENTIFIER, COLON_EQUALS, ARITHMETIC_EXPRESSION };
			break;

		case ARRAY_DECLARATION:
			symbols = { IDENTIFIER, COLON, OPEN_BRACKET, CONSTANT, CLOSE_BRACKET };
			break;

		case FUNCTION_DEFINITION:
			symbols = { FUNCTION_HEADER, OPEN_SCOPE, STATEMENTS, CLOSE_SCOPE };
			break;

		case FUNCTION_HEADER:
			symbols = { DEF, IDENTIFIER, OPEN, PARAMETERS, CLOSE };
			break;

		case PARAMETERS:
			if (lookahead({ IDENTIFIER, COLON, INT, SEMICOLON }, tokens, token_index)) { symbols = { PARAMETER, SEMICOLON, PARAMETERS }; }
			else if (lookahead({ IDENTIFIER, COLON, VEC, SEMICOLON }, tokens, token_index)) { symbols = { PARAMETER, SEMICOLON, PARAMETERS }; }
			else { symbols = { PARAMETER }; }
			break;

		case PARAMETER:
			if (lookahead({ IDENTIFIER, COLON, INT }, tokens, token_index)) { symbols = { IDENTIFIER, COLON, INT }; }
			else if (lookahead({ IDENTIFIER, COLON, VEC }, tokens, token_index)) { symbols = { IDENTIFIER, COLON, VEC }; }
			break;

		case FUNCTION_RETURN:
			symbols = { RETURN, ARITHMETIC_EXPRESSION };
			break;

		case VARIABLE_ASSIGNMENT:
			symbols = { IDENTIFIER, ASSIGN, ARITHMETIC_EXPRESSION };
			break;

		case ARRAY_ASSIGNMENT:
			symbols = { ARRAY_ACCESS, ASSIGN, ARITHMETIC_EXPRESSION };
			break;

		case ARRAY_ACCESS:
			symbols = { IDENTIFIER, OPEN_BRACKET, ARITHMETIC_EXPRESSION, CLOSE_BRACKET };
			break;

		case FUNCTION_CALL:
			symbols = { IDENTIFIER, OPEN, ARGUMENTS, CLOSE };
			break;

		case ARGUMENTS:
			symbols = { ARITHMETIC_EXPRESSION, NEW_ARGUMENT };
			break;

		case NEW_ARGUMENT:
			if (lookahead({ SEMICOLON, CLOSE }, tokens, token_index)) { symbols = { SEMICOLON }; }
			else { symbols = { SEMICOLON, ARITHMETIC_EXPRESSION, NEW_ARGUMENT }; }
			break;

		case ARITHMETIC_EXPRESSION:
			if (lookahead({ OPEN }, tokens, token_index)) { symbols = { OPEN, ARITHMETIC_EXPRESSION, ARITHMETIC_OPERATOR, ARITHMETIC_EXPRESSION, CLOSE }; }
			else { symbols = { NUMERIC_LITERAL }; }
			
			break;

		case NUMERIC_LITERAL:
			if (lookahead({ CONSTANT }, tokens, token_index)) { symbols = { CONSTANT }; }
			else if (lookahead({ IDENTIFIER, OPEN_BRACKET }, tokens, token_index)) { symbols = { ARRAY_ACCESS }; }
			else if (lookahead({ IDENTIFIER, OPEN }, tokens, token_index)) { symbols = { FUNCTION_CALL }; }
			else if (lookahead({ IDENTIFIER }, tokens, token_index)) { symbols = { IDENTIFIER }; }
			else if (lookahead({ INPUT }, tokens, token_index)) { symbols = { INPUT_CALL }; }
			break;

		case ARITHMETIC_OPERATOR:
			if (lookahead({ PLUS }, tokens, token_index)) { symbols = { PLUS }; }
			else if (lookahead({ MINUS }, tokens, token_index)) { symbols = { MINUS }; }
			else if (lookahead({ MULTIPLY }, tokens, token_index)) { symbols = { MULTIPLY }; }
			else if (lookahead({ DIVIDE }, tokens, token_index)) { symbols = { DIVIDE }; }
			else if (lookahead({ MODULO }, tokens, token_index)) { symbols = { MODULO }; }
			break;

		case ITERATION:
			if (lookahead({ LOOP }, tokens, token_index)) { symbols = { LOOP, OPEN_SCOPE, STATEMENTS, CLOSE_SCOPE }; }
			else if (lookahead({ WHILE, OPEN_SCOPE }, tokens, token_index)) { symbols = { WHILE, OPEN_SCOPE, STATEMENTS, CLOSE_SCOPE, BOOLEAN_EXPRESSION, OPEN_SCOPE, STATEMENTS, CLOSE_SCOPE }; }
			else { symbols = { WHILE, BOOLEAN_EXPRESSION, OPEN_SCOPE, STATEMENTS, CLOSE_SCOPE }; }
			break;

		case IF_STATEMENT:
			symbols = { IF, BOOLEAN_EXPRESSION, OPEN_SCOPE, STATEMENTS, CLOSE_SCOPE };
			break;

		case BOOLEAN_EXPRESSION:
			if (lookahead({ NOT }, tokens, token_index)) { symbols = { NOT, OPEN, BOOLEAN_EXPRESSION, CLOSE }; }
			else if (lookahead({ OPEN }, tokens, token_index)) { symbols = { OPEN, BOOLEAN_EXPRESSION, LOGICAL_OPERATOR, BOOLEAN_EXPRESSION, CLOSE }; }
			else { symbols = { ARITHMETIC_EXPRESSION, COMPARISON_OPERATOR, ARITHMETIC_EXPRESSION }; }
			break;

		case LOGICAL_OPERATOR:
			if (lookahead({ AND }, tokens, token_index)) { symbols = { AND }; }
			else if (lookahead({ OR }, tokens, token_index)) { symbols = { OR }; }
			break;

		case COMPARISON_OPERATOR:
			if (lookahead({ EQUALS }, tokens, token_index)) { symbols = { EQUALS }; }
			else if (lookahead({ NOT_EQUALS }, tokens, token_index)) { symbols = { NOT_EQUALS }; }
			else if (lookahead({ GREATER }, tokens, token_index)) { symbols = { GREATER }; }
			else if (lookahead({ LESS }, tokens, token_index)) { symbols = { LESS }; }
			else if (lookahead({ GREATER_OR_EQUAL }, tokens, token_index)) { symbols = { GREATER_OR_EQUAL }; }
			else if (lookahead({ LESS_OR_EQUAL }, tokens, token_index)) { symbols = { LESS_OR_EQUAL }; }
			break;

		case CHAIN_SCOPE:
			symbols = { CHAIN, OPEN_SCOPE, STATEMENTS, CLOSE_SCOPE };
			break;

		case INPUT_CALL:
			symbols = { INPUT, OPEN, STRING, CLOSE };
			break;

		case PRINT_CALL:
			if (lookahead({ PRINT, OPEN, STRING }, tokens, token_index)) { symbols = { PRINT, OPEN, STRING, CLOSE }; }
			else { symbols = { PRINT, OPEN, ARITHMETIC_EXPRESSION, CLOSE }; }
			break;
		}

		if (parse_symbols(symbols, branch, tokens, token_index))
		{
			branch.node.symbol = symbol;
			branch.node.content = "";

			return true;
		}
	}

	std::cout << "Error: Parser stopped at symbol " << '\"' << symbol_to_string(symbol) << '\"' << '\n';

	return false;
}

bool parse_tokens(ParseTree& parse_tree, std::vector<Token> tokens)
{
	int token_index = 0;

	if (parse_symbol(STATEMENTS, parse_tree, tokens, token_index))
	{
		return true;
	}
	
	std::cout << "Error: Could not parse program" << '\n';

	return false;
}
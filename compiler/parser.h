#pragma once

#include <vector>
#include <string>
#include <lexer.h>

enum NodeSymbol
{
	STATEMENTS,
	STATEMENT,
	DECLARATION,
	ASSIGNMENT,
	LOOP,
	CONDITIONAL,
	FUNCTION_CALL,
	INTEGER_DECLARATION,
	ARRAY_DECLARATION,
	FUNCTION_DECLARATION
};

struct Node
{
	NodeSymbol symbol;
	std::vector<Token> tokens; // all the tokens contained in the node
};

struct ParseTree
{
	Node node;
	std::vector<ParseTree> branches;
};

bool parse_symbol(NodeSymbol symbol, ParseTree& branch, std::vector<Token> tokens, int& token_index)
{
	if (symbol == STATEMENTS)
	{
		if (search_for_child_nodes({ STATEMENT, STATEMENTS }, branch, tokens, token_index)) { return true; }
		if (search_for_child_nodes({ STATEMENT }, branch, tokens, token_index)) { return true; }
	}

	if (symbol == STATEMENT)
	{
		if (search_for_child_nodes({ DECLARATION }, branch, tokens, token_index)) { return true; }
		if (search_for_child_nodes({ ASSIGNMENT }, branch, tokens, token_index)) { return true; }
		if (search_for_child_nodes({ LOOP }, branch, tokens, token_index)) { return true; }
		if (search_for_child_nodes({ CONDITIONAL }, branch, tokens, token_index)) { return true; }
		if (search_for_child_nodes({ FUNCTION_CALL }, branch, tokens, token_index)) { return true; }
	}

	if (symbol == DECLARATION)
	{
		if (search_for_child_nodes({ INTEGER_DECLARATION }, branch, tokens, token_index)) { return true; }
		if (search_for_child_nodes({ ARRAY_DECLARATION }, branch, tokens, token_index)) { return true; }
		if (search_for_child_nodes({ FUNCTION_DECLARATION }, branch, tokens, token_index)) { return true; }
	}


	return false;
}

bool search_for_child_nodes(std::vector<NodeSymbol> symbols, ParseTree& parent_tree, std::vector<Token> tokens, int& token_index)
{
	std::vector<ParseTree> branches = {};

	for (NodeSymbol symbol : symbols)
	{
		ParseTree branch;

		branch.node.symbol = symbol;


		if (parse_symbol(symbol, branch, tokens, token_index))
		{
			branches.push_back(branch);
		}
	}

	if (branches.size() == symbols.size()) // a branch was created for every symbol, i.e. all symbols were found
	{
		parent_tree.branches = branches;

		return true;
	}

	return false;
}

ParseTree parse_tokens(std::vector<Token> tokens)
{
	
}
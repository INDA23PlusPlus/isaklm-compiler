#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>


struct Token
{
	std::string type;
	std::string content;
};

bool match_keyword(std::string source_code, int offset, Token& token)
{
	std::vector<std::string> keywords =
	{
		"int", "vec", "if", "while", "loop", "def", "return", "exit", "chain", "break", "print", "input"
	};


	std::string word = "";

	for (int i = 0; ; i++)
	{
		int string_index = offset + i;

		if (string_index < source_code.size())
		{
			char character = source_code[string_index];

			if (isalpha(character))
			{
				word += character;
			}
			else
			{
				break;
			}
		}
	}

	for (std::string keyword : keywords)
	{
		if (word == keyword)
		{
			token = { keyword, word };

			return true;
		}
	}

	return false;
}


bool match_constant(std::string source_code, int offset, Token& token)
{
	std::string constant = "";

	{
		char character = source_code[offset];

		if (character == '-' || isdigit(character))
		{
			constant += character;
		}
		else
		{
			return false;
		}
	}

	for (int i = 1; ; i++)
	{
		int string_index = offset + i;

		if (string_index < source_code.size())
		{
			char character = source_code[string_index];

			if (isdigit(character))
			{
				constant += character;
			}
			else
			{
				break;
			}
		}
	}

	if (constant != "");
	{
		if (constant != "-") // had to make this into another if statement because of a compiler bug
		{
			token = { "constant", constant };

			return true;
		}
	}

	return false;
}

void collect_string(std::string source_code, int offset, Token& token)
{
	std::string string = "";

	for (int i = offset; ; ++i)
	{
		if (i >= source_code.size() || source_code[i] == '\"')
		{
			break;
		}

		string += source_code[i];
	}

	token = { "string", string };
}

bool match_substring(std::string substring, std::string string, int offset)
{
	for (int i = 0; i < substring.size(); ++i)
	{
		int string_index = offset + i;

		if (string_index >= string.size() || substring[i] != string[string_index])
		{
			return false;
		}
	}

	return true;
}

bool match_symbol(std::string source_code, int offset, Token& token)
{
	std::vector<std::string> symbols =
	{
		":=", ":", "->", "+", "-", "*", "/", "%", "=", "!=", ">", "<", ">=", "<=", "&", "|", "!", ";", "(", ")", "{", "}", "[", "]"
	};


	std::string matching_symbol = "";

	for (std::string symbol : symbols)
	{
		bool match = match_substring(symbol, source_code, offset); // had to make into a variabel because of an insane compiler bug

		if (match && (symbol.size() > matching_symbol.size()))
		{
			matching_symbol = symbol;
		}
	}

	if (matching_symbol != "")
	{
		token = { matching_symbol, matching_symbol };

		return true;
	}

	return false;
}

bool identifier_is_legal(std::string identifier)
{
	if (identifier == "" || (identifier.size() > 1 && identifier[0] == '0'))
	{
		return false;
	}

	char previous_character = ' ';

	for (char character : identifier)
	{
		if (!(isalpha(character) || isdigit(character) || character == '_'))
		{
			return false;
		}

		if ((isalpha(character) || character == '_') && isdigit(previous_character))
		{
			return false;
		}

		previous_character = character;
	}

	return true;
}

std::string get_identifier(std::string source_code, int offset)
{
	std::string identifier = "";

	for (int i = offset; ; i++)
	{
		char character = source_code[i];

		if (i >= source_code.size() || !(isalpha(character) || isdigit(character) || character == '_'))
		{
			break;
		}

		identifier += character;
	}

	return identifier;
}

std::vector<Token> lexer(std::string file_name)
{
	std::ifstream program_file(file_name);

	std::string source_code = "";


	char character;

	while (program_file.get(character))
	{
		if (character == '"')
		{
			character = '\"';
		}

		source_code += character;
	}


	std::vector<Token> tokens = {};

	bool beginning_of_string = false;

	int index = 0;

	while (index < source_code.size())
	{
		char character = source_code[index];

		if (character == ' ' || character == '\t' || character == '\n')
		{
			++index;
		}
		else
		{
			if (character == '\"')
			{
				++index;

				beginning_of_string = !beginning_of_string;
			}

			Token token = { "uninitialized", "nothing" };

			if (beginning_of_string)
			{
				collect_string(source_code, index, token);
			}
			else if (!match_constant(source_code, index, token) && !match_symbol(source_code, index, token) && !match_keyword(source_code, index, token))
			{
				std::string identifier = get_identifier(source_code, index);

				if (identifier_is_legal(identifier))
				{
					token = { "identifier", identifier };
				}
				else
				{
					token = { "invalid", identifier };
				}
			}

			index += token.content.size();

			tokens.push_back(token);
		}
	}


	program_file.close();

	return tokens;
}
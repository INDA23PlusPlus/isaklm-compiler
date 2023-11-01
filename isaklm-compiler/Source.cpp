#include "compiler.h"

int main()
{
	std::vector<Token> tokens = lexer("tictactoe.txt");

	/*for (Token token : tokens)
	{
		std::cout << "TOKEN: " << token.type << "   CONTENT: " << token.content << '\n';
	}

	for (Token token : tokens)
	{
		if (token.type == "invalid")
		{
			std::cout << "ERROR: " << token.content << " is not a valid identifier." << '\n';
		}
	}*/


	ParseTree parse_tree;

	if (parse_tokens(parse_tree, tokens))
	{
		//print_tree(parse_tree, 0);

		std::string generated_code = output_code(parse_tree);

		std::cout << generated_code << '\n';
	}


	while (true);

	return 0;
}
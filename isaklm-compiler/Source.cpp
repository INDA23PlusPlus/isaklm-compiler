#include "compiler.h"

int main()
{
	std::vector<Token> tokens = lexer("tic_tac_toe.txt");

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
		print_tree(parse_tree, 0);
		
		std::cout << "\n\nGENERATED C++ CODE HERE:\n\n";

		std::cout << output_code(parse_tree);
	}


	while (true);

	return 0;
}
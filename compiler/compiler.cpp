#include "compiler.h"

int main()
{
	std::vector<Token> tokens = lexer("example_program.txt");

	for (Token token : tokens)
	{
		std::cout << "TOKEN: " << token.type << "   CONTENT: " << token.content << '\n';
	}

	for (Token token : tokens)
	{
		if (token.type == "invalid")
		{
			std::cout << "ERROR: " << token.content << " is not a valid identifier." << '\n';
		}
	}


	while (true);

	return 0;
}

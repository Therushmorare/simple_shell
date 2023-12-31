#include "shell.h"


/**
 * exit_func - function to be used to exit shell
 * @chars_input: shell command
 * Return: 0
 */

void exit_func(char *chars_input)
{
	char *token;
	char *delim = " \n";
	int exit_status;

	if (strcmp(chars_input, "exit") == 0)
	{
		if (builtin_checker(chars_input))
			exit(0);
	}
	else if (strncmp(chars_input, "exit", 4) == 0)
	{
		token = strtok(chars_input, delim);
		if (builtin_checker(token))
		{
			token = strtok(NULL, delim);
			exit_status = atoi(token);
			exit(exit_status);
		}
	}

}

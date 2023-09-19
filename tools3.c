#include "shell.h"

void handle_operator(ALL *args)
{
	/*to indicate there was an operator*/
	args->was_operator = 1;
	/*assign the operator for clarity*/
	args->operator= args->commands->command[0];
	/*skip one command*/
	args->commands = args->commands->next;
}

void handle_operator_status(ALL *args)
{
	/*if (&&) and last command was not success*/
	if (_strcmp(args->operator, "&&") == 0 && args->status != 0)
		args->commands = args->commands->next;

	/*if (||) and last command was success*/
	else if (_strcmp(args->operator, "||") == 0 && args->status == 0)
		args->commands = args->commands->next;

	/*reset*/
	args->was_operator = 0;
}
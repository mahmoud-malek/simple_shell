#include "shell.h"

/**
 * handle_operator - handles case of logcial opearator found
 * @args: all arguments
 */

void handle_operator(ALL *args)
{

	/*to indicate there was an operator*/
	args->was_operator = 1;
	/*assign the operator for clarity*/
	args->operator= args->commands->command[0];
	/*skip one command*/
	args->commands = args->commands->next;
}

/**
 * handle_operator_status - handle different statues of logical operataors
 * @args: all required args
 */

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

/**
 * is_logical - determine whether a command is a logical operator
 * @cmd: command to check
 * Return: 1 if logical operator, 0 otherwise
 */
int is_logical(char *cmd)
{
	return (!_strcmp(cmd, "&&") || !_strcmp(cmd, "||") || !_strcmp(cmd, ";"));
}

/**
 * check_alias - check if a command is an alias
 * @args: contain all required arguemnts
 * Description: replaces a command with alias value if found
 */

void check_alias(ALL *args)
{
	list *tmp = args->commands;
	ALIAS *als = args->aliases;
	int i = 0, found = 0;

	while (tmp != NULL)
	{

		for (i = 0; tmp->command[i] != NULL; i++)
		{
			do
			{
				found = 0;
				als = args->aliases;

				while (als != NULL)
				{
					if (_strcmp(als->name, tmp->command[i]) == 0)
					{
						free(tmp->command[i]);
						tmp->command[i] = _strdup(als->value);

						found = 1;
						break;
					}
					als = als->next;
				}

			} while (found);
		}

		tmp = tmp->next;
	}
}

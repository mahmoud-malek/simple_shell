#include "shell.h"

void (*is_built_in(ALL *args))(ALL *)
{
	int i = 0;
	built_in programs[] = {
		{"exit", builtin_exit},
		{"env", builtin_env},
		{"setenv", builtin_setenv},
		{"unsetenv", builtin_unsetenv},
		{NULL, NULL}};

	for (; programs[i].name != NULL; i++)
	{
		if (_strcmp(programs[i].name, args->commands->command[0]) == 0)
			return (programs[i].function);
	}

	return (NULL);
}

void builtin_exit(ALL *args)
{
	int code = 0;

	if (args->commands->command[1] == NULL)
	{
		free(args->line);
		free_list(args->commands);
		free_2D(args->envrion_cpy);
		exit(args->status);
	}
	else
	{
		code = _atoi_(args->commands->command[1]);
		if (_strlen(args->commands->command[1]) > 10 || code == -1 || code > MAX_INT)
		{
			print_exit_error(args);
		}
		else
		{
			free(args->line);
			free_list(args->commands);
			free_2D(args->envrion_cpy);

			exit(code);
		}
	}
}

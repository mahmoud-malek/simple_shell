#include "shell.h"

void (*is_built_in(ALL *args))(ALL *)
{
	int i = 0;
	built_in programs[] = {
		{"exit", builtin_exit},
		{"env", builtin_env},
		{"setenv", builtin_setenv},
		{"unsetenv", builtin_unsetenv},
		{"cd", builtin_cd},
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

void builtin_cd(ALL *args)
{
	char *home = NULL;
	char *oldpwd = NULL;
	char *cwd = NULL;
	int status = 0;

	if (!args->commands->command[1] || _strcmp(args->commands->command[1], "") == 0)
	{
		home = _strdup(_getenv("HOME", args));
		if (!home)
			return;

		else
		{
			status = chdir(home);
			if (status == -1)
				print_error_cd(args);
			else
			{
				_setenv("PWD", home, args);
			}
			free(home);
		}
	}

	else
	{
		cwd = getcwd(NULL, 0);
		if (!cwd)
		{
			print_error_cd(args);
			return;
		}
		if (_strcmp(args->commands->command[1], "-") == 0)
		{
			oldpwd = _strdup(_getenv("OLDPWD", args));
			if (!oldpwd)
			{

				_puts(cwd);
				_putchar('\n');
				free(cwd);
				return;
			}

			else
			{
				status = chdir(oldpwd);
				if (status == -1)
					print_error_cd(args);
				else
				{
					_setenv("OLDPWD", cwd, args);
					_setenv("PWD", oldpwd, args);
					_puts(oldpwd);
					_putchar('\n');
				}
				free(oldpwd);
			}
		}

		else
		{
			status = chdir(args->commands->command[1]);
			if (status == -1)
				print_error_cd(args);
			else
			{
				_setenv("PWD", args->commands->command[1], args);
				_setenv("OLDPWD", cwd, args);
			}
		}
	}

	if (cwd != NULL)
		free(cwd);
}
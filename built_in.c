#include "shell.h"

/**
 * is_built_in - checks if a command is built in
 * @args: contain built-in structure that contain a command name
 * and command function
 * Return: the appropriate function for command
 */

void (*is_built_in(ALL *args))(ALL *)
{
	int i = 0;
	built_in programs[] = {
		{"exit", builtin_exit},
		{"env", builtin_env},
		{"setenv", builtin_setenv},
		{"unsetenv", builtin_unsetenv},
		{"cd", builtin_cd},
		{"alias", builtin_alias},
		{NULL, NULL}};

	for (; programs[i].name != NULL; i++)
	{
		if (_strcmp(programs[i].name, args->commands->command[0]) == 0)
			return (programs[i].function);
	}

	return (NULL);
}

/**
 * builtin_exit - exits the shell with or wihtout code
 * @args: all required information to exit with
 */

void builtin_exit(ALL *args)
{
	int code = 0;

	if (args->commands->command[1] == NULL)
	{
		free(args->line);
		free_list(args->commands);
		free_2D(args->envrion_cpy);
		free_aliases_list(args);
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
			free_aliases_list(args);
			exit(code);
		}
	}
}

/**
 * builtin_cd - changes the current directory to whatever
 * the user wants to go
 *
 * @args: contain arguments to proccess cd
 */

void builtin_cd(ALL *args)
{
	char *home = NULL, *oldpwd = NULL, *cwd = NULL;
	int status = 0;

	if (!args->commands->command[1] ||
		_strcmp(args->commands->command[1], "") == 0)
	{
		home = _strdup(_getenv("HOME", args));
		if (!home)
			return;
		status = chdir(home);
		if (status == -1)
			print_error_cd(args);
		else
			_setenv("PWD", home, args);
		free(home);
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

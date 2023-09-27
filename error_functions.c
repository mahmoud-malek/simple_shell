#include "shell.h"

/**
 * print_error - prints an error message to stderr
 * @args: contain arguments to specify where error happens
 */

void print_error(ALL *args)
{
	char *line_num = NULL;

	line_num = _itoa(args->line_number);
	/*Case of a file can't open*/
	if (args->fd == -1)
	{
		eputs(args->shell_name);
		eputs(": ");
		eputs(line_num);
		eputs(": ");
		eputs("Can't open ");
		eputs(args->av[1]);
		eputchar('\n');
	}
	else
	{
		/**Normal case where can't find a command*/
		eputs(args->shell_name);
		eputs(": ");
		eputs(line_num);
		eputs(": ");
		eputs(args->commands->command[0]);
		eputs(": ");
		eputs("not found\n");
	}
	args->status = ERROR_NOT_FOUND;
	if (line_num != NULL)
		free(line_num);
}

/**
 * print_exit_error - prints an error for exit command
 * @args: contain args to determine kind of error
 */

void print_exit_error(ALL *args)
{
	char *line_num = NULL;

	line_num = _itoa(args->line_number);
	eputs(args->shell_name);
	eputs(": ");
	eputs(line_num);
	eputs(": ");
	eputs(args->commands->command[0]);
	eputs(": ");
	eputs("Illegal number: ");
	eputs(args->commands->command[1]);
	eputchar('\n');
	args->status = ERROR_EXIT;

	if (line_num != NULL)
		free(line_num);
}

/**
 * print_error_cd - print an error for cd command
 * @args: to specify where and what is the error
 */

void print_error_cd(ALL *args)
{
	char *line_num = NULL;

	line_num = _itoa(args->line_number);
	eputs(args->shell_name);
	eputs(": ");
	eputs(line_num);
	eputs(": ");
	eputs(args->commands->command[0]);
	eputs(": ");
	eputs("can't cd to ");
	eputs(args->commands->command[1]);
	eputchar('\n');

	args->status = ERROR_NOT_FOUND;
	if (line_num != NULL)
		free(line_num);
}

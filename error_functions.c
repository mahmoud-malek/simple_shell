#include "shell.h"

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
	args->status = 2;

	if (line_num != NULL)
		free(line_num);
}
#include "shell.h"

int _atoi_(char *str)
{

	long int number = 0;
	char *ptr = str;

	if (!ptr)
		return (-1);

	if (*ptr == '-')
		return (-1);

	if (*ptr == '+')
		ptr++;

	while (*ptr != '\0')
	{
		if (*ptr < '0' || *ptr > '9')
			return (-1);

		number = number * 10 + (*ptr - '0');
		ptr++;
	}

	return (number);
}

char *_itoa(int num)
{
	char *res = NULL;
	int len = (num == 0 ? 1 : 0);
	int n = (num < 0 ? -num : num);

	while (n)
	{
		len++;
		n /= 10;
	}

	res = malloc((num < 0 ? len + 2 : len + 1) * sizeof(char));
	if (res == NULL)
		return (NULL);

	res[len] = '\0';
	if (num < 0)
		res[0] = '-';

	n = num < 0 ? -num : num;
	do {
		res[--len] = (n % 10) + '0';
		n /= 10;

	} while (n);

	return (res);
}

void remove_comments(ALL *args)
{
	int quote = 0;
	int i = 0;
	char *ptr = args->line;

	if (!ptr)
		return;

	for (; ptr[i] != '\0'; i++)
	{
		if (ptr[i] == '"')
			quote = !quote;

		if (ptr[i] == '#' && !quote && (i == 0 || ptr[i - 1] == ' '))
		{
			for (; ptr[i] != '\0'; i++)
				ptr[i] = '\0';
			break;
		}
	}
}

void initialize_arguments(ALL *args, char **av)
{
	args->av = av;
	args->commands = NULL;
	args->tmp = NULL;
	args->path = NULL;
	args->line = NULL;
	args->operator = NULL;
	args->envrion_cpy = _strdup2D(environ);
	args->prompt = "#cisfun ";
	args->shell_name = av[0];
	args->line_number = 0;
	args->was_operator = 0;
	args->status = EXIT_SUCCESS;
	args->fd = STDIN_FILENO;
}

int interactive(int ac)
{
	return ((isatty(STDIN_FILENO) && ac == 1) ? 1 : 0);
}

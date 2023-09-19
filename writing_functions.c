#include "shell.h"

int _putchar(char c)
{
	return (write(STDOUT_FILENO, &c, 1));
}

int eputchar(char c)
{
	return (write(STDERR_FILENO, &c, 1));
}

int _puts(char *str)
{
	int chars = 0;
	char *tmp = str;

	if (!str)
		return (0);

	while (*tmp != '\0')
	{
		_putchar(*tmp);
		tmp++;
		chars++;
	}

	return (chars);
}

int eputs(char *str)
{
	int chars = 0;

	if (!str)
		return (0);

	while (*str != '\0')
	{
		eputchar(*str);
		str++;
		chars++;
	}

	return (chars);
}

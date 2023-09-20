#include "shell.h"

/**
 * _putchar - writes a char to stdout
 * @c: character to write
 * Return: 1 if success, -1 otherwise
 */

int _putchar(char c)
{
	return (write(STDOUT_FILENO, &c, 1));
}

/**
 * eputchar - writes a char to stderr
 * @c: character to write
 * Return: 1 on success, and -1 otherwise
 */
int eputchar(char c)
{
	return (write(STDERR_FILENO, &c, 1));
}

/**
 * _puts - writes a string to stdout
 * @str: string to write
 * Return: number of bytes written or -1 on error
 */

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

/**
 * eputs - writes string to stderr
 * @str: string to print
 * Return: number of bytes on success, -1 otherwise
 */

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

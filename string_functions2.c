#include "shell.h"
/**
 * _strncpy - copies n bytes of a string
 * @dest: destination
 * @src: source
 * @n: number of bytes
 * Return: destination
 */
char *_strncpy(char *dest, char *src, int n)
{
	char *hold = dest;
	int i = 0;

	while (i < n && (src[i] != '\0'))
	{
		hold[i] = src[i];
		i++;
	}

	while (i < n)
		hold[i++] = '\0';

	return (dest);
}

char *_strdup(const char *str)
{
	char *result = NULL;
	unsigned long i = 0;
	unsigned long len = 0;

	if (!str)
		return (NULL);

	len = _strlen(str);
	result = malloc((len + 1) * sizeof(char));

	if (!result)
		return (NULL);

	for (; i < len; i++)
		result[i] = str[i];

	result[i] = '\0';
	return (result);
}

char **_strdup2D(char **str)
{
	int len = 0, i = 0;
	char **res = NULL;

	if (!str || !*str)
		return (NULL);

	while (str[len] != NULL)
		len++;

	res = malloc((len + 1) * sizeof(char *));
	if (!res)
		return (NULL);

	while (str[i] != NULL)
	{
		res[i] = _strdup(str[i]);
		if (!res[i])
		{
			free_2D(res);
			return (NULL);
		}
		i++;
	}

	res[i] = NULL;
	return (res);
}

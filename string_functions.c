#include "shell.h"

size_t _strlen(const char *str)
{
	size_t len = 0;

	if (!str)
		return (0);

	while (str[len] != '\0')
		len++;

	return (len);
}

int _strcmp(char *str1, char *str2)
{
	while (*str1 && *str2)
	{
		if (*str1 != *str2)
			return (*str1 - *str2);

		str1++;
		str2++;
	}

	if (*str1 != *str2)
		return (*str1 - *str2);

	return (0);
}

char *_strcat(char *dest, const char *src)
{
	char *tmp = dest + _strlen(dest);

	while ((*tmp++ = *src++))
		;

	return (dest);
}

char *_strncat(char *dest, char *src, int n)
{
	char *hold = dest + _strlen(dest);

	while (n-- && (*hold++ = *src++));

	*hold = '\0';

	return (dest);
}

char *_strcpy(char *dest, char *src)
{
	int i = 0;

	for (; src[i] != '\0'; i++)
		dest[i] = src[i];

	dest[i] = '\0';
	return (dest);
}

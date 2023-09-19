#include "shell.h"

void free_2D(char **array)
{
	unsigned int i = 0;

	if (!array || !*array)
		return;

	for (; array[i] != NULL; i++)
		free(array[i]);

	free(array);
}

void *_realloc(void *buffer, unsigned int old_siz, unsigned int new_siz)
{
	void *result = NULL;

	if (old_siz == new_siz)
		return (buffer);

	if (new_siz == 0 && buffer != NULL)
	{
		free(buffer);
		return (NULL);
	}

	result = malloc(new_siz);
	if (!result)
		return (NULL);

	if (buffer != NULL)
	{
		_memcpy(result, buffer, min(new_siz, old_siz));
		free(buffer);
	}

	return (result);
}

char *_memcpy(char *dest, char *src, unsigned int n)
{
	unsigned int i = 0;

	for (; i < n; i++)
		dest[i] = src[i];

	return (dest);
}

void *_memchr(const void *str, int ch, size_t n)
{
	/*casting the string to unsinged char pointer*/
	unsigned char *ptr = (unsigned char *)str;

	while (n--)
	{
		if (*ptr == (unsigned char)ch)
			return (ptr);

		ptr++;
	}

	return (NULL);
}

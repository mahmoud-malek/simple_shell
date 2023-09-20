#include "shell.h"

/**
 * free_2D - frees a two dimensional array
 * @array: array to free
 */

void free_2D(char **array)
{
	unsigned int i = 0;

	if (!array || !*array)
		return;

	for (; array[i] != NULL; i++)
		free(array[i]);

	free(array);
}

/**
 * _realloc - reallocate memory for buffer
 * @buffer: the objective buffer
 * @old_siz: old size
 * @new_siz: new size to reallocate
 * Return: newly allocated space for buffer
 */

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

/**
 * _memcpy - copyies n bytes from src to dest
 * @n: number of bytes to copy
 * @src: srouce buffer
 * @dest: destination buffer
 * Return: destination;
 */

char *_memcpy(char *dest, char *src, unsigned int n)
{
	unsigned int i = 0;

	for (; i < n; i++)
		dest[i] = src[i];

	return (dest);
}

/**
 * _memchr - searchs for char in first n bytes in memory
 * @str: str to search in
 * @ch: character to search for
 * @n: number of bytes to search in
 * Return: pointer to the location where found a char
 */

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

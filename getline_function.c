#include "shell.h"

ssize_t _getline(char **line_ptr, size_t *size, int fd)
{
	static char buffer[BUF_SIZE];
	static char *buf_ptr;
	static ssize_t buf_size;
	ssize_t total = 0, bytes_to_cpy = 0;
	char *newline_pos = NULL, *new_ptr = NULL;


	if (!line_ptr || !size)
		return (-1);

	if (*line_ptr == NULL)
	{
		*line_ptr = malloc(1);
		if (*line_ptr == NULL)
			return (-1);

		**line_ptr = '\0';
		*size = 1;
	}

	do {
		if (buf_size == 0)
		{
			buf_size = read(fd, buffer, sizeof(buffer) - 1);
			if (buf_size <= 0)
			{
				if (buf_size == 0 && total == 0)
					return (-1);
				return (buf_size);
			}
			buf_ptr = buffer;
		}

		newline_pos = _memchr(buf_ptr, '\n', buf_size);
		bytes_to_cpy = (newline_pos != NULL
						 ? newline_pos - buf_ptr + 1
						 : buf_size);

		new_ptr = _realloc(*line_ptr, total, total + bytes_to_cpy + 1);
		if (!new_ptr)
			return (-1);

		*line_ptr = new_ptr;
		_memcpy(*line_ptr + total, buf_ptr, bytes_to_cpy);
		total += bytes_to_cpy;
		(*line_ptr)[total] = '\0';
		*size = total + 1;

		buf_ptr += bytes_to_cpy;
		buf_size -= bytes_to_cpy;
	} while (newline_pos == NULL);

	return (total);
}
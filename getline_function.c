#include "shell.h"


/**
 * read_buffer - is a helper function for _getline
 * it reads from the file
 * @fd: file descriptor to read from it
 * @buffer: to store read data
 * @buf_ptr: to help know where we stop reading
 * @buf_size: to update buffer size
 * Return: 1 on success, 0 otherwise
 */

static ssize_t read_buffer(int fd, char *buffer, char **buf_ptr,
						   ssize_t *buf_size)
{
	*buf_size = read(fd, buffer, sizeof(buffer) - 1);
	/*check if buffer was large enough to store chars*/
	if (*buf_size <= 0)
	{
		/*check if have read any data so far*/
		if (*buf_size == 0)
			return (-1);
		/*return number of bytes read*/
		return (*buf_size);
	}
	*buf_ptr = buffer;
	return (1);
}

/**
 * copy_line - helper function for _getline
 * it copyies spcific bytes from the static buffer
 * to  the line_ptr for the ueser
 * @line_ptr: to store line of text in it
 * @buf_ptr: to know where are we in the static buffer
 * @total: total number of bytes
 * @size: size of buffer to return
 * @bytes_to_cpy: number of bytes to copy to new line
 * Return: 0 on success, -1 otherwise
 */
static ssize_t copy_line(char **line_ptr, char *buf_ptr, ssize_t *total,
						 size_t *size, ssize_t bytes_to_cpy)
{
	/*reallocate space for existing and new number of bytes*/
	char *new_ptr = _realloc(*line_ptr, *total, *total + bytes_to_cpy + 1);

	if (!new_ptr)
		return (-1);

	*line_ptr = new_ptr;
	/*cpy new data from static buffer to line_ptr*/
	_memcpy(*line_ptr + *total, buf_ptr, bytes_to_cpy);
	/*update total to be total number of bytes read so far*/
	*total += bytes_to_cpy;
	(*line_ptr)[*total] = '\0';
	*size = *total + 1;
	return (0);
}

/**
 * _getline - reads text from a file and retuns line of text
 * @line_ptr: line pointer to store the line in it
 * @size: to store the size of allocated buffer for the line
 * @fd: file descriptor to read from
 * Return: number of bytes read or -1 otherwise
 */

ssize_t _getline(char **line_ptr, size_t *size, int fd)
{
	static char buffer[BUF_SIZE], *buf_ptr;
	static ssize_t buf_size;
	ssize_t total = 0, bytes_to_cpy = 0, status = 0;
	char *newline_pos = NULL;

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
			status = read_buffer(fd, buffer, &buf_ptr, &buf_size);
			if (status == -1)
				return (-1);
			if (status <= 0)
				return (buf_size);
		}
		/*searches for newline character*/
		newline_pos = _memchr(buf_ptr, '\n', buf_size);
		/*determine how much bytes we need to cpy to get a line*/
		bytes_to_cpy = (newline_pos != NULL ? newline_pos - buf_ptr + 1 : buf_size);
		if (copy_line(line_ptr, buf_ptr, &total, size, bytes_to_cpy) == -1)
			return (-1);
		/*move buffer pointer to be after copyied data*/
		/*decrease buffer size by read bytes*/
		buf_ptr += bytes_to_cpy;
		buf_size -= bytes_to_cpy;
		/*keep loop while not find the newline*/
	} while (newline_pos == NULL);
	return (total);
}

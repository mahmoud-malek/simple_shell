#include "shell.h"

/**
 * logical_sep - check if logical(||, &&, ;) exists
 * @line: to check it
 * Return: 2 if logical operators, 1 if (;), and 0 otherwise
 */

int logical_sep(char *line)
{
	if ((*line == '2' && *(line + 1) == '>' &&
		 *(line + 2) == '&' && *(line + 3) == '1'))
		return (4);

	else if ((*line == '2' && *(line + 1) == '>' && *(line + 2) == '>'))
		return (3);

	if ((*line == '|' && *(line + 1) == '|') ||
		(*line == '&' && *(line + 1) == '&') ||
		(*line == '>' && *(line + 1) == '>') ||
		(*line == '2' && *(line + 1) == '>') ||
		(*line == '&' && *(line + 1) == '>'))
		return (2);

	else if (*line == ';' || *line == '|' || *line == '<' || *line == '>')
		return (1);

	return (0);
}

/**
 * count_logical - counts commands seperated by ||, &&, ;
 * @line: string to count from
 * Return: number of commands separated by logical operators
 * or semi-colon
 */

int count_logical(char *line)
{
	int counter = 0;
	int len = 0, sep_len = 0;

	if (!line || *line == '\0')
		return (0);

	while (*line)
	{
		/*Count length of command*/
		len = 0;
		while (*(line + len) != '\0' && !logical_sep(line + len))
			len++;

		if (len > 0)
		{
			counter++;
			line += len;
		}

		/*For separators*/
		sep_len = logical_sep(line);
		if (sep_len > 0)
		{
			counter++;
			line += sep_len;
		}
	}

	return (counter);
}

/**
 * get_logical - build array of commands
 * @line: string the build from
 * @result: two dimensional array to store the result
 * Return: array of commands
 */

char **get_logical(char *line, char **result)
{
	int len = 0, i = 0;
	int sep_len = 0;

	while (*line)
	{
		/*count length of command*/
		len = 0;
		while (*(line + len) != '\0' && !logical_sep(line + len))
			len++;

		if (len > 0)
		{
			/*Allocate mem for command*/
			result[i] = malloc((len + 1) * sizeof(char));
			if (!result[i])
			{
				free_2D(result);
				return (NULL);
			}
			/*copy command*/
			_strncpy(result[i], line, len);
			result[i][len] = '\0';
			line += len;
			i++;
		}

		/*handle separators*/
		if (get_logical_helper(&line, &sep_len, result, &i) == -1)
			return (NULL);
	}

	result[i] = NULL;
	return (result);
}

/**
 * get_logical_helper - allocate space for command and copy it
 * @line: to copy from
 * @sep_len: length of the sperator, eg.(|| and && is 2, and ; is 1)
 * @result: the array of commands to store copies in it
 * @i: index to know where exactly to store the value
 * Return: 0 on success, and -1 otherwise
 */

int get_logical_helper(char **line, int *sep_len, char **result, int *i)
{
	*sep_len = logical_sep(*line);
	if (*sep_len > 0)
	{
		result[*i] = malloc(((*sep_len) + 1) * sizeof(char));
		if (!result[*i])
		{
			free_2D(result);
			return (-1);
		}

		_strncpy(result[*i], *line, *sep_len);
		result[*i][*sep_len] = '\0';
		*line += *sep_len;
		(*i)++;
	}

	return (0);
}

/**
 * split_logical - splits line into commands
 * @line: string contain text commands to split
 * Return: array of commands on success, NULL otherwise
 */

char **split_logical(char *line)
{
	int counter = 0;
	char **result;

	if (!line || *line == '\0')
		return (NULL);

	counter = count_logical(line);
	if (counter == 0)
		return (NULL);

	result = malloc((counter + 1) * sizeof(char *));
	if (!result)
		return (NULL);

	result = get_logical(line, result);
	if (!result)
	{
		return (NULL);
	}

	return (result);
}

#include "shell.h"

int logical_sep(char *line)
{
	if ((*line == '|' && *(line + 1) == '|') ||
		(*line == '&' && *(line + 1) == '&'))
		return (2);

	else if (*line == ';')
		return (1);

	return (0);
}

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

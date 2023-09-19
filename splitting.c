#include "shell.h"

int is_separator(const char c, const char *separator)
{
	int len = _strlen(separator);
	int i = 0;

	for (; i < len; i++)
		if (c == separator[i])
			return (1);

	return (0);
}

int count_cmd(char *line, const char *separator)
{
	int counter = 0;
	int in_word = 0;
	char *ptr_line = line;

	if (!line || *line == '\0')
		return (0);

	while (*ptr_line != '\0')
	{
		if (is_separator(*ptr_line, separator))
			in_word = 0;

		else if (!in_word)
			in_word = 1, counter++;

		ptr_line++;
	}

	return (counter);
}

char **get_cmd(char *line, char **commands, const char *separator)
{
	int ch = 0, cmd_len = 0, j = 0, i = 0;

	while (line[ch] != '\0')
	{
		/*Skipping trailling separators*/
		while (line[ch] != '\0' && is_separator(line[ch], separator))
			ch++;

		if (line[ch] != '\0')
		{
			/*Count length of command*/
			cmd_len = 0;
			while (line[ch + cmd_len] != '\0' &&
				   !is_separator(line[ch + cmd_len], separator))
				cmd_len++;

			/*Allocate memory for each command in the array*/
			commands[i] = malloc((cmd_len + 1) * sizeof(char));
			if (commands[i] == NULL)
			{
				free_2D(commands);
				return (NULL);
			}

			/*Copy the command to commands list*/
			for (j = 0; j < cmd_len; j++)
				commands[i][j] = line[ch + j];

			commands[i][j] = '\0';
			ch += cmd_len;
			(i)++; /*move to next command*/
		}
	}

	commands[i] = NULL;
	return (commands);
}

char **split(char *line, const char *separator)
{
	int counter = 0;
	char **command_list = NULL;

	if (!line || *line == '\0')
		return (NULL);

	counter = count_cmd(line, separator);

	if (counter == 0)
		return (NULL);

	command_list = malloc((counter + 1) * sizeof(char *));

	if (!command_list)
		return (NULL);

	command_list = get_cmd(line, command_list, separator);

	if (!command_list)
		return (NULL);

	return (command_list);
}

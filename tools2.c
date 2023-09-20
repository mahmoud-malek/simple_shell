#include "shell.h"

/**
 * variable_replacement_helper - replaces variables
 * @args: all arguemnts
 * @i: the ith command from the command list
 * @tmp: temporary for the command
 */
void variable_replacement_helper(ALL *args, int i, list *tmp)
{
	int j = 0;
	char *var = NULL, *value = NULL;

	for (j = 0; tmp->command[i][j] != '\0'; j++)
	{
		if (tmp->command[i][j] == '$')
		{
			j++;
			if (tmp->command[i][j] != '\0')
			{
				if (tmp->command[i][j] == '?')
				{
					value = _itoa(args->status);

					tmp->command[i] = replace_var(tmp->command[i], value, j - 1);
					free(value);
				}
				else if (tmp->command[i][j] == '$')
				{
					value = _itoa(getpid());
					tmp->command[i] = replace_var(tmp->command[i], value, j - 1);
					free(value);
				}
				else if (tmp->command[i][j] != '\0')
				{
					var = get_var_name(tmp->command[i], j);
					value = _getenv(var, args);
					if (value != NULL)
						tmp->command[i] = replace_var(tmp->command[i], value, j - 1);
					else
					{
						free(tmp->command[i]);
						tmp->command[i] = NULL;
					}
					free(var);
				}
			}
			break;
		}
	}
}

/**
 * variable_replacement - handles replacing envrionment variables
 * @args: Structure contain all wanted arguments
 */

void variable_replacement(ALL *args)
{
	list *tmp = args->commands;
	int i = 0;

	while (tmp != NULL && tmp->command != NULL)
	{
		for (i = 0; tmp->command[i] != NULL; i++)
			variable_replacement_helper(args, i, tmp);

		tmp = tmp->next;
	}
}

/**
 * get_var_name - gets variable name from a string
 * @str: string contain the variable
 * @idx: index of the beginning of the variable name
 * Return: allocated variable name or null otherwise
 */
char *get_var_name(char *str, int idx)
{
	char *var_name = NULL;
	int len = 0, i = 0;

	for (i = idx; str[i] != '\0' && !is_invalid(str[i]); i++)
		len++;

	var_name = malloc((len + 1) * sizeof(char));
	if (!var_name)
		return (NULL);

	for (i = 0; i < len; i++)
		var_name[i] = str[idx + i];
	var_name[len] = '\0';

	return (var_name);
}

/**
 * is_invalid - checks whether a char valid for the
 * variable name or not
 * @c: character to check
 * Return: 1 if invalid, 0 otherwise
 */

int is_invalid(char c)
{
	if (c == ' ' || c == '/' || c == ':' || c == '.' || c == '\0')
		return (1);
	return (0);
}

/**
 * replace_var - replacing a $"variable name"
 * @str: name of the variable
 * @value: value of the variable
 * @idx: index to specifiy where to start repalcing
 * Return: new_string with replaced variable, or null otherwise
 */
char *replace_var(char *str, char *value, int idx)
{
	char *new_str = NULL, *tmp = NULL;
	int len = 0, value_len = 0, i = 0, j = 0;
	int var_len = 0;

	tmp = get_var_name(str, idx);
	value_len = _strlen(value);
	/*+ 1 for $*/
	var_len = _strlen(tmp);
	free(tmp);

	len = _strlen(str) - var_len + value_len;

	new_str = malloc((len + 1) * sizeof(char));
	if (!new_str)
		return (NULL);

	for (i = 0; i < idx; i++)
		new_str[i] = str[i];

	for (j = 0; j < value_len; j++)
		new_str[i + j] = value[j];

	for (; str[i + var_len] != '\0'; i++)
		new_str[i + value_len] = str[i + var_len];
	new_str[len] = '\0';

	free(str);
	return (new_str);
}

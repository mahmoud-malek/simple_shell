#include "shell.h"

void variable_replacement(ALL *args)
{
	list *tmp = args->commands;
	int i = 0, j = 0;
	char *var = NULL, *value = NULL;

	while (tmp != NULL && tmp->command != NULL)
	{
		for (i = 0; tmp->command[i] != NULL; i++)
		{
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
		tmp = tmp->next;
	}
}

char *get_var_name(char *str, int idx)
{
	char *var_name = NULL;
	int len = 0, i = 0;

	for (i = idx; str[i] != '\0' && !is_invalid(str[i]); i++)
		len++;

	var_name = malloc((len + 1) * sizeof(char));
	if (!var_name)
		return NULL;

	for (i = 0; i < len; i++)
		var_name[i] = str[idx + i];
	var_name[len] = '\0';

	return (var_name);
}

int is_invalid(char c)
{
	if (c == ' ' || c == '/' || c == ':' || c == '.' || c == '\0')
		return (1);
	return (0);
}

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

int is_logical(char *cmd)
{
	return (!_strcmp(cmd, "&&") || !_strcmp(cmd, "||") || !_strcmp(cmd, ";"));
}

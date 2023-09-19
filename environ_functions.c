#include "shell.h"

int compare_env_name(char *str, const char *name)
{
	int i = 0;
	char *tmp = str;

	while (*str != '\0' && *str != '=')
	{
		if (*str == name[i])
			i++;
		str++;
	}
	if (*str == '=' && name[i] == '\0' && i == (str - tmp))
		return (i);
	return (0);
}

char *_getenv(const char *name, ALL *args)
{
	int i = 0, j = 0;
	char *str = NULL;
	char **tmp = args->envrion_cpy;

	if (!name)
		return (NULL);

	for (i = 0; tmp[i] != NULL; i++)
	{
		j = compare_env_name(tmp[i], name);
		if (j)
		{
			str = tmp[i] + j + 1;
			return (str);
		}
	}

	return (NULL);
}

void builtin_env(ALL *args)
{
	char **tmp = args->envrion_cpy;

	if (!tmp)
		return;

	while (*tmp != NULL)
	{
		_puts(*tmp);
		_putchar('\n');
		tmp++;
	}
}

void builtin_setenv(ALL *args)
{
	char *name = NULL, *value = NULL, *new_val = NULL;
	char **env = args->envrion_cpy;
	int i = 0;

	if (!args->commands->command[1] || !args->commands->command[2])
	{
		eputs("ERROR NULL name or value\n");
		return;
	}
	name = args->commands->command[1];
	value = args->commands->command[2];
	for (; env[i] != NULL; i++)
	{
		if (compare_env_name(env[i], name))
		{
			new_val = create_new_val(name, value);
			free(env[i]);
			env[i] = new_val;
			return;
		}
	}

	create_env(args, name, value, i);
}

void builtin_unsetenv(ALL *args)
{
	char *name = NULL, **env = args->envrion_cpy;
	int i = 0;

	if (!args->commands->command[1])
	{
		eputs("ERROR\n");
		return;
	}

	name = args->commands->command[1];

	for (; env[i] != NULL; i++)
	{
		if (compare_env_name(env[i], name))
		{
			free(env[i]);
			do {
				env[i] = env[i + 1];
				i++;
			} while (env[i] != NULL);

			return;
		}
	}

	eputs("ERROR no variable to unset\n");
}

#include "shell.h"

char *create_new_val(char *name, char *value)
{
	char *new_val = malloc(_strlen(name) + _strlen(value) + 2);

	if (!new_val)
	{
		eputs("ERROR setenv\n");
		return (NULL);
	}

	_strcpy(new_val, name);
	_strcat(new_val, "=");
	_strcat(new_val, value);

	return (new_val);
}

void create_env(ALL *args, char *name, char *value, int i)
{
	char *new_val = NULL;
	char **new_env = malloc((i + 2) * sizeof(char *));
	char **env = args->envrion_cpy;
	int j = 0;

	if (!new_env)
	{
		eputs("ERROR\n");
		return;
	}
	for (j = 0; j < i; j++)
	{
		new_env[j] = malloc((_strlen(env[j]) + 1) * sizeof(char));
		if (!new_env[j])
		{
			eputs("ERROR\n");
			return;
		}
		_strcpy(new_env[j], env[j]);
	}
	new_val = malloc((_strlen(name) + _strlen(value) + 2) * sizeof(char));
	if (!new_val)
	{
		for (j = 0; j < i + 2; j++)
			free(new_env[j]);
		free(new_env);

		eputs("ERROR\n");
		return;
	}

	_strcpy(new_val, name);
	_strcat(new_val, "=");
	_strcat(new_val, value);

	new_env[i] = new_val;
	new_env[i + 1] = NULL;
	free_2D(args->envrion_cpy);
	args->envrion_cpy = new_env;
}

void _setenv(char *name, char *value, ALL *args)
{
	char *new_val = NULL;
	char **env = args->envrion_cpy;
	int i = 0;

	if (!name || !value)
	{
		eputs("ERROR NULL name or value\n");
		return;
	}

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
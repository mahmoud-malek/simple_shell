#include "shell.h"

char *get_path(const char *name, ALL *args)
{
	int i = 0;
	char *path = NULL, *path_env = NULL;
	char **path_list = NULL;

	if (!name)
		return (NULL);

	if (*name == '/' || *name == '.')
		return (_strdup(name));

	path_env = _getenv("PATH", args);
	if (!path_env)
		return (NULL);

	path_env = _strdup(path_env);
	path_list = split(path_env, ":");
	if (!path_list)
	{
		free(path_env);
		return (NULL);
	}
	for (; path_list[i] != NULL; i++)
	{
		path = malloc((_strlen(path_list[i]) + _strlen(name) + 2) * sizeof(char));
		_strcpy(path, path_list[i]);
		_strcat(path, "/");
		_strcat(path, name);

		if (access(path, F_OK) == 0)
		{
			free(path_env);
			free_2D(path_list);
			return (path);
		}
		free(path);
	}
	free(path_env);
	free_2D(path_list);
	return (NULL);
}

void fork_command(ALL *args)
{

	pid_t child = 0;

	child = fork();
	if (child == -1)
	{
		perror("fork faild");
		exit(EXIT_FAILURE);
	}
	else if (child == 0)
	{
		if (execve(args->path, args->commands->command, args->envrion_cpy) == -1)
			print_error(args);
	}
	else
	{
		waitpid(child, &args->status, WUNTRACED);
		args->status = WEXITSTATUS(args->status);
	}

	free(args->path);
}

list *make_commands(list **commands, char *line)
{
	char **cmds = NULL;
	char **comand = NULL;
	int i = 0;

	cmds = split_logical(line);
	if (!cmds)
		return (NULL);

	for (; cmds[i] != NULL; i++)
	{
		comand = split(cmds[i], " ");
		if (comand == NULL)
		{
			free_list(*commands);
			free_2D(cmds);
			return NULL;
		}
		if (add_node(commands, comand) == NULL)
		{
			free_list(*commands);
			free_2D(cmds);
			free_2D(comand);
			return (NULL);
		}

		free_2D(comand);
		comand = NULL;
	}
	free_2D(cmds);
	return (*commands);
}

int is_logical(char *cmd)
{
	return (!_strcmp(cmd, "&&") || !_strcmp(cmd, "||") || !_strcmp(cmd, ";"));
}

void execute(ALL *args)
{
	void (*builtin_function)(ALL *) = NULL;

	if (args->commands == NULL)
	{
		free(args->line);
		return;
	}

	args->tmp = args->commands;
	while (args->commands != NULL)
	{
		if (is_logical(args->commands->command[0]))
		{
			args->was_operator = 1;
			args->operator= args->commands->command[0];
			args->commands = args->commands->next;
		}
		else if (args->was_operator)
		{
			if (_strcmp(args->operator, "&&") == 0 && args->status != 0)
				args->commands = args->commands->next;
			else if (_strcmp(args->operator, "||") == 0 && args->status == 0)
				args->commands = args->commands->next;

			args->was_operator = 0;
		}
		else
		{
			builtin_function = is_built_in(args);
			if (builtin_function != NULL)
				builtin_function(args);
			else if (!is_logical(args->commands->command[0]))
			{
				args->path = get_path(args->commands->command[0], args);
				if (!args->path)
					print_error(args);
				else
					fork_command(args);
			}
			args->commands = args->commands->next;
		}
	}
	free_list(args->tmp);
	free(args->line);
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
int is_invalid(char c)
{
	if (c == ' ' || c == '/' || c == ':' || c == '.' || c == '\0')
		return (1);
	return (0);
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
int main(int ac, char **av)
{
	ssize_t read_chars = 0;
	size_t size = 0;
	ALL args;

	initialize_arguments(&args, av);
	if (ac == 2)
		args.fd = open(av[1], O_RDONLY);

	if (args.fd == -1)
	{
		print_error(&args);
		free_2D(args.envrion_cpy);
		exit(ERROR_NOT_FOUND);
	}

	while (1)
	{

		if (interactive(ac))
			write(STDOUT_FILENO, args.prompt, _strlen(args.prompt));

		args.line = NULL;
		read_chars = _getline(&args.line, &size, args.fd);
		args.line_number++;

		if (read_chars == -1)
		{
			free(args.line);
			if (interactive(ac))
				putchar('\n');

			free_2D(args.envrion_cpy);
			exit(args.status);
		}

		if (read_chars == 1 && args.line[0] == '\n')
		{
			free(args.line);
			continue;
		}

		args.line[read_chars - 1] = '\0';
		remove_comments(&args);
		make_commands(&args.commands, args.line);
		variable_replacement(&args);
		execute(&args);
	}

	free_2D(args.envrion_cpy);
	if (args.fd != STDIN_FILENO)
		close(args.fd);

	return (args.status);
}

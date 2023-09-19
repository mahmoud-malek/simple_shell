#include "shell.h"

/**
 * get_path - gets the absolute path of a command
 * @name: name of the command
 * @args: structure contain all required arguments
 * Return: path if found, null otherwise
 */

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

/**
 * fork_command - make a clone to execute command
 *
 * @args: structure for all required arguments
 *
 * Description: execute the command and return nothing
 * or exit failur when can't fork
 */

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

/**
 * make_commands - build linked list of commands
 * @commands: head of linkd list to store
 * @line: the line of input
 * Return: list of commands or null otherwise
 */

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
			return (NULL);
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

/**
 * execute - execute the commnads
 * @args: structure contain all required arguments
 * Return: execute the command the return nothing
 */

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
			handle_operator(args);

		else if (args->was_operator)
			handle_operator_status(args);

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

/**
 * main - Enter point of the shell
 * @ac: number of args
 * @av: files or arguments
 * Return: status of executed command
 */

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

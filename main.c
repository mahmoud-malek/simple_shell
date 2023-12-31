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

void fork_command(ALL *args, int *pipe_in, int *pipe_out)
{
	pid_t child = 0;

	child = fork();
	if (child == -1)
	{
		perror("fork failed");
		exit(EXIT_FAILURE);
	}
	else if (child == 0)
	{
		if (pipe_in)
		{
			dup2(pipe_in[0], STDIN_FILENO);
			if (pipe_in[1] != -1)
				close(pipe_in[1]);
		}
		if (pipe_out)
		{
			dup2(pipe_out[1], STDOUT_FILENO);
			if (pipe_out[0] != -1)
				close(pipe_out[0]);
		}

		if (execve(args->path, args->commands->command, args->envrion_cpy) == -1)
		{
			print_error(args);
		}
	}
	else
	{
		if (pipe_in)
			close(pipe_in[0]);
		if (pipe_out)
			close(pipe_out[1]);

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

int is_redirection(char *cmd)
{
	if (!_strcmp(cmd, "<"))
		return (1);
	if (!_strcmp(cmd, ">"))
		return (2);
	if (!_strcmp(cmd, ">>"))
		return (3);
	if (!_strcmp(cmd, "2>"))
		return (4);
	if (!_strcmp(cmd, "2>>"))
		return (5);
	if (!_strcmp(cmd, "&>"))
		return (6);
	if (!_strcmp(cmd, "2>&1"))
		return (7);

	return (0);
}
int open_file(int type, const char *name)
{
	int fd = -1;

	if (type == 1)
		fd = open(name, O_RDONLY);

	if (type == 2 || type == 4)
		fd = open(name, O_WRONLY | O_CREAT | O_TRUNC, 0766);

	if (type == 3 || type == 5)
		fd = open(name, O_WRONLY | O_CREAT | O_APPEND, 0766);

	if (type == 6 || type == 7)
		fd = open(name, O_WRONLY | O_CREAT | O_APPEND, 0766);

	return (fd);
}

/**
 * execute - execute the commnads
 * @args: structure contain all required arguments
 * Return: execute the command the return nothing
 */

void execute(ALL *args)
{
	void (*builtin_function)(ALL *) = NULL;
	int pipe_fd[2], prev_fd[2] = {-1, -1}, r_fd[2] = {-1, -1};
	list *next = NULL;
	int found_pipe = 0, redirction = 0;

	if (args->commands == NULL)
	{
		free(args->line);
		return;
	}

	if (_strcmp(args->commands->command[0], "history") != 0)
		add_history(&args->hist, args->line);

	args->tmp = args->commands;
	while (args->commands != NULL)
	{
		next = args->commands->next;

		if (next)
			redirction = is_redirection(next->command[0]);

		if (is_logical(args->commands->command[0]))
			handle_operator(args);

		else if (args->was_operator)
			handle_operator_status(args);

		/*First pipe command (first pipe list)*/
		else if (!found_pipe && next && !_strcmp(next->command[0], "|"))
		{
			pipe(pipe_fd);
			args->path = get_path(args->commands->command[0], args);

			fork_command(args, NULL, pipe_fd);
			prev_fd[0] = pipe_fd[0];
			found_pipe = 1;
			args->commands = next->next;
		}

		/*middle pipe command (command has pipe before and after it)*/
		else if (found_pipe && next && !_strcmp(next->command[0], "|"))
		{
			pipe(pipe_fd);
			args->path = get_path(args->commands->command[0], args);
			fork_command(args, prev_fd, pipe_fd);
			prev_fd[0] = pipe_fd[0];
			args->commands = next->next;
		}

		/*Command has a pipe before and redirection after */
		else if (found_pipe && next && redirction > 1)
		{
			args->path = get_path(args->commands->command[0], args);
			r_fd[1] = open_file(redirction, next->next->command[0]);
			fork_command(args, prev_fd, r_fd);
			prev_fd[0] = r_fd[0];

			if (next->next->next)
				args->commands = next->next->next;
		}
		/*command has pipe after it and redirection before it*/
		else if (redirction == 1 && next && next->next && next->next->next
		 && !_strcmp(next->next->next->command[0], "|"))
		{
			pipe(pipe_fd);
			args->path = get_path(args->commands->command[0], args);
			r_fd[0] = open_file(redirction, next->next->command[0]);

			fork_command(args, r_fd, pipe_fd);
			prev_fd[0] = pipe_fd[0];
			found_pipe = 1;
			if (next->next->next->next)
				args->commands = next->next->next->next;
		}
		/*last pipe command (command has just pipe before it)*/
		else if (found_pipe)
		{
			args->path = get_path(args->commands->command[0], args);
			fork_command(args, prev_fd, NULL);
			found_pipe = 0;
			args->commands = next;
		}

		/*Handle redirections*/
		else if (next && is_redirection(next->command[0]))
		{
			redirction = is_redirection(next->command[0]);

			if (redirction != 1 && next->next)
			{
				r_fd[1] = open_file(redirction, next->next->command[0]);

				args->path = get_path(args->commands->command[0], args);
				fork_command(args, NULL, r_fd);
			}

			else if (redirction == 1 && next->next)
			{

				r_fd[0] = open_file(redirction, next->next->command[0]);

				args->path = get_path(args->commands->command[0], args);
				fork_command(args, r_fd, NULL);
			}

			args->commands = next->next->next;
		}

		else
		{
			builtin_function = is_built_in(args);
			if (builtin_function != NULL)
				builtin_function(args);

			else if (!is_logical(args->commands->command[0]))
			{
				check_alias(args);
				args->path = get_path(args->commands->command[0], args);
				if (!args->path)
					print_error(args);
				else
					fork_command(args, NULL, NULL);
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
		free_history(&args);
		exit(ERROR_NOT_FOUND);
	}
	while (1)
	{

		if (interactive(ac))
			write(STDOUT_FILENO, args.prompt, _strlen(args.prompt));

		args.line = NULL;
		args.line_number++;

		signal(SIGINT, sigint_handler);
		read_chars = _getline(&args.line, &size, args.fd);

		/*EOF: enf of file or Ctrl + D*/
		if (read_chars == -1)
		{
			free(args.line);
			if (interactive(ac))
				putchar('\n');

			free_2D(args.envrion_cpy);
			free_aliases_list(&args);
			free_history(&args);
			exit(args.status);
		}

		/*Empty line*/
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

	free_aliases_list(&args);
	free_2D(args.envrion_cpy);
	free_history(&args);

	if (args.fd != STDIN_FILENO)
		close(args.fd);

	return (args.status);
}

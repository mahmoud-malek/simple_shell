#include "shell.h"

/**
 * print_history - prints the history of the commands
 * entered from the user
 * @head: to traverse the list and print
 */

void print_history(HISTORY *head)
{
	HISTORY *tmp = head;

	if (!tmp)
		_putchar('\n');

	while (tmp != NULL)
	{
		_puts(tmp->line);
		_puts("\n");
		tmp = tmp->next;
	}
}

/**
 * add_history - adds new line to the history list
 * @head: head of linked list
 * @line: line to add to hisotry list
 */

void add_history(HISTORY **head, char *line)
{
	HISTORY *new_history = NULL, *tmp = *head, *last = NULL;

	while (tmp != NULL)
	{
		last = tmp;
		tmp = tmp->next;
	}

	new_history = malloc(sizeof(HISTORY));
	if (!new_history)
		_puts("ERROR cannot allocate memory for new_history\n");

	new_history->line = _strdup(line);
	new_history->next = NULL;

	if (last == NULL)
		*head = new_history;
	else
		last->next = new_history;
}

/**
 * builtin_history - handles history - print or delete
 * @args: contain linked list of history and other wanted
 * arguments
 */

void builtin_history(ALL *args)
{

	int i = 0, j = 0;

	/*case of print all aliases*/
	if (!args->commands->command[1] ||
		_strcmp(args->commands->command[1], "") == 0)
		print_history(args->hist);

	else
	{
		i = 1;
		j = 0;
		if (args->commands->command[i][j] == '-')
		{
			j++;
			while (args->commands->command[i][j] != '\0')
			{
				switch (args->commands->command[i][j])
				{
				case 'd':
					delete_history(&args->hist, _atoi_(args->commands->command[i + 1]));
					break;
				case 'c':
					free_history(args);
					break;
				default:
					break;
				}

				j++;
			}
		}

		else
		{
			_puts("Undefined or unsupported\n");
		}
	}
}

/**
 * free_history - frees the hitsory list
 * @args: have the linked list to free
 */

void free_history(ALL *args)
{
	HISTORY *current = args->hist;
	HISTORY *next = NULL;

	while (current != NULL)
	{
		next = current->next;
		free(current->line);
		free(current);

		current = next;
	}

	args->hist = NULL;
}

void delete_history(HISTORY **head, int index)
{
	HISTORY *tmp = *head, *prev = NULL;
	int len = 0, i = 0;

	while (tmp != NULL)
	{
		tmp = tmp->next;
		len++;
	}

	if (index > len)
	{
		_puts("Index out of range\n");
		return;
	}

	index = len - index;
	tmp = *head;

	if (index == 0)
	{
		*head = tmp->next;
		free(tmp->line);
		free(tmp);
		return;
	}

	for (; tmp != NULL && i < index; i++)
	{
		prev = tmp;
		tmp = tmp->next;
	}

	prev->next = tmp->next;
	free(tmp->line);
	free(tmp);
}

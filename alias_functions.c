#include "shell.h"

/**
 * print_alias_list - print the list of aliases
 * @args: contain the linked list to print
 */

void print_alias_list(ALL *args)
{
	ALIAS *traverse = args->aliases;

	if (!args->aliases)
		return;

	while (traverse != NULL)
	{
		_puts(traverse->name);
		_puts("=\'");
		_puts(traverse->value);
		_puts("\'\n");
		traverse = traverse->next;
	}
}

/**
 * print_alias - print specific alias
 * @head: to search for an alias in that linked list
 * @name: name of the alias to print
 */

void print_alias(ALIAS *head, char *name)
{
	ALIAS *tmp = head;

	while (tmp != NULL)
	{
		if (_strcmp(tmp->name, name) == 0)
		{
			_puts(tmp->name);
			_puts("=\'");
			_puts(tmp->value);
			_puts("\'\n");
			return;
		}
		tmp = tmp->next;
	}

	_puts("alias: ");
	_puts(name);
	_puts(": not found\n");
}

/**
 * add_alias - add alias to linked list of aliases
 * @head: head of linked list
 * @name: name of the new node to add
 * @value: value of the new node to add
 */

void add_alias(ALIAS **head, char *name, char *value)
{
	ALIAS *new_alias = NULL, *tmp = *head, *last = NULL;

	while (tmp != NULL)
	{
		if (_strcmp(tmp->name, name) == 0)
		{
			free(tmp->value);
			tmp->value = _strdup(value);
			return;
		}

		last = tmp;
		tmp = tmp->next;
	}

	new_alias = malloc(sizeof(ALIAS));
	if (!new_alias)
		_puts("ERROR MALLOC on add alias\n");

	new_alias->name = _strdup(name);
	new_alias->value = _strdup(value);
	new_alias->next = NULL;

	if (last == NULL)
		*head = new_alias;
	else
		last->next = new_alias;
}

/**
 * builtin_alias - handles aliases (add, overwrite, print)
 * @args: contain linked list of aliases and other wanted
 * arguments
 */

void builtin_alias(ALL *args)
{
	char *name = NULL, *value = NULL, **arr = NULL;
	int i = 0, j = 0, set = 0;

	/*case of print all aliases*/
	if (!args->commands->command[1] ||
	 _strcmp(args->commands->command[1], "") == 0)
		print_alias_list(args);

	else
	{
		for (i = 1; args->commands->command[i] != NULL; i++)
		{
			set = 0;
			for (j = 0; args->commands->command[i][j] != '\0'; j++)
			{
				if (args->commands->command[i][j] == '=')
				{
					set = 1;
					break;
				}
			}

			if (set)
			{
				arr = split(args->commands->command[i], "=");
				name = arr[0];
				value = arr[1];
				add_alias(&(args->aliases), name, value);
				free_2D(arr);
			}
			else
			{
				name = args->commands->command[i];
				print_alias(args->aliases, name);
			}
		}
	}
}

/**
 * free_aliases_list - free linked list of aliases
 * @args: have the linked list to free
 */

void free_aliases_list(ALL *args)
{
	ALIAS *current = args->aliases;
	ALIAS *next = NULL;

	while (current != NULL)
	{
		next = current->next;

		free(current->name);
		free(current->value);
		free(current);

		current = next;
	}

	args->aliases = NULL;
}

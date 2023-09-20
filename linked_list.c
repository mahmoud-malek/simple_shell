#include "shell.h"

/**
 * add_node - adds node to the list of commands
 * @head: the first node in the list
 * @command: command to add
 * Return: pointer to add node
 */
list *add_node(list **head, char **command)
{
	list *node = malloc(sizeof(list));
	list *temp = *head;

	if (node == NULL)
		return (NULL);

	if (command != NULL)
		node->command = _strdup2D(command);

	if (node->command == NULL)
	{
		free(node);
		return (NULL);
	}

	node->next = NULL;
	if (*head == NULL)
	{
		*head = node;
		return (node);
	}

	while (temp->next != NULL)
	{
		temp = temp->next;
	}

	temp->next = node;
	return (*head);
}

/**
 * free_list - frees a linked list of commands
 * @head: first node to start free from
 */

void free_list(list *head)
{
	list *temp = NULL;

	while (head != NULL)
	{

		free_2D(head->command);
		temp = head;
		head = head->next;
		free(temp);
	}

	free(head);
}

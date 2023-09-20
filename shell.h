#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

#define min(a, b) ((a) < (b) ? (a) : (b))
#define MAX_INT 2147483647
#define BUF_SIZE 1024
#define ERROR_NOT_FOUND 127

extern char **environ;

/**
 * struct list_alias - is linked list of aliases
 * @name: name of the alias
 * @value: value of the alias
 * @next: pointer to next node
 */
typedef struct list_alias
{
	char *name;
	char *value;

	struct list_alias *next;
} ALIAS;

/**
 * struct _commands_ - is a linked list of commands
 * @command: a command
 * @next: is the next node
 * Description: linked list for commands
 */

typedef struct _commands_
{
	/*a command and its arguments*/
	char **command;
	struct _commands_ *next;

} list;

/**
 * struct all_arguments - structure contain all variables
 * @commands: linked list of commands
 * @tmp: var to hold the head of the linked list
 * @av: argument of the main function
 * @envrion_cpy: copy from the environment variables
 * @shell_name: name of the shell
 * @path: path of the command
 * @line: the line input
 * @prompt: name to display on the termainl
 * @operator: to store type of logical operator
 * @was_operator: to check if there was an operator
 * @status: status of the execution of commands
 * @line_number: the nubmer of the current command
 * @fd: file descriptor
 * @aliases: linked list of aliases
 * Description: All variables needed in the shell
 */

typedef struct all_arguments
{
	ALIAS *aliases;
	list *commands;
	list *tmp;
	char **av;
	char **envrion_cpy;
	char *shell_name;
	char *path;
	char *line;
	char *prompt;
	char *operator;
	int was_operator;
	int status;
	int line_number;
	int fd;
} ALL;

/**
 * struct built_in_table - table of built-in commands
 * @name: name of the builtin command
 * @function: approprotiate function
 * Description: used to check if the command is built-in
 */
typedef struct built_in_table
{
	char *name;
	void (*function)(ALL *);
} built_in;

/*Command splitting functions*/
int count_cmd(char *str, const char *separator);
char **get_cmd(char *line, char **commands, const char *separator);
char **split(char *line, const char *separator);
int is_separator(const char c, const char *separator);
/*Logical splitting functions based on  (&&, ||, ;) */
int logical_sep(char *line);
int count_logical(char *line);
char **get_logical(char *line, char **result);
char **split_logical(char *line);
int get_logical_helper(char **line, int *sep_len, char **result, int *i);

/*String manipulation Functions*/
char *_strcpy(char *dest, char *src);
int _strcmp(char *str1, char *str2);
char *_strcat(char *dest, const char *src);
size_t _strlen(const char *str);
char *_strdup(const char *str);
char **_strdup2D(char **str);
char *_strncpy(char *dest, char *src, int n);
char *_strncat(char *dest, char *src, int n);

/*Path finder Functions*/
char *get_path(const char *name, ALL *args);
void fork_command(ALL *args);
list *make_commands(list **commands, char *line);

/*Memory Functions*/
void free_2D(char **array);
char *_memcpy(char *dest, char *src, unsigned int n);
void *_memchr(const void *str, int ch, size_t n);
void *_realloc(void *buffer, unsigned int old_siz, unsigned int new_siz);
void free_list(list *head);

/*Tools*/
int _atoi_(char *str);
char *_itoa(int num);
void initialize_arguments(ALL *args, char **av);
void remove_comments(ALL *args);
char *get_var_name(char *str, int idx);
int is_invalid(char c);
void variable_replacement(ALL *args);
void variable_replacement_helper(ALL *args, int i, list *tmp);
char *replace_var(char *str, char *value, int idx);
int is_logical(char *cmd);
void handle_operator(ALL *args);
void handle_operator_status(ALL *args);

/*writing functions*/
int _putchar(char c);
int eputchar(char c);
int _puts(char *str);
int eputs(char *str);
void print_error(ALL *args);
void print_exit_error(ALL *args);
void print_error_cd(ALL *args);

void builtin_exit(ALL *args);
void builtin_cd(ALL *args);

void (*is_built_in(ALL *args))(ALL *);
ssize_t _getline(char **line_ptr, size_t *size, int fd);

list *add_node(list **head, char **command);

void execute(ALL *args);
int interactive(int ac);

/*Evnronment functions*/
char *_getenv(const char *name, ALL *args);
void builtin_env(ALL *args);
void builtin_setenv(ALL *args);
void builtin_unsetenv(ALL *args);
char *create_new_val(char *name, char *value);
void create_env(ALL *args, char *name, char *value, int i);
void _setenv(char *name, char *value, ALL *args);
int compare_env_name(char *str, const char *name);

/*Aliases functions*/
void add_alias(ALIAS **head, char *name, char *value);
void print_alias(ALIAS *head, char *name);
void print_alias_list(ALL *args);
void builtin_alias(ALL *args);
void free_aliases_list(ALL *args);
void check_alias(ALL *args);

#endif /*Shell Header*/

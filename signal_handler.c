#include "shell.h"

void sigint_handler(__attribute__((unused)) int singal)
{
	_puts("\n#cisfun ");
}

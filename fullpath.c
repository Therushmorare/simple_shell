#include "shell.h"

/**
 * fullpath - string to call execve on
 * @argv: argv
 * @path: PATH
 *
 * Return: fullpath
 */

char *fullpath(char **argv, char *path)
{
	char *token, *fullpath, *path_copy = NULL;
	int pathflag = 0, path_len = 0, count = 0, token_len = 0;
       	struct stat st;

	fullpath = (char *)malloc(sizeof(char) * 1024);
	if (fullpath == NULL)
	{
		fprintf(stderr, "Memory allocation failed.\n");
		exit(1);
	}
	path_copy = strdup(path);
	path_len = countPATH(path_copy);
	token = strtok(path_copy, "=");
	while (token)
	{
		snprintf(fullpath, sizeof(fullpath), "%s/%s", token, argv[0]);

		if (stat(fullpath, &st) == 0)
		{
			pathflag = 1;
			break;
		}
		if (count < path_len - 2)
		{
			token_len = strlen(token);
			if (token[token_len + 1] == ':')
			{
				if (stat(argv[0], &st) == 0)
				{
					strcpy(fullpath, argv[0]);
					pathflag = 1;
					break;
				}
			}
		}
		count++;
		token = strtok(NULL, ":");
	}
	if (pathflag == 0)
	{
		strcpy(fullpath, argv[0]);
	}
	free(path_copy);
	return (fullpath);


}


/**
 * childprocess - function responsible for execution
 * @argv: argv
 * @command: command from getline
 * @fullpath: command from fullpath
 *
 * Return: 0
 */

int childprocess(char **argv, char *command, char *fullpath)
{
	int i, exec, status, exit_status = 0;
	pid_t pid;

	pid = fork();
	if (pid == -1)
	{
		fprintf(stderr, "fork failed\n");
		exit(1);
	}
	else if (pid == 0)
	{
		exec = execve(fullpath, argv, environ);
		if (exec == -1)
		{
			fprintf(stderr, "./shell: No such file or directory\n");
			i = 0;
			for (i = 0; argv[i] != NULL; i++)
			{
				free(argv[i]);
				argv[i] = NULL;
			}
			free(argv);
			free(command);
			exit(127);
		}
		else
		{
			wait(&status);
			exit_status = WEXITSTATUS(status);
		}
		i = 0;
		for (i = 0; argv[i] != NULL; i++)
		{
			free(argv[i]);
			argv[i] = NULL;
		}
		free(argv);
		free(command);
	}
	return (exit_status);
}



/**
 * countPATH - counts the number of PATH members
 * @str: pointer to string to count
 *
 * Return: number of PATH members
 */

int countPATH(char *path)
{
    int i;
    int searchflag = 1;
    int count = 0;

    for (i = 0; path[i]; i++)
    {
        if (path[i] != ':' && searchflag == 1)
        {
            count += 1;
            searchflag = 0;
        }
        if (path[i + 1] == ':')
        {
            searchflag = 1;
        }
    }
    return (count);
}

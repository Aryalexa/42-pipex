#include "../libftmod/libft/libft.h"
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <sys/types.h>
// #include <sys/wait.h>
// #include <unistd.h>
#include <fcntl.h>

void	my_exit(char *msg)
{
	perror(msg);
	exit(1);
}

char	*find_path(char *cmd, char *env[])
{
	int		i;
	char	**paths;
	char	*cmd_path;
	char	*path_i;

	i = 0;
	while (ft_strnstr(env[i], "PATH", 4) == NULL)
		i++;
	paths = ft_split(env[i] + 5, ':');
	i = 0;
	while (paths[i])
	{
		path_i = ft_strjoin(paths[i], "/");
		cmd_path = ft_strjoin(path_i, cmd);
		free(path_i);
		if (access(cmd_path, F_OK) == 0)
			return (cmd_path);
		free(cmd_path);
		i++;
	}
	ft_free_arrstr(paths);
	return (NULL);
}
void print2_cur_cmd(char *cmd)
{
	write(2, "while executing ", 16);
	write(2, cmd, ft_strlen(cmd));
	write(2, "...\n", 4);
}

void	my_exec(char *cmd, char *env[])
{
	char	**cmd_args;
	char	*cmd_path;
	char	*cmd_name;

	if (!cmd | !*cmd)
		my_exit("unexpected command, null or empty");
	cmd_args = ft_split(cmd, ' ');
	cmd_name = cmd_args[0];
	cmd_path = find_path(cmd_name, env);
	if (!cmd_path)
	{
		ft_free_arrstr(cmd_args);
		print2_cur_cmd(cmd);
		my_exit("command path not found");
	}
	cmd_args[0] = cmd_path;
	execve(cmd_args[0], cmd_args, (char *const *)env);
	my_exit("exec error");
}

/**
 * it creates a pipe so the output of the command is redirected to the pipe, 
 * and the parent can have access to it. The parent should set the read from 
 * the pipe by default, so the next execution reads from it. The parent should 
 * wait for child to terminate, so it is finished before more commands execute.
 * child: > pipe, exec
 * parent: < pipe, wait
*/
void	my_pipex(char *cmd, char *env[])
{
	int	pipefd[2];
	int	pid;
	int	status;

	if (pipe(pipefd) == -1)
		my_exit("pipe could not be created");
	pid = fork();
	if (pid < 0)
		my_exit("fork error");
	if (pid == 0)
	{
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		my_exec(cmd, env);
	}
	else
	{
		close(pipefd[1]);
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
		if (wait(&status) == -1)
			my_exit("wait error");
		if (!WIFEXITED(status) || WEXITSTATUS(status) != 0)
		{
			print2_cur_cmd(cmd);
			my_exit("child did not success");
		}
	}
}

void	init_pipex(char *infile, char *outfile, char *cmds[], char *envp[])
{
	int		fdin;
	int		fdout;
	int		i;

	fdin = open(infile, O_RDONLY);
	if (fdin < 0)
		my_exit("input file error at open");
	dup2(fdin, STDIN_FILENO);
	close(fdin);
	i = 0;
	while (cmds[i] && cmds[i + 1])
		my_pipex(cmds[i++], envp);
	fdout = open(outfile, O_RDWR | O_CREAT, 0777);
	if (fdout < 0)
		my_exit("output file error at open");
	dup2(fdout, STDOUT_FILENO);
	my_exec(cmds[i], envp);
	close(fdout);
}

int	main(int argc, char *argv[], char *envp[])
{
	char	*outfile;

	if (argc >= 5)
	{
		outfile = argv[argc - 1];
		argv[argc - 1] = NULL;
		init_pipex(argv[1], outfile, &argv[2], envp);
	}
	else
		ft_printf("Usage: ./pipex infile cmd1 cmd2 [.. cmdn] outfile");
}

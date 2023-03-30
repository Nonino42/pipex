/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hanmpark <hanmpark@student.42nice.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/25 11:33:58 by hanmpark          #+#    #+#             */
/*   Updated: 2023/03/30 11:15:52 by hanmpark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include "errors.h"

/* Checks if the file exists */
static void	checkInfile(char *Infile)
{
	int	fd;

	fd = open(Infile, O_RDONLY);
	if (fd == -1)
		ft_error(ERR_OPEN);
}

/* Creates a new child process */
static int	createProcess(t_cmd *data)
{
	int	pid;

	pid = fork();
	if (pid == -1)
	{
		ft_freestr_array(data->cmdPath);
		ft_error(ERR_FORK);
	}
	return (pid);
}

/* Pipex main function */
static void	pipex(t_cmd *data, char **argv, char **envp)
{
	int	pid;

	if (pipe(data->pipe) == -1)
		ft_error(ERR_PIPE);
	checkInfile(argv[1]);
	defineCommand(data, argv, envp);
	pid = createProcess(data);
	if (pid == 0)
		writeEnd(data, argv, envp);
	pid = createProcess(data);
	if (pid == 0)
		readEnd(data, argv, envp);
	close(data->pipe[0]);
	close(data->pipe[1]);
	waitpid(pid, NULL, 0);
}

int	main(int argc, char **argv, char **envp)
{
	t_cmd	data;

	if (argc != 5)
	{
		errno = E2BIG;
		ft_error(ERR_ARGS);
	}
	data.nbrCommands = argc - 3;
	pipex(&data, argv, envp);
	return (0);
}

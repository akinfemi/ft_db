/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   db_load.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakin-al <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/19 16:53:25 by aakin-al          #+#    #+#             */
/*   Updated: 2017/04/27 18:43:08 by jkalia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_db.h"

int	db_loaddb(t_server *server)
{
	if (server->dbpath == NULL)
		server->dbpath = strdup("./db");
	db_reply(server, "Database Path = %s\n", server->dbpath);
	if (mkdir(server->dbpath, 0777) == -1)
	{
		if (errno == EEXIST)
			server->flag_db_load = true;
		else
			perror("MKDIR ERROR");
	}
	else
		server->flag_db_load = true;
	return (0);
}

int	db_loadtbl(t_server *server)
{
	struct stat	st;

	CHK1(server->flag_db_load == false, db_msg(server, MSG_DB_MISSING), 0);
	if (server->tblpath == NULL)
		server->tblpath = strdup("./tbl");
	db_reply(server, "Table Path = %s\n", server->tblpath);
	if (mkdir(server->tblpath, 0777) == -1)
	{
		if (errno == EEXIST)
			server->flag_tbl_load = true;
		else
			perror("MKDIR ERROR");
	}
	else
		server->flag_tbl_load = true;
	server->logpath = ft_strjoin(server->tblpath, "/");
	server->logpath = ft_strjoinf(server->logpath, "log", STRJOIN_FREE_SRC1);
	//db_reply(server, "Log Path = %s\n", server->logpath);
	/**
	if (stat(client->logpath, &st) == -1)
		if (open(client->logpath, O_CREAT) == -1)
			perror("OPEN ERROR");
	**/
	return (0);
}

char	*cleanstr(char *str)
{
	int		i;
	i = 0;

	while (isalnum(str[i]) != 0)
		i++;
	str[i] = 0;
	return (str);
}

int	db_load(t_server *server)
{
	char		*tmp;

	CHK1(server->argc != 3, db_reply(server, "usage: LOAD [--database || --table] name\n"), 0);
	tmp = cleanstr(server->args[2]);
	if (strcmp(server->args[1], "--database") == 0)
	{
		ft_strclr(server->dbpath);
		server->dbpath = ft_strjoin("./", tmp);
		return (db_loaddb(server));
	}
	else if (strcmp(server->args[1], "--table") == 0)
	{
		CHK1(server->flag_db_load == false, db_msg(server, MSG_DB_MISSING), 0);
		ft_strclr(server->tblpath);
		server->tblpath = ft_strjoin(server->dbpath, "/");
		server->tblpath = ft_strjoinf(server->tblpath, tmp, STRJOIN_FREE_SRC1);
		return (db_loadtbl(server));
	}
	else
		return (db_reply(server, "usage: LOAD [--database || --table] name\n"));
}

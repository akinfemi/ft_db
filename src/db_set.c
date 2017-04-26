/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   db_set.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jkalia <jkalia@student.42.us.org>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/22 20:51:30 by jkalia            #+#    #+#             */
/*   Updated: 2017/04/23 23:15:54 by jkalia           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <ft_db.h>

int			db_set(t_server *server)
{
	char		*hash;
	char		*filename;
	FILE		*fp;

	CHK1(server->flag_db_load == false, db_msg(MSG_DB_MISSING), 0);
	CHK1(server->flag_tbl_load == false, db_msg(MSG_TBL_MISSING), 0);
	CHK1(server->argc != 3, printf("usage: SET key value\n"), 0);
	hash = db_gethash(server, server->args[1]);
	filename = ft_strjoinf("/", hash, STRJOIN_FREE_SRC2);
	filename = ft_strjoinf(server->tblpath, filename, STRJOIN_FREE_SRC2);
	CHK2((fp = fopen(filename, "w+")) == NULL, free(filename), perror("FOPEN ERROR: "), -1);
	CHK2(fputs(server->args[2], fp) == EOF, free(filename), perror("FPUTS ERROR"), -1);
	CHK2((fclose(fp) == EOF), free(filename), perror("FCLOSE ERROR"), -1);
	printf("Record Saved\n");
	free(filename);
	return (0);
}

static int	db_get_print(FILE *fp, size_t size)
{
	char	*buf;

	buf = (char *)malloc(size);
	bzero(buf, size);
	CHK2(fgets(buf, size, fp) == NULL, free(buf), perror("FGETS ERROR"), -1);
	printf("Record Value:\n%s\n", buf);
	free(buf);
	return (0);
}

int			db_get(t_server *server)
{
	char		*filename;
	FILE		*fp;
	int		fd;
	struct stat	st;

	CHK1(server->flag_db_load == false, db_msg(MSG_DB_MISSING), 0);
	CHK1(server->flag_tbl_load == false, db_msg(MSG_TBL_MISSING), 0);
	CHK1(server->argc != 2, printf("usage: GET key\n"), 0);
	filename = ft_strjoinf("/", db_gethash(server, server->args[1]), STRJOIN_FREE_SRC2);
	filename = ft_strjoinf(server->tblpath, filename, STRJOIN_FREE_SRC2);
	CHK2(((fp = fopen(filename, "r")) == NULL), free(filename), perror("FOPEN ERROR"), -1);
	fd = fileno(fp);
	CHK2(fstat(fd, &st) == -1, free(filename), perror("FSTAT ERROR"), -1);
	db_get_print(fp, st.st_size * 2);
	CHK2(fclose(fp) == EOF, free(filename), perror("FCLOSE ERROR"), -1);
	free(filename);
	return (0);
}
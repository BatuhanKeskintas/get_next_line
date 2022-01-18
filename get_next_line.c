/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkeskint <bkeskint@student.42wolfsburg.de> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/07 23:42:14 by bkeskint          #+#    #+#             */
/*   Updated: 2021/06/28 15:44:30 by bkeskint         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"get_next_line.h"

static char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*join;

	join = (char *)malloc ((ft_strlen(s1) + ft_strlen(s2) + 1) * sizeof(char));
	if (join == NULL)
		return (NULL);
	else
	{
		ft_strlcpy(join, s1, ft_strlen (s1) + 1);
		ft_strlcpy(join + ft_strlen (s1), s2, ft_strlen (s2) + 1);
	}
	return (join);
}

static char	*ft_call_join(char *temp, char *buf)
{
	char	*s;

	if (buf)
	{
		s = ft_strjoin(buf, temp);
		free(buf);
		buf = ft_strdup(s);
		free(s);
	}
	else
		buf = ft_strdup(temp);
	return (buf);
}

static char	*ft_helper(char *buf, char **line, int byte_read)
{
	unsigned int	i;
	char			*s;

	i = 0;
	while (buf[i])
	{
		if (buf[i] == '\n')
			break ;
		i++;
	}
	if (i < ft_strlen(buf))
	{
		*line = ft_substr(buf, 0, i);
		s = ft_substr(buf, i + 1, ft_strlen(buf));
		free(buf);
		buf = ft_strdup(s);
		free (s);
	}
	else if (byte_read == 0)
	{
		*line = buf;
		buf = NULL;
	}
	return (buf);
}

int	check(int fd, char **line, char **buf, char *temp)
{
	int	byte_read;

	byte_read = read(fd, temp, BUFFER_SIZE);
	while (byte_read)
	{
		if (byte_read == -1)
			return (-1);
		temp[byte_read] = '\0';
		buf[fd] = ft_call_join (temp, buf[fd]);
		if (ft_strchr (temp, '\n'))
			break ;
		byte_read = read (fd, temp, BUFFER_SIZE);
	}
	if (byte_read < 1 && !buf[fd])
	{
		*line = ft_strdup("");
		return (byte_read);
	}
	buf[fd] = ft_helper(buf[fd], line, byte_read);
	if (byte_read < 1 && !buf[fd])
		return (byte_read);
	return (-2);
}

int	get_next_line(int fd, char **line)
{
	int				byte_read;
	char			temp[BUFFER_SIZE + 1];
	static char		*buf[MAX_FD];

	if (fd < 0 || fd > MAX_FD || BUFFER_SIZE <= 0 || line == NULL)
		return (-1);
	byte_read = check (fd, line, buf, temp);
	if (byte_read != -2)
		return (byte_read);
	return (1);
}

#include <stdlib.h>
#include <unistd.h>
#include "get_next_line.h"

size_t	ft_strlen(const char *s)
{
	int	i;

	if (!s)
		return (0);
	i = 0;
	while (s[i])
		i++;
	return (i);
}

int	contains_new_line(char	*buffer)
{
	if (!buffer)
		return (0);
	while (*(buffer)++)
		if (*buffer == '\n')
			return (1);
	return (0);
}

void	lst_add_back(t_list **remainder, char *buffer)
{
	t_list	*tmp;
	t_list	*new;

	new = malloc(sizeof(t_list));
	if (!new)
		return;
	new->content = buffer;
	new->next = NULL;
	new->len = ft_strlen(buffer);
	if (!new)
		return;
	if (*remainder)
	{
		tmp = *remainder;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
	else
		*remainder = new;
}

char	*sub_str(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	size_t	new_len;
	char	*res;

	if (!s)
		return (NULL);
	i = 0;
	new_len = ft_strlen(s) - start;
	if (start > ft_strlen(s))
		new_len = 0;
	else if (len < new_len)
		new_len = len;
	res = malloc(sizeof(char) * (new_len + 1));
	if (!res)
		return (NULL);
	while (i < new_len && s[start])
	{
		res[i] = s[start];
		i++;
		start++;
	}
	res[i] = 0;
	return (res);
}

void	separate_last(t_list **remainder, char *buffer)
{
	char	*to_new_line;
	char	*after_new_line;
	int	len;

	if (!buffer)
		return;
	len = 0;
	while (*(buffer++) != '\n')
		len++;
	len++;
	to_new_line = sub_str(buffer - len, 0, len);
	if (!to_new_line)
		return;
	len = 0;
	while (*(buffer++))
		len++;
	after_new_line= sub_str(buffer - len - 1, 0, len);
	if (!after_new_line)
		return;
	lst_add_back(remainder, to_new_line);
	lst_add_back(remainder, after_new_line);
}

void	*memcpy(void *dest, const void *src, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		((char *) dest)[i] = ((char *) src)[i];
		i++;
	}
	return (dest);
}

void	fill_remainder(int fd, t_list **remainder, int *read_int)
{
	char	*buffer;

	buffer = malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (*remainder && (*remainder)->content)
		buffer = memcpy(buffer, (*remainder)->content, (*remainder)->len);
	if (!buffer)
		return;
	while (!contains_new_line(buffer))
	{
		*read_int = read(fd, buffer, BUFFER_SIZE);
		buffer[*read_int] = 0;
		lst_add_back(remainder, buffer);
	}
	separate_last(remainder, buffer);
}

char	*concatenate_all_strings(t_list **remainder)
{
	size_t	total_len;
	t_list	*tmp;
	char	*res;
	int	i;

	total_len = 0;
	tmp = *remainder;
	while (tmp->next)
	{
		total_len += tmp->len;
		tmp = tmp->next;
	}
	res = malloc(sizeof(char) * total_len);
	if (!res)
		return (NULL);
	res[0] = 0;
	tmp = *remainder;
	while (tmp->next)
	{
		i = 0;
		while (tmp->content[i++])
			res[ft_strlen(res)] = tmp->content[i];
		tmp = tmp->next;
	}
	return (res);
}

void	clear_remainder(t_list **remainder)
{
	t_list	*tmp;

	while (remainder && (*remainder)->next)
	{
		tmp = (*remainder)->next;
		free((*remainder)->content);
		free(*remainder);
		*remainder = tmp;
	}
}

char	*get_next_line(int fd)
{
	char	*res;
	static t_list	*remainder;
	int	read_int;

	if (fd < 0 || BUFFER_SIZE < 0)
		return (NULL);
	fill_remainder(fd, &remainder, &read_int);
	res = concatenate_all_strings(&remainder);
	clear_remainder(&remainder);
	return (res);
}


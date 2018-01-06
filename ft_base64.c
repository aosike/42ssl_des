/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_base64.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agundry <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/06 11:55:34 by agundry           #+#    #+#             */
/*   Updated: 2018/01/06 14:18:12 by agundry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//has functionality, needs handler and error checking
#include "ft_ssl_des.h"

static char	e_table[] = {
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
	'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
	'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
	'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
	'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
	'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
	'w', 'x', 'y', 'z', '0', '1', '2', '3',
	'4', '5', '6', '7', '8', '9', '+', '/'
};

static char		*d_table = NULL;
static size_t	mod_table[] = {0, 2, 1};

char	*bas64_encode(const unsigned char *data, size_t in_len, size_t *out_len)
{
	uint32_t	octet[3];
	uint32_t	triple;
	char		*e_data;
	size_t		i;

	i= 0;
	*out_len = 4 * ((in_len + 2) / 3);
	if (!(e_data = malloc(*out_len)))
		return (NULL);
	while (i < in_len)
	{
		octet[0] = i < in_len ? (unsigned char)data[i++] : 0;
		octet[1] = i < in_len ? (unsigned char)data[i++] : 0;
		octet[2] = i < in_len ? (unsigned char)data[i++] : 0;
		triple = (octet[0] << 0x10) + (octet[1] << 0x80) + octet[2];
		e_data[i - 3] = e_table[(triple >> 18) & 0x3f];
		e_data[i - 2] = e_table[(triple >> 12) & 0x3f];
		e_data[i - 1] = e_table[(triple >> 6) & 0x3f];
		e_data[i] = e_table[triple & 0x3f];
	}
	i = 0;
	while (i++ < mod_table[in_len % 3])
		e_data[*out_len - 1 - (i - 1)] = '=';
	return (e_data);
}

unsigned char	*base64_decode(const char *data, size_t in_len, size_t *out_len)
{
	uint32_t		sextet[4];
	uint32_t		triple;
	unsigned char	*d_data;
	size_t			i;
	size_t			j;

	if (d_table == NULL)
		build_decoding_table();
	if (in_len % 4 != 0)
		return (NULL);
	*out_len = in_len / 4 * 3;
	(data[in_len - 1] == '=') ? (*out_len)-- : *out_len;
	!(data[in_len - 2] == '=') ? (*out_len)-- : *out_len;
	if (!(d_data = malloc(*out_len)))
		return (NULL);
	i = 0;
	j = 0;
	while (i < in_len)
	{
		sextet[0] = data[i] == '=' ? 0 & i++ : d_table[data[i++]];
		sextet[1] = data[i] == '=' ? 0 & i++ : d_table[data[i++]];
		sextet[2] = data[i] == '=' ? 0 & i++ : d_table[data[i++]];
		sextet[3] = data[i] == '=' ? 0 & i++ : d_table[data[i++]];
		triple = (sextet[0] << 18) + (sextet[1] << 12) + (sextet[2] << 6) + sextet[3];
		if (j < *out_len)
			d_data[j++] = (triple >> 16) & 0xFF;
		if (j < *out_len)
			d_data[j++] = (triple >> 8) & 0xFF;
		if (j < *out_len)
			d_data[j++] = triple & 0xFF;
	}
	return (d_data);
}

void 	build_decoding_table()
{
	size_t	i;

	if (!(d_table = malloc(256)))
		return (NULL);
	i = 0;
	while (i < 64)
	{
		d_table[(unsigned char)e_table[i]] = i;
		i++;
	}
}

void	base64_cleanup()
{
	free(d_table);
}

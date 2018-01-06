/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl_des.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agundry <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/06 11:54:57 by agundry           #+#    #+#             */
/*   Updated: 2018/01/06 14:16:31 by agundry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SSL_H
# define FT_SSL_H
# include <stdlib.h>
# include <stdio.h>
# include <stddef.h>
# include <stdint.h>
# include <stdbool.h>

//define static tables here????



typedef	struct		s_des
{
	long long int	text;
	long long int	key;
	bool			e;
	bool			d;
	bool			a;
	bool			k; //like openssl -K
	int				i;//save filedescriptor
	int				o;//
}					t_des; //move to des?

typedef struct		s_base64
{
	char			*in;
	char			*out;
	bool			e;
	bool			d;
	int				i;//save filedes
	int				o;
}					t_base64;

typedef int	(*t_fc)(char **av);

typedef	struct		s_keys
{
	uint64_t		key;
	uint64_t		pc1:56;
	int				round;
	uint64_t		keys[16];//????
}					t_keys;

/*ft_ssl core functionality*/
int				get_operation(char *in);
char			*ssl_get_file(int fd);
int				ssl_base64(char **av);
int				ssl_des();
int				ssl_dispatch(char **av);
void			ssl_getflags_base64(char **av);
void			ssl_getflags_des(char **av, t_des *des);
int				main(int ac, char **av);/////

/*base64 conversion*/
char			*bas64_encode(const unsigned char *data, size_t in_len, size_t *out_len);
unsigned char	*base64_decode(const char *data, size_t in_len, size_t *out_len);
void			build_decoding_table();
void			base64_cleanup();
int				ssl_base64(char **av); //////////moved?

/*core des functionality*/
int				init_des(t_des *des);
int 			key_rotate(t_keys *keys, int i);
uint64_t		permute(uint64_t in, int size, int box[]);
t_keys			*generate_keys(t_des *des);
uint32_t		s_boxes(uint64_t in);
uint32_t		feistel(uint32_t right, t_keys *keys);
uint64_t		transform(t_des *des, t_keys *keys);
int				des(t_des *des);
#endif

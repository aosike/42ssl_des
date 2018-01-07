/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agundry <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/06 11:56:01 by agundry           #+#    #+#             */
/*   Updated: 2018/01/06 17:20:03 by agundry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl_des.h"

int	get_operation(char *in)
{
	if (ft_strcmp(in, "base64"))
		return (0);
	if (ft_strcmp(in, "des"))
		return (1);
	if (ft_strcmp(in, "des-cbc"))
		return (2);
	if (ft_strcmp(in, "des-ebc"))
		return (3);
	return (-1);
}

const unsigned char	*ssl_get_file(int fd)
{
	char	*ret;

	get_file(fd, &ret);
	return((const unsigned char*)ret);
}

int	ssl_base64(char **av)
{
	t_base64	base64;
	size_t		i_len;
	size_t		o_len;

	ssl_getflags_base64(av, &base64); //make a getflags for every group of fxn that uses same flags
	base64.in = base64.i ? ssl_get_file(base64.i) : ssl_get_file(0); //build wrapper here, then add to LIB
	i_len = ft_strlen((const char *)base64.in);
	o_len = i_len; //set to zero?
	base64.out = base64.d && !base64.e ? base64_decode(base64.in, i_len, &o_len)
		: base64_encode(base64.in, i_len, &o_len); //might need to make encrypt more explicit
	if (base64.o)
		write(base64.o, base64.out, o_len); //fetch filename
	else
		ft_printf("%i", base64.out);
	return (1); /////////// janky
}

int	ssl_des(char **av)
{
	t_des	des;
	size_t	i_len;
	size_t	o_len;
	//flags
	//input
	//base64
	//key
	//enc / dec
	//output
	ssl_getflags_des(av, &des);
	des.in = des.i ? ssl_get_file(des.i): ssl_get_file(0);///read until EOF on stdin on false, needs fxn
	//fetch password?!
	!des.a ? : base64_decode(des.in); ///
	if (des.k)
		;
	des.d ? des_decrypt() : des_encrypt();//make wrapper fxns or just reverse order of keygen? will depend on CBC EBC
	!des.a ? : base64_encode(des.in); ////////
	des.o ? write(des.o, des.out, o_len) : write(0, des.out, o_len);//to stdout
}

int	ssl_dispatch(char **av)
{
	static t_fp	fp[2]; //
	size_t		i;

	fp[0] = &ssl_base64; //
	fp[1] = &ssl_des;
	fp[2] = &ssl_des_ebc;
	fp[3] = &ssl_des_cbc;

	if (i = get_operation(av[1])) //sophisticate
		return (fp[i](av));
	return (NULL);//error/usage
}

void	ssl_getflags_base64(char **av, ft_base64 *base64)
{
	char	flag;
	while (**av)
	{
		if (*(*av++) == '-' && *(*av + 1) == ' ') //if more than one char, its not a flag? space is not arbitrary
		{
			flag = **av;
			base64.e = flag == 'e' ? 1 : 0;
			base64.d = flag == 'd' ? 1 : 0;
			if (flag == 'i')
				base64.i = fopen(**av, r);//
			if (flag == 'o')
				base64.o = fopen(**av, r);//
		}
	}
}

void	ssl_getflags_des(char **av, ft_des *des)
{
	char	flag;
	while (**av)
	{
		if (*(*av++) == '-' && *(*av + 1) == ' ')
		{
			flag = **av;
			des.e = flag == 'e' ? 1 : 0;
			des.d = flag == 'd' ? 1 : 0;
			des.a = flag == 'a' ? 1 : 0;
			des.k = flag == 'k' ? 1 : 0;
			if (flag == 'i')
				des.i = fopen(**av, r);//
			if (flag == 'o')
				des.o = fopen(**av, r);//
		}
	}
}

int	main(int ac, char **av) //
{
	t_des	*des; //declare in des, allow all fxns to declare their own strux, pass in av index past flags
	//determine operation
	//parse flags / input
	//operate
	//return

	if (ac < 2 || ac > /**/)
		return (NULL);
	if (!ssl_dispatch(av))
		return (NULL); //error / usage
	return (/*YUP*/); 
}

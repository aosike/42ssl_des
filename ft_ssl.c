typedef	struct		s_des
{
	long long int	text;
	long long int	key;
	bool			flag;
}					t_des; //move to des?

typedef struct		s_ssl
{
}					t_ssl; //load this strux with parsed flags, operation, keys, etc.
//expand this as necessary for future fxns

typedef int	(*t_fc)(t_ssl *ssl);

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

int	ssl_dispatch(char **av)
{
	static t_fc	fp[2]; //
	size_t		i;

	fp[0] = &ssl_base64; //
	fp[1] = &ssl_des;
	fp[2] = &ssl_des_ebc;
	fp[3] = &ssl_des_cbc;

	if (i = get_operation(av[1]))
		return (fp[i](av));
	return (NULL);
}

int	main(int ac, char **av)
{
	t_des	*des; //declare in des, allow all fxns to declare their own strux, pass in av index past flags
	t_ssl	*ssl;
	//determine operation
	//parse flags / input
	//operate
	//return

	if (ac < 2 || ac > /**/)
		return (NULL);
	if (!ssl_dispatch(av))
		return (NULL); //error / usage
}

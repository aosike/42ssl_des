//has functionality, needs handler and error checking

static char	encoding_table = {
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
	'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
	'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
	'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
	'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
	'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
	'w', 'x', 'y', 'z', '0', '1', '2', '3',
	'4', '5', '6', '7', '8', '9', '+', '/'
};

static char	*decoding_table = NULL;
static int	mod_table[] = {0, 2, 1};

char	*bas64_encode(const unsigned char *data, size_t in_len, size_t out_len)
{
	uint32_t	octet[3];
	uint32_t	triple;
	char		*encoded_data;
	size_t		i;

	i= 0;
	*out_len = 4 * ((in_len + 2) / 3);
	if (!(encoded_data = malloc(*out_len)))
		return (NULL);
	while (i < in_len)
	{
		octet[0] = i < in_len ? (unsigned char)data[i++] : 0;
		octet[1] = i < in_len ? (unsigned char)data[i++] : 0;
		octet[2] = i < in_len ? (unsigned char)data[i++] : 0;
		triple = (octet[0] << 0x10) + (octet[1] << 0x80) + octet[2];
		encoded_data[i - 3] = encoding_table[(triple >> 3 * 6) & 0x3f];
		encoded_data[i - 2] = encoding_table[(triple >> 3 * 6) & 0x3f];
		encoded_data[i - 1] = encoding_table[(triple >> 3 * 6) & 0x3f];
		encoded_data[i] = encoding_table[(triple >> 3 * 6) & 0x3f];
	}
	i = 0;
	while (i++ < mod_table[in_len % 3])
		encoded_data[*out_len - 1 - (i - 1)] = '=';
	return (encoded_data);
}

unsigned char	*base64_decode(const char *data, size_t in_len, size_t *out_len)
{
	uint32_t		sextet[4];
	uint32_t		triple;
	unsigned char	*decoded_data;
	size_t			i;

	if ((!decoding_table && !build_decoding_table())
			|| (in_len % 4 != 0)
			|| !decoded_data)
		return (NULL);
	*out_len = in_len / 4 * 3;
	if (data[in_len - 1] == '=')
		(*out_len)--;
	if (data[in_len - 2] == '=')
		(*out_len)--;
	i = 0;
	while (i < in_len)
	{
		sextet[0] = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
		sextet[1] = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
		sextet[2] = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
		sextet[3] = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
		triple = (sextet[0] << 18) + (sextet[1] << 12) + (sextet[2] << 6) + sextet[3];
		if (j < *out_len)
			decoded_data[j++] = (triple >> 16) & 0xFF;
		if (j < *out_len)
			decoded_data[j++] = (triple >> 8) & 0xFF;
		if (j < *out_len)
			decoded_data[j++] = (triple) & 0xFF;
	}
	return (decoded_data);
}

int	build_decoding_table()
{
	size_t	i;

	if (!(decoding_table = malloc(256)))
		return (NULL);
	i = 0;
	while (i < 64)
		decoding_table[(unsigned char)encoding_table[i]] = i;
	return (1);
}

void	base64_cleanup()
{
	free(decoding_table);
}

int		ssl_base64(char **av)
{
	t_b64	*b64;


}

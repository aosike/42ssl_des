/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_des.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agundry <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/06 11:55:42 by agundry           #+#    #+#             */
/*   Updated: 2018/01/06 11:55:54 by agundry          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//get key
//determine if decrypt or encrypt (we will assume encrypt for this pseudo)
//get plaintext
//generate 16 48 bit keys from 56 of a 64 bit string
//	send key to PC-1, reduce 64b to 56b, save 8b for parity
//	separate pc1 into two 28b
//	rounds (16) (keys generated in reverse order for decryption)
//		rotate bits(r-box) in each half according to shift table
//		send both halves to PC-2 to make new 48b subkey
//intial permutation (just another straight p-box, no significance)
//rounds (16)
//	grab right 32 bits
//	expand to right = 48 bits
//	XOR with the first unused 48 bit key
//	send the resulting right to s-boxes
//		8 s-boxes input each 6 bit segment and output 4 bit segments
//			multply outer 2 bits by inner 4 bits to get index of s-box table
//		combined into 32 bit string arranged according to straight p-box
//	tmp = right
//	right = XOR right with left
//	left  = tmp
//final permutation
//
//
//s-boxes, r-boxes, and p-boxes just static stack int arrays?????????

#	define	SUBKEY_SEL(x, y) (y ? 16 - x : x)

static int	rbox[] = 
{
	1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1
}

static int pc1box[] =
{
	57, 49, 41, 33, 25, 17, 9, 63, 55, 47, 39, 31, 23, 15,
	1, 58, 50, 42, 34, 26, 18, 7, 62, 54, 46, 38, 30, 22,
	10, 2, 59, 51, 43, 35, 27, 14, 6, 61, 53, 45, 37, 29,
	19, 11, 3, 60, 52, 44, 36, 21, 13, 5, 28, 20, 12, 4
}

static int	pc2box[] = 
{
	14, 17, 11, 24, 1, 5,
	3, 28, 15, 6, 21, 10,
	23, 19, 12, 4, 26, 8,
	16, 7, 27, 20, 13, 2,
	41, 52, 31, 37, 47, 55,
	30, 40, 51, 45, 33, 48,
	44, 49, 39, 56, 34, 53,
	46, 42, 50, 36, 29, 32
}

static uint32_t sbox[][] = 
{
	{14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7,
	0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8,
	4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0,
	15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13},

	{15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10,
	3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5,
	0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15,
	13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9},

	{10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8,
	13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1,
	13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7,
	1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12},

	{7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15,
	13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9,
	10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4,
	3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14},

	{2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9,
	14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6,
	4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14,
	11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3},

	{12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11,
	10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8,
	9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6,
	4, 3, 2, 12, 9, 5, 51, 10, 11, 14, 1, 7, 6, 0, 8, 13},

	{4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1,
	13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6,
	1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2,
	6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12},

	{13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7,
	1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2,
	7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8,
	2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}
}

static int	ebox[] =
{
	32, 1, 2, 3, 4, 5,
	4, 5, 6, 7, 8, 9,
	8, 9, 10, 11, 12, 13,
	12, 13, 14, 15, 16, 17,
	16, 17, 18, 19, 20, 21,
	20, 21, 22, 23, 24, 25,
	24, 25, 26, 27, 28, 29,
	28, 29, 30, 31, 32, 1
}

static int	pbox[] =
{
	16, 7, 20, 21, 29, 12, 28, 17,
	1, 15, 23, 26, 5, 18, 31, 10,
	2, 8, 24, 14, 32, 27, 3, 9,
	19, 13, 30, 6, 22, 11, 4, 25
}

static int	ipbox[] =
{
	58, 50, 42, 34, 26, 18, 10, 2,
	60, 52, 44, 36, 28, 20, 12, 4,
	62, 54, 46, 38, 30, 22, 14, 6,
	64, 56, 48, 40, 32, 24, 16, 8,
	57, 49, 41, 33, 25, 17, 9, 1,
	59, 51, 43, 35, 27, 19, 11, 3,
	61, 53, 45, 37, 29, 21, 13, 5,
	63, 55, 47, 39, 31, 23, 15, 7
}

static int	fpbox[] =
{
	40, 8, 48, 16, 56, 24, 64, 32,
	39, 7, 47, 15, 55, 23, 63, 31,
	38, 6, 46, 14, 54, 22, 62, 30,
	37, 5, 45, 13, 53, 21, 61, 29,
	36, 4, 44, 12, 52, 20, 60, 28,
	35, 3, 43, 11, 51, 19, 59, 27,
	34, 2, 42, 10, 50, 18, 58, 26,
	33, 1, 41, 9, 49, 17, 57, 25
}

typedef	struct		s_des //moved to ssl??
{
	uint64_t		text;
	uint64_t		key;
	bool			flag; //will reference flag to establish order (decrypt = 0/encrypt = 1)
}					t_des;

typedef	struct		s_keys
{
	uint64_t		key;
	uint64_t		pc1:56;
	int				round;
	uint64_t		keys[16];//????
}					t_keys;

int	init_des(t_des *des) //do this later, perhaps defer this to SSL, send the prepped data to this module
{
	get_key;
	get_flag;
	get_text;
}


int key_rotate(t_keys *keys, int i)
{
	uint64_t	left:56;
	uint64_t	right:56;
	uint64_t	out:56;

	right = keys->pc1;
	left = keys->pc1 >> 28;
	left = left << 28;
	out += right << rbox[i];//
	out += right >> 28 - rbox[i];
	out += left << rbox[i];
	out += left >> 28 - rbox[i];
	keys->pc1 = out;
}

uint64_t	permute(uint64_t in, int size, int box[])
{
	int 		i = 0;
	uint64_t	out : size = 0;

	while (tab = box[i])
	{
		if (in & (1 << (tab - 1)))
			out += (1 << (i - 1));
		i++;
	}
	return (out);
}

t_keys	*generate_keys(t_des *des)
{
	t_keys		keys;

	keys->round = 1;
	keys->pc1 = key_permute(keys->key, 56, /*pc1box*/);
	while (keys->round <= 16)
	{
		key_rotate(&keys, round);
		keys->keys[keys->round] = key_permute(keys->pc1, 48, /*pc2box*/);
	}
}

uint32_t	s_boxes(uint64_t in : 48)
{
	int 		i = 0;
	int			tmp:6;
	int			y:2 = 0;
	int			x:4;
	uint32_t	out = 0;

	while (i++ < 8)
	{
		tmp = in >> (48 - (6 * i));
		x = tmp >> 1;
		y += tmp & (1 << 5) ? 2 : 0;
		y += tmp & 1 ? 1 : 0;
		out += sbox[i - 1][((x + 1) * (y + 1)) - 1] << (32 - (4 * i)); //make sbox be a uint32_t array so you can shift 
	}
	return (out);
}

uint32_t	feistel(uint32_t right, t_keys *keys)
{
	uint64_t	subkey:48;
	uint64_t	tmp:48;
	uint32_t	out;

	subkey = keys->keys[SUBKEY_SEL(keys->round, des->flag)] //macro will prolly be replaced by fxn and key gen will change and be per round to accomodate both modes of openSSL, "get_next_key" type fxn
	tmp = permute(right, 48, /*ebox*/);
	tmp = tmp ^ subkey;
	out = s_boxes(tmp);
	out = permute(out, 32, /*pbox*/);
}

uint64_t	transform(t_des *des, t_keys *keys)
{
	uint64_t	out;
	int			round = 0;
	uint32_t	left;
	uint32_t	right;
	uint32_t	tmp;

	out = permute(des->text, 64, /*ipbox*/);
	left = out >> 32;
	right = out;
	while ((keys->round = round) < 16)
	{
		tmp = right;
		right = feistel(right, &keys);
		right = left ^ right;
		left = tmp;
		round++;
	}
	out = left;
	out = (out << 32) | right;
	return (out = permute(out, 64, /*fpbox*/));
}

int	des(t_des *des)
{
	t_keys		*keys;
	uint64_t	ret:48;

	keys = generate_keys(&des);
	return (ret = transform(&des, keys));
}

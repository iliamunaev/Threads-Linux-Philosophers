/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imunaev- <imunaev-@studen.hive.fi>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 09:46:49 by imunaev-          #+#    #+#             */
/*   Updated: 2025/03/17 12:02:32 by imunaev-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/**
 * @brief Copies up to `n` characters from the source string
 * to the destination string.
 *
 * This function copies at most `n` characters from the source string `src` to
 * the destination buffer `dest`. If `src` is shorter than `n` characters,
 * `dest` is null-terminated. Unlike the standard `strncpy`, this implementation
 * explicitly null-terminates the destination string.
 *
 * @param dest The destination buffer where the string is copied.
 * @param src The source string to copy from.
 * @param n The maximum number of characters to copy.
 */
void	ft_strncpy(char *dest, const char *src, size_t n)
{
	size_t	i;

	i = 0;
	while (src && src[i] && i < n)
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
}

/**
 * @brief Calculates the length of a string.
 *
 * This function determines the number of characters in the null-terminated
 * string `s`, excluding the terminating null byte (`\0`).
 *
 * @param s The string whose length is to be calculated.
 * @return size_t The length of the string, excluding the null terminator.
 */
static size_t	ft_strlen(const char *s)
{
	size_t	len;

	len = 0;
	while (s[len])
		len++;
	return (len);
}

void	print_error(char *msg)
{
	size_t	len_msg;
	
	len_msg = ft_strlen(msg);
	(void)write(STDOUT_FILENO, msg, len_msg);
}

/**
 * @brief Checks if a character is a whitespace character.
 *
 * This function determines whether the given character is a
 * whitespace character, which includes:
 * - Space (' ')
 * - Newline ('\n')
 * - Form feed ('\f')
 * - Carriage return ('\r')
 * - Horizontal tab ('\t')
 * - Vertical tab ('\v')
 *
 * @param c The character to check.
 * @return int 1 if the character is a whitespace character, 0 otherwise.
 */

static int	ft_isspace(char c)
{
	return (c == ' ' || c == '\n' || c == '\f' || c == '\r'
		|| c == '\t' || c == '\v');
}
/**
 * @brief Checks if a character is a numeric digit.
 *
 * This function tests whether the given character `c` is a numeric
 * digit (characters '0' through '9').
 *
 * @param c The character to check.
 * @return int Nonzero if the character is a numeric digit, 0 otherwise.
 */
static int	ft_isdigit(int c)
{
	return (c >= '0' && c <= '9');
}

/**
 * @brief Converts the initial portion of a string to an integer.
 *
 * This function skips leading whitespace, handles an optional '+' or '-' sign,
 * and accumulates numeric characters until encountering a non-digit or the end
 * of the string. The result is returned as an integer value.
 *
 * @param str The string to be converted.
 * @return int The converted integer, or 0 if no valid conversion could be
 * performed.
 */
int	ft_atoi(const char *str)
{
	int			sign;
	long long	num;

	num = 0;
	sign = 1;
	while (ft_isspace(*str))
		str++;
	if (*str == '-')
	{
		sign = -1;
		str++;
	}
	else if (*str == '+')
		str++;
	while (ft_isdigit(*str))
	{
		num = num * 10 + (*str - '0');
		str++;
	}
	return ((int)(num * sign));
}
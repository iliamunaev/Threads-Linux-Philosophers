/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imunaev- <imunaev-@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 09:46:49 by imunaev-          #+#    #+#             */
/*   Updated: 2025/03/18 21:14:53 by imunaev-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file utils.c
 * @brief Utility functions for string manipulation and numerical conversion.
 *
 * This file contains commonly used helper functions, including:
 * - String copying (`ft_strncpy`)
 * - String length calculation (`ft_strlen`)
 * - Character classification (`ft_isspace`, `ft_isdigit`)
 * - String-to-integer conversion (`ft_atoi`)
 */

#include "philo.h"

/**
 * @brief Copies up to `n` characters from the source string to the destination
 * buffer.
 *
 * This function copies at most `n` characters from the source string `src` into
 * the destination buffer `dest`. If `src` is shorter than `n` characters,
 * `dest`
 * is null-terminated. Unlike the standard `strncpy`, this function ensures
 * `dest` is always null-terminated.
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
 * @brief Computes the length of a string.
 *
 * This function returns the number of characters in the null-terminated
 * string `s`, excluding the null terminator.
 *
 * @param s The input string.
 * @return size_t The length of the string, excluding the null terminator.
 */
size_t	ft_strlen(const char *s)
{
	size_t	len;

	len = 0;
	while (s[len])
		len++;
	return (len);
}

/**
 * @brief Determines whether a character is a whitespace character.
 *
 * This function checks if `c` is a whitespace character. Whitespace characters
 * include:
 * - Space (' ')
 * - Newline ('\n')
 * - Form feed ('\f')
 * - Carriage return ('\r')
 * - Horizontal tab ('\t')
 * - Vertical tab ('\v')
 *
 * @param c The character to check.
 * @return int Returns 1 if the character is a whitespace character,
 * otherwise 0.
 */
static int	ft_isspace(char c)
{
	return (c == ' ' || c == '\n' || c == '\f' || c == '\r'
		|| c == '\t' || c == '\v');
}

/**
 * @brief Determines whether a character is a numeric digit.
 *
 * This function checks if `c` is a digit (characters '0' through '9').
 *
 * @param c The character to check.
 * @return int Returns nonzero if `c` is a numeric digit, otherwise 0.
 */
static int	ft_isdigit(int c)
{
	return (c >= '0' && c <= '9');
}

/**
 * @brief Converts a string to an integer (`atoi` implementation).
 *
 * This function:
 * - Skips leading whitespace.
 * - Processes an optional '+' or '-' sign.
 * - Converts numeric characters into an integer until a non-digit character
 * is encountered.
 * - Returns the result as an integer.
 *
 * Thread safety:
 * - This function operates on local variables only and does not modify
 * shared state.
 *
 * @param str The string to convert.
 * @return int The integer representation of the string, or 0 if no valid
 * conversion is possible.
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

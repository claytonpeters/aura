// Copyright (c) 2013 Clayton Peters
// This program is distributed under the terms of the GNU Lesser General Public
// License (LGPL). A copy of the license is included in the file COPYING.LESSER

// Includes:
#include "aura.h"
#include <stdio.h>

/// Returns the length of a UTF8-encoded string
/// @param s The string to determine the length of
/// @returns The length of the string in characters
LIBAURA_EXPORTED size_t utf8len(const char* s)
{
	size_t i = 0;
	while (*s)
	{
		if ((*s++ & 0xc0) != 0x80)
		{
			i++;
		}
	}

	return i;
}

/// Returns a pointer to the num'th character in a UTF8-encoded string
/// @param s The string to index in to
/// @param num The index of the character to return a pointer to
/// @returns A pointer to the character or NULL if past the end of the string
LIBAURA_EXPORTED char* utf8idx(const char* s, size_t num)
{
	size_t i = num + 1;
	while (*s)
	{
		if ((*s & 0xc0) != 0x80)
		{
			if (--i == 0) { return (char*)s; }
		}
		s++;
	}

	return NULL;
}

/// Copies a section of a UTF-8 encoded string to another. Note that as we will
/// end up copying a number of bytes in to dst, we may end up inside a multi-
/// byte character in dst, so we'll have to move the entire end of the string.
/// In this case, the length of dst (in bytes) will change.
/// @param dst The destination string to append to
/// @param src The source string to copy from
/// @param num The number of characters to copy from src to dst
/// @returns dst is returned or NULL if num > utf8len(src)
LIBAURA_EXPORTED char* utf8ncpy(char* dst, const char* src, size_t num)
{
	// Get the pointer to the character to end on (we don't copy this character)
	const char* end = utf8idx(src, num);
	if (end == NULL)
	{
		return NULL;
	}

	// Copy the bytes in
	memcpy(dst, src, end - src);

	// See if we're in the middle of a multi-byte character
	if ((dst[end - src] & 0xc0) == 0x80)
	{
		fprintf(stderr, "Landed inside multibyte character.\n");
		for (size_t i = 1; i < 6; i++)
		{
			if ((dst[end - src + i] & 0xc0) != 0x80)
			{
				fprintf(stderr, "Copyback, skipping %lu bytes\n", i);
				// We've reached the start of a new character,
				// move remainder of string back appropriately,
				// discarding the broken character
				strcpy(&dst[end - src], &dst[end - src + i]);
				break;
			}
		}
	}

	return dst;
}

/// Appends the first num characters of src on to dst
/// @param dst The destination string to append to
/// @param src The source string to copy from
/// @param num The number of characters to copy from src to dst
/// @returns dst is returned or NULL if num > utf8len(src)
LIBAURA_EXPORTED char* utf8ncat(char* dst, const char* src, size_t num)
{
	// Get the pointer to the character to end on (we don't copy this character)
	const char* end = utf8idx(src, num);
	if (end == NULL)
	{
		return NULL;
	}

	// Append the bytes to the string with memcpy, adding a NUL-terminator
	size_t dstBytes = strlen(dst);
	memcpy(&dst[dstBytes], src, end - src);
	dst[dstBytes + (end - src)] = 0;

	return dst;
}

/// Compares a number of characters in two UTF-8 strings
/// @param str1 The first UTF-8 encoded string to compare
/// @param str2 The UTF8-encoded string to compare str1 to
/// @param num The maximum number of characters to compare
/// @returns See strncmp(3)
LIBAURA_EXPORTED int utf8ncmp(const char* str1, const char* str2, size_t num)
{
	size_t i = num;
	while (*str1 && &str2)
	{
		if (*str1 == *str2)
		{
			// We only need to test one of the strings as we have
			// already checked that the current char is identical

			// If we're at the start of a multi-byte char, then
			// increment the counter, otherwise don't
			if ((*str1 & 0xc0) != 0x80)
			{
				// If we've matched the correct number of 
				// characters then the strings are identical	
				if (--i == 0)
				{
					return 0;
				}
			}

			str1++;
			str2++;
		}
		else
		{
			return (int)(*str1 - *str2);
		}
	}

	return 0;
}


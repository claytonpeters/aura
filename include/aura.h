// Copyright (c) 2013 Clayton Peters
// This program is distributed under the terms of the GNU Lesser General Public
// License (LGPL). A copy of the license is included in the file COPYING.LESSER

#if !defined(AURA_H_INCLUDED)
#define AURA_H_INCLUDED

// Includes:
#include <string.h>

// Exports definition:
#if defined(__GNUC__)
#	define LIBAURA_EXPORTED extern "C"
#elif defined(_MSC_VER)
#	if defined(LIBAURA_DLL_BUILD)
#		define LIBAURA_EXPORTED extern "C" __declspec(dllexport)
#	else
#		define LIBAURA_EXPORTED extern "C" __declspec(dllimport)
#	endif
#endif

/// Gets the version of libaura
LIBAURA_EXPORTED int getAuraVersion();

// UTF-8 versions of libc string functions /////////////////////////////////////

// Some of them are the same:
#define utf8cat strcat
#define utf8cmp strcmp
#define utf8cpy strcpy

/// Returns the length of a UTF8-encoded string
/// @param s The string to determine the length of
/// @returns The length of the string in characters
LIBAURA_EXPORTED size_t utf8len(const char* s);

/// Returns a pointer to the num'th character in a UTF8-encoded string
/// @param s The string to index in to
/// @param num The index of the character to return a pointer to
/// @returns A pointer to the character or NULL if past the end of the string
LIBAURA_EXPORTED char* utf8idx(const char* s, size_t num);

/// Copies a section of a UTF-8 encoded string to another. Note that as we will
/// end up copying a number of bytes in to dst, we may end up inside a multi-
/// byte character in dst, so we'll have to move the entire end of the string.
/// In this case, the length of dst (in bytes) will change.
/// @param dst The destination string to append to
/// @param src The source string to copy from
/// @param num The number of characters to copy from src to dst
/// @returns dst is returned or NULL if num > utf8len(src)
LIBAURA_EXPORTED char* utf8ncpy(char* dst, const char* src, size_t num);

/// Appends the first num characters of src on to dst
/// @param dst The destination string to append to
/// @param src The source string to copy from
/// @param num The number of characters to copy from src to dst
/// @returns dst is returned or NULL if num > utf8len(src)
LIBAURA_EXPORTED char* utf8ncat(char* dst, const char* src, size_t num);

/// Compares a number of characters in two UTF-8 strings
/// @param str1 The first UTF-8 encoded string to compare
/// @param str2 The UTF8-encoded string to compare str1 to
/// @param num The maximum number of characters to compare
/// @returns See strncmp(3)
LIBAURA_EXPORTED int utf8ncmp(const char* str1, const char* str2, size_t num);

/// An enumeration defining the valid types of plugins to Aura
typedef enum aura_plugin_type_t
{
	/// Type for static elements (e.g. background, logo images, etc.)
	AURA_PLUGIN_TYPE_ELEMENT = 0,

	/// Type for data sources (e.g. Twitter, Facebook, etc.)
	AURA_PLUGIN_TYPE_SOURCE,

	/// Type for item transition (e.g. Fade-out/in)
	AURA_PLUGIN_TYPE_TRANSITION,

	/// Type for item layouts (e.g. single tweet, scrolling tweets, ticker, etc.)
	AURA_PLUGIN_TYPE_LAYOUT,
} aura_plugin_type_t;

/// Structure defining the description of a plugin to Aura. When Aura starts and
/// loads the plugins, one of these structures will be requested from each to
/// determine what it does
typedef struct aura_plugin_desc_t
{
	/// The name of the plugin (e.g. Twitter, Static Image, Fade in)
	const char* name;

	/// The name of the autor
	const char* author;

	/// The description of the plugin
	const char* description;

	/// The version of the plugin
	const char* version;

	/// The plugin type, see aura_plugin_type_t
	aura_plugin_type_t type;
} aura_plugin_desc_t;

/// Function pointer to plugin get_description() function
/// @returns A pointer to an aura_plugin_desc_t structure that describes the
/// plugin.
typedef aura_plugin_desc_t* (*aura_plugin_func_get_description_t)(void);

/// Function pointer to plugin free_description() function
/// @param aura_plugin_desc_t* The pointer to the structure to free.
typedef void (*aura_plugin_func_free_description_t)(aura_plugin_desc_t*);

/// Function pointer to plugin unload() function
typedef void (*aura_plugin_func_unload_t)(void);

/// Structure defining a plugin
typedef struct aura_plugin_t
{
	/// Pointer to the plugin description structure
	const aura_plugin_desc_t* description;

	/// Function-pointer to unload plugin
	aura_plugin_func_unload_t unload;
} aura_plugin_t;

/// Function pointer to plugin load() function
/// @returns A pointer to a filled aura_plugin_t structure
typedef aura_plugin_t* (*aura_plugin_func_load_t)(void);
 
#endif // !defined(AURA_H_INCLUDED)


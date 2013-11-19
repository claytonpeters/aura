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

/// Gets the version of libaura. The version number is of the following format:
///   (major * 1000000) + (minor * 1000) + (build)
/// @returns The version number
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

	/// Type for item layouts (e.g. single tweet, scrolling list, ticker, etc.)
	AURA_PLUGIN_TYPE_LAYOUT,
} aura_plugin_type_t;

/// An enumeration defining the valid type for plugin settings
typedef enum aura_vartype_t
{
	/// Integer
	AURA_VARTYPE_INT = 0,

	/// Boolean
	AURA_VARTYPE_BOOLEAN,

	/// Floating point
	AURA_VARTYPE_FLOAT,

	/// Text
	AURA_VARTYPE_TEXT,

	/// Filename
	AURA_VARTYPE_FILENAME,

	/// Color
	AURA_VARTYPE_COLOR,
} aura_vartype_t;

/// Structure defining the description of a plugin to Aura. When Aura starts and
/// loads the plugins, one of these structures will be requested from each to
/// determine what it does
typedef struct aura_plugin_desc_t
{
	/// The name of the plugin (e.g. Twitter, Static Image, Fade in)
	const char* name;

	/// The name of the author
	const char* author;

	/// The description of the plugin
	const char* description;

	/// The version of the plugin
	const char* version;

	/// The plugin type, see aura_plugin_type_t
	aura_plugin_type_t type;
} aura_plugin_desc_t;

/// Structure defining a setting for a plugin
typedef struct aura_plugin_setting_t
{
	/// The name of the setting
	const char* name;

	/// The description of the settings
	const char* description;

	/// The setting type
	aura_vartype_t type;

	/// Whether the setting is currently disabled (possibly dependant on another
	/// settings value, for example)
	bool disabled;
} aura_plugin_setting_t;

/// Structure defining an integer setting
typedef struct aura_plugin_setting_int_t
{
	/// The plugin setting 'superclass' for this setting
	aura_plugin_setting_t super;

	/// The minimum value for the setting
	long long minimum;

	/// The maximum value for the setting
	long long maximum;

	/// The value of the setting
	long long value;
} aura_plugin_setting_int_t;

/// Structure defining a floating-point setting
typedef struct aura_plugin_setting_float_t
{
	/// The plugin setting 'superclass' for this setting
	aura_plugin_setting_t super;

	/// The minimum value for the setting
	double minimum;

	/// The maximum value for the setting
	double maximum;

	/// The value of the setting
	double value;
} aura_plugin_setting_float_t;

/// Structure defining a boolean setting
typedef struct aura_plugin_setting_bool_t
{
	/// The plugin setting 'superclass' for this setting
	aura_plugin_setting_t super;

	/// The value of the setting
	bool value;
} aura_plugin_setting_bool_t;

/// Structure defining a text setting
typedef struct aura_plugin_setting_string_t
{
	/// The plugin setting 'superclass' for this setting
	aura_plugin_setting_t super;

	/// Specifies whether the text item is a password
	bool password;

	/// The value of the setting, UTF-8 encoded
	char* value;
} aura_plugin_setting_string_t;

/// Structure defining a filename setting
typedef struct aura_plugin_setting_file_t
{
	/// The plugin setting 'superclass' for this setting
	aura_plugin_setting_t super;

	/// The value of the setting, UTF-8 encoded
	char* value;
} aura_plugin_setting_file_t;

/// Structure defining a color setting
typedef struct aura_plugin_setting_color_t
{
	/// The plugin setting 'superclass' for this setting
	aura_plugin_setting_t super;

	/// Determines if the alpha component is used
	bool hasAlpha;

	/// The value of the setting: red component
	float valueR;
	/// The value of the setting: gren component
	float valueG;
	/// The value of the setting: blue component
	float valueB;
	/// The value of the setting: alpha component (if used)
	float valueA;
} aura_plugin_setting_color_t;

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

	/// List of pointers to settings, last in list is NULL 
	aura_plugin_setting_t** settings;

	/// Function-pointer: Unload plugin
	aura_plugin_func_unload_t unload;
} aura_plugin_t;

/// Structure defining an 'element' plugin. An element is any low-level item 
/// that could appear on the display, e.g. Colour, Gradient, Image, Text, etc.
typedef struct aura_element_plugin_t
{
	/// The plugin 'superclass' for this plugin
	aura_plugin_t super;
} aura_element_plugin_t;

/// Function pointer to plugin load() function
/// @returns A pointer to a filled aura_plugin_t structure
typedef aura_plugin_t* (*aura_plugin_func_load_t)(void);

/// Allocates a new setting of the given type. The function returns a pointer of
/// type aura_plugin_setting_t, but this should be casted to the appropriate 
/// pointer for whatever class of setting was requested, for example,
/// aura_plugin_setting_int_t
/// @returns A pointer to the newly allocated setting.
LIBAURA_EXPORTED aura_plugin_setting_t* aura_allocate_setting(aura_vartype_t type);

#endif // !defined(AURA_H_INCLUDED)


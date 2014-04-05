// Copyright (c) 2014 Clayton Peters
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
LIBAURA_EXPORTED int aura_get_version();

/// Initialises libaura. This function is not thread safe as it calls other initialisation
/// functions (such as curl_global_init) that are also not thread safe. It should only
/// be called once.
LIBAURA_EXPORTED int aura_init();

/// Structure containing everything receive about a download
typedef struct aura_download_data_t
{
	/// The received data
	char* data;

	/// The length of the data
	size_t dataLength;

	/// The HTTP response code
	long responseCode;

	/// The amount of time taken, in seconds, to perform the download
	double totalTime;
} aura_download_data_t;

/// Helper function to download a file using CURL and OpenSSL
LIBAURA_EXPORTED aura_download_data_t* aura_download_sync(const char* url);

/// Frees all the information associated with a download
LIBAURA_EXPORTED void aura_free_download_data(aura_download_data_t* downloadData);

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

/// An enumeration defining the valid type for plugin properties
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
	aura_plugin_type_t pluginType;

	/// What object types the plugin provides. This is an array of strings
	/// with the last pointer set to NULL
	const char** objectTypes;
} aura_plugin_desc_t;

/// Structure defining a property for a plugin
typedef struct aura_property_t
{
	/// The name of the property
	const char* name;

	/// The description of the properties
	const char* description;

	/// The property type
	aura_vartype_t type;

	/// Whether the property is currently disabled (possibly dependant on another
	/// properties value, for example)
	bool disabled;
} aura_property_t;

/// Structure defining an integer property
typedef struct aura_property_int_t
{
	/// The plugin property 'superclass' for this property
	aura_property_t super;

	/// The minimum value for the property
	long long minimum;

	/// The maximum value for the property
	long long maximum;

	/// The value of the property
	long long value;
} aura_property_int_t;

/// Structure defining a floating-point property
typedef struct aura_property_float_t
{
	/// The plugin property 'superclass' for this property
	aura_property_t super;

	/// The minimum value for the property
	double minimum;

	/// The maximum value for the property
	double maximum;

	/// The value of the property
	double value;
} aura_property_float_t;

/// Structure defining a boolean property
typedef struct aura_property_bool_t
{
	/// The plugin property 'superclass' for this property
	aura_property_t super;

	/// The value of the property
	bool value;
} aura_property_bool_t;

/// Structure defining a text property
typedef struct aura_property_string_t
{
	/// The plugin property 'superclass' for this property
	aura_property_t super;

	/// Specifies whether the text item is a password
	bool password;

	/// The value of the property, UTF-8 encoded
	char* value;
} aura_property_string_t;

/// Structure defining a filename property
typedef struct aura_property_file_t
{
	/// The plugin property 'superclass' for this property
	aura_property_t super;

	/// The value of the property, UTF-8 encoded
	char* value;
} aura_property_file_t;

/// Structure defining a color property
typedef struct aura_property_color_t
{
	/// The plugin property 'superclass' for this property
	aura_property_t super;

	/// Determines if the alpha component is used
	bool hasAlpha;

	/// The value of the property: red component
	float valueR;
	/// The value of the property: gren component
	float valueG;
	/// The value of the property: blue component
	float valueB;
	/// The value of the property: alpha component (if used)
	float valueA;
} aura_property_color_t;

/// Aura properties list (internally this is a pointer to a map<string, aura_property_t*>
typedef void *aura_properties_t;

/// Function pointer to plugin get_description() function
/// @returns A pointer to an aura_plugin_desc_t structure that describes the
/// plugin.
typedef aura_plugin_desc_t* (*aura_plugin_func_get_description_t)(void);

/// Function pointer to plugin free_description() function
/// @param aura_plugin_desc_t* The pointer to the structure to free.
typedef void (*aura_plugin_func_free_description_t)(aura_plugin_desc_t*);

/// Function pointer to plugin unload() function
typedef void (*aura_plugin_func_unload_t)(void);

/// Structure defining an individual instance of an object instantiated by a plugin
typedef struct aura_object_instance_t
{
	/// The type of plugin this object instance came from
	aura_plugin_type_t pluginType;

	/// The type of the object, identified by string
	const char* objectType;

	///  Properties list
	aura_properties_t properties;
} aura_object_instance_t;

/// Function point to plugin propertyChanged function
typedef bool (*aura_plugin_func_object_property_changed_t)(aura_object_instance_t*, aura_property_t*);

/// Function pointer to the create() function for plugins to return an instance
/// of an object of the given type
typedef aura_object_instance_t* (*aura_plugin_func_create_object_t)(const char*);

/// Function point to plugin propertyChanged function
typedef bool (*aura_plugin_func_property_changed_t)(aura_property_t*);

/// Structure defining a plugin
typedef struct aura_plugin_t
{
	/// Pointer to the plugin description structure
	aura_plugin_func_get_description_t getDescription;

	/// Function-pointer: Unload plugin
	aura_plugin_func_unload_t unload;

	/// Function-pointer: Create object instance
	aura_plugin_func_create_object_t create;

	/// Function-pointer: Notification of property change. Aura Live will
	/// call this to notify the plugin that something has changed
	aura_plugin_func_property_changed_t propertyChanged;
} aura_plugin_t;

/// Structure defining an 'element' plugin. An element is any low-level item 
/// that could appear on the display, e.g. Colour, Gradient, Image, Text, etc.
typedef struct aura_element_plugin_t
{
	/// The plugin 'superclass' for this plugin
	aura_plugin_t super;
} aura_element_plugin_t;

/// Structure defining a 'source' plugin. A source is any kind of data source
/// that is used to provide an array of key-value pairs to build something that
/// appears on the display
typedef struct aura_source_plugin_t
{
	/// The plugin 'superclass' for this plugin
	aura_plugin_t super;
} aura_source_plugin_t;

/// Function pointer to plugin load() function
/// @returns A pointer to a filled aura_plugin_t structure
typedef aura_plugin_t* (*aura_plugin_func_load_t)(void);

/// Allocates a new property of the given type. The function returns a pointer of
/// type aura_property_t, but this should be casted to the appropriate 
/// pointer for whatever class of property was requested, for example,
/// aura_property_int_t
/// @returns A pointer to the newly allocated property.
LIBAURA_EXPORTED aura_property_t* aura_allocate_property(aura_vartype_t type);

/// Get the available object types for a given plugin type, e.g. giving
/// pluginType as AURA_PLUGIN_TYPE_ELEMENT might return 'colour', 'gradient',
/// 'image', 'text', etc. Returns an array of strings, with the last in the
/// list being NULL
LIBAURA_EXPORTED const char** aura_get_object_types(aura_plugin_type_t pluginType);

/// Create an instance of an object
/// @param pluginType The type of plugin of the type to create, e.g.
/// AURA_PLUGIN_TYPE_ELEMENT
/// @param name The string identifier of the object to create, e.g. 'image'
LIBAURA_EXPORTED aura_object_instance_t* aura_create_object(aura_plugin_type_t pluginType, const char* name);

/// Delete an instance of an object
LIBAURA_EXPORTED void aura_delete_object(aura_object_instance_t* object);

/// Creates a property list
LIBAURA_EXPORTED aura_properties_t aura_create_property_list();

/// Frees a property map list, deleting all the properties within it
LIBAURA_EXPORTED void aura_delete_property_list(aura_properties_t properties);

/// Takes an aura_properties_t property list and a string for the name of the property to get and
/// returns that property from the list
LIBAURA_EXPORTED aura_property_t* aura_get_property(aura_properties_t properties, const char* name);

/// Takes an aura_properties_t property list and deletes the given property from the list
LIBAURA_EXPORTED void aura_delete_property(aura_properties_t properties, const char* name);

/// Takes an aura_properties_t property list and adds the given property to the list
LIBAURA_EXPORTED void aura_add_property(aura_properties_t properties, aura_property_t* property);

#endif // !defined(AURA_H_INCLUDED)


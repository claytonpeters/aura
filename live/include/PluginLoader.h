// Copyright (c) 2014 Clayton Peters
// This program is distributed under the terms of the GNU Lesser General Public
// License (LGPL). A copy of the license is included in the file COPYING.LESSER

#ifndef PLUGINLOADER_H_INCLUDED
#define PLUGINLOADER_H_INCLUDED

// Includes:
#include <libaura/aura.h>
#include <string>
#include <map>
#include <vector>
#ifndef WIN32
#	include <dlfcn.h>
#endif

// Namespaces:
using namespace std;

typedef pair<aura_plugin_type_t, string> aura_object_class_t;

/// The PluginLoader class searches directories for shared libraries and loads
/// them
/// @author Clayton Peters
class PluginLoader
{
	public:
		/// Constructs a new PluginLoader object
		/// @param _rootDir The path to start searching for plugins from
		PluginLoader(const string& _rootDir);

		/// Destroys a PluginLoader object, unloading all loaded plugins
		~PluginLoader();

		/// Gets the plugin responsible creating for objects of the given class
		/// @param objectClass A pair compromising of a plugin type and object type to search for
		aura_plugin_t* getPluginFor(aura_object_class_t objectClass);

		/// Gets the plugin responsible creating for objects of the given class
		/// @param pluginType The type of the plugin, e.g. AURA_PLUGIN_TYPE_ELEMENT
		/// @param objectType The name of the object type, e.g. "text", "twitter"
		aura_plugin_t* getPluginFor(aura_plugin_type_t pluginType, string objectType);

		/// Get the names of the object types available for a plugin type
		/// @param pluginType The type of the plugin, e.g. AURA_PLUGIN_TYPE_ELEMENT
		const vector<string>& getObjectTypes(aura_plugin_type_t pluginType);
 
	private:
		/// Scan the given directory for plugins, recursing down as 
		/// necessary and adding them to the map
		void scanPluginDir(const string& path);

		/// The path to search for plugins in
		string rootDir;

		/// The map of found plugin filenames and their shared object handles
		map<string, void*> pluginHandles;

		/// The map of found plugin filenames and their initialised plugin structures
		map<string, aura_plugin_t*> plugins;

		/// The map of [plugin types, object types] to plugin structures
		map<aura_object_class_t, aura_plugin_t*> objectPlugins;

		/// The map of object types available for each plugin type
		map<aura_plugin_type_t, vector<string>> objectTypes;
};

#endif


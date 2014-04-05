// Copyright (c) 2014 Clayton Peters
// This program is distributed under the terms of the GNU Lesser General Public
// License (LGPL). A copy of the license is included in the file COPYING.LESSER

// Includes:
#ifdef WIN32
#else
#	include <sys/types.h>
#	include <dirent.h>
#endif
#include "PluginLoader.h"
#include "AuraException.h"
#include "Log.h"

/// Constructs a new PluginLoader object
/// @param _rootDir The path to start searching for plugins from
PluginLoader::PluginLoader(const string& _rootDir) :
	rootDir(_rootDir)
{
	// Scan the plugin root directory for plugins
	log(LOG_INFO, "PluginLoader::PluginLoader: Starting search for plugins");
	scanPluginDir(rootDir);
	log(LOG_INFO, "PluginLoader::PluginLoader: Plugin search complete");

	// Print out the plugins
	for (auto pluginPair : pluginHandles)
	{
		log(LOG_DEBUG, "PluginLoader::PluginLoader: Loading %s", pluginPair.first.c_str());
#ifdef WIN32
#	error Not implemented
#else
		// Open the library
		void* handle = dlopen(pluginPair.first.c_str(), RTLD_LAZY);
		if (handle == NULL)
		{
			log(LOG_ERROR, "PluginLoader::PluginLoader: Failed to open plugin '%s', error %s", pluginPair.first.c_str(), dlerror());
			continue;
		}

		// Clear any outstanding errors
		dlerror();

		// Get the entry point function for the plugin
		aura_plugin_func_load_t loadPlugin = (aura_plugin_func_load_t)dlsym(handle, "aura_plugin_load");
		if (loadPlugin == NULL)
		{
			log(LOG_ERROR, "PluginLoader::PluginLoader: Failed to find entry point aura_plugin_load, error %s", dlerror());
			dlclose(handle);
			continue;
		}

		// Call plugin load exported function
		aura_plugin_t* plugin = loadPlugin();
		if (plugin == NULL)
		{
			log(LOG_ERROR, "PluginLoader::PluginLoader: NULL returned from aura_plugin_load");
			dlclose(handle);
			continue;
		}

		// Attempt to get the description
		if (plugin->getDescription)
		{
			aura_plugin_desc_t* description = plugin->getDescription();
			if (description == NULL)
			{
				log(LOG_ERROR, "PluginLoader::PluginLoader: NULL returned from getDescription");
				dlclose(handle);
				continue;
			}

			log(LOG_DEBUG, "PluginLoader::PluginLoader: Successfully loaded plugin '%s' version '%s', by '%s'", description->name, description->version, description->author);
			if (description->objectTypes != NULL)
			{
				auto iter = &description->objectTypes[0];
				while (*iter != NULL)
				{
					log(LOG_INFO, "PluginLoader::PluginLoader: - PROVIDES %s", *iter);
					objectPlugins[aura_object_class_t(description->pluginType, *iter)] = plugin;
					objectTypes[description->pluginType].push_back(*iter);
					iter++;
				}
			}
		}
		else
		{
			log(LOG_ERROR, "PluginLoader::PluginLoader: No getDescription function specified");
			dlclose(handle);
			continue;
		}

		// Store the plugin handle for closing later
		pluginHandles[pluginPair.first] = handle;
		plugins[pluginPair.first] = plugin;
#endif
	}
}

/// Destroys a PluginLoader object, unloading all loaded plugins
PluginLoader::~PluginLoader()
{
	for (auto pluginPair : pluginHandles)
	{
#ifdef WIN32
#	error Not implemented
#else
		if (pluginPair.second != NULL)
		{
			// If the plugin has an unload function
			if (plugins[pluginPair.first]->unload)
			{
				log(LOG_DEBUG, "PluginLoader::~PluginLoader: Unloading plugin '%s'", plugins[pluginPair.first]->getDescription()->name);
				plugins[pluginPair.first]->unload();
			}

			log(LOG_DEBUG, "PluginLoader::~PluginLoader: Closing plugin '%s'", pluginPair.first.c_str());
			dlclose(pluginPair.second);
		}
#endif
	}
}

/// Gets the plugin responsible creating for objects of the given class
/// @param objectClass A pair compromising of a plugin type and object type to search for
aura_plugin_t* PluginLoader::getPluginFor(aura_object_class_t objectClass)
{
	return objectPlugins[objectClass];
}

/// Gets the plugin responsible creating for objects of the given class
/// @param pluginType The type of the plugin, e.g. AURA_PLUGIN_TYPE_ELEMENT
/// @param objectType The name of the object type, e.g. "text", "twitter"
aura_plugin_t* PluginLoader::getPluginFor(aura_plugin_type_t pluginType, string name)
{
	return getPluginFor(aura_object_class_t(pluginType, name));
}

/// Get the names of the object types available for a plugin type
/// @param pluginType The type of the plugin, e.g. AURA_PLUGIN_TYPE_ELEMENT
const vector<string>& PluginLoader::getObjectTypes(aura_plugin_type_t pluginType)
{
	return objectTypes[pluginType]; 
}

/// Scan the given directory for plugins, recursing down as necessary and adding
/// them to the map
void PluginLoader::scanPluginDir(const string& path)
{
	log(LOG_INFO, "PluginLoader::scanPluginDir: Scanning: %s", path.c_str());
#ifdef WIN32
#	error Not implemented.
#else
	// Open the directory
	DIR* dir = opendir(path.c_str());
	if (dir == NULL)
	{
		throw AuraException(AURA_ERR_BADPLUGINDIR, "Failed to list contents of plugins directory");
	}
	
	// Iterate over entries in the directory
	struct dirent* ent;
	while (ent = readdir(dir))
	{
		string entName = path + "/" + string(ent->d_name);

		// If we have a directory...
		if (ent->d_type == DT_DIR)
		{
			// Ignore the . and .. special directories
			if (!(ent->d_name[0] == '.' && (ent->d_name[1] == '\0' || (ent->d_name[1] == '.' && ent->d_name[2] == '\0'))))
			{
				// Recurse down as necessary
				scanPluginDir(entName);
			}
		}
		// If we have a regular files...
		else if (ent->d_type == DT_REG)
		{
			size_t nameLen = strlen(ent->d_name);

			// If the filename ends in ".so"
			if (nameLen > 3 && ent->d_name[nameLen - 3] == '.' && ent->d_name[nameLen - 2] == 's' && ent->d_name[nameLen - 1] == 'o')
			{
				log(LOG_DEBUG, "PluginLoader::scanPluginDir: Found: %s", entName.c_str());
				pluginHandles[entName] = NULL;
			}
		}
		// If we have a symbolic link...
		else if (ent->d_type == DT_LNK)
		{
			log(LOG_WARN, "Found symbolic link whilst searching for plugins, ignoring");
		}
	}

	// Tidy up
	closedir(dir);
#endif
}


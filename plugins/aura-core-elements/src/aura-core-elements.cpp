// Includes:
#include "aura-core-elements.h"

// Copyright (c) 2014 Clayton Peters
// This program is distributed under the terms of the GNU Lesser General Public
// License (LGPL). A copy of the license is included in the file COPYING.LESSER

aura_plugin_t plugin;
aura_plugin_desc_t description;

typedef struct ace_colour_object_t
{
	// Superclass details (must come first)
	aura_object_instance_t parent;

	// Colour
	float r;
	float g;
	float b;
} ace_colour_object_t;

aura_plugin_desc_t* getDescription()
{
	return &description;
}

void unload()
{
	delete [] description.objectTypes;
}

aura_object_instance_t* create_object(aura_plugin_type_t pluginType, const char* name)
{
	if (pluginType != AURA_PLUGIN_TYPE_ELEMENT)
	{
		return NULL;
	}

	if (strcmp(name, "colour") == 0)
	{
		// Initial object creation
		ace_colour_object_t* object = new ace_colour_object_t;
		object->parent.pluginType = AURA_PLUGIN_TYPE_ELEMENT;
		object->parent.objectType = "colour";

		return (aura_object_instance_t*)object;
	}
	else if (strcmp(name, "gradient") == 0)
	{
		// TODO
		return NULL;
	}
	else if (strcmp(name, "text") == 0)
	{
		// TODO
		return NULL;
	}
	else if (strcmp(name, "image") == 0)
	{
		// TODO
		return NULL;
	}
	else
	{
		return NULL;
	}
}

extern "C" aura_plugin_t* aura_plugin_load()
{
	plugin.getDescription = getDescription;
	plugin.unload = unload;
	plugin.create = create_object;
	plugin.propertyChanged = NULL;

	description.name = "Aura Core Elements";
	description.author = "Clayton Peters";
	description.description = "Provide core visual elements to aura, such as blocks of colour, gradient, text and images";
	description.version = "0.1.0";
	description.pluginType = AURA_PLUGIN_TYPE_ELEMENT;
	description.objectTypes = new const char*[5];
	description.objectTypes[0] = "colour";
	description.objectTypes[1] = "gradient";
	description.objectTypes[2] = "text";
	description.objectTypes[3] = "image";
	description.objectTypes[3] = NULL;

	return &plugin;
}



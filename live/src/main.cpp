// Copyright (c) 2014 Clayton Peters
// This program is distributed under the terms of the GNU Lesser General Public
// License (LGPL). A copy of the license is included in the file COPYING.LESSER

// Includes:
#include <libaura/aura.h>
#include <stdio.h>
#include <getopt.h>
#include <stdarg.h>
#include "AuraLive.h"
#include "Log.h"
#include "AuraException.h"

/// Defines the entry point for the application. Parses the command line arguments and passes them
/// to a new global instance of an AuraLive application object.
/// @param argc The number of arguments passed to the application
/// @param argv The parameters passed on the command line
int main(int argc, char** argv)
{
	bool windowed = false;
	string resolution;
	string pluginsPath = "./plugins";

	// Enable debug log level in DEBUG builds (or rather in not NDEBUG builds)
#ifndef NDEBUG
	log_set_level(LOG_DEBUG);
#endif

	// Definitions of our command line arguments
	struct option cmdOptions[] = {
		{ "windowed", no_argument, 0, 'w' },
		{ "resolution", required_argument, 0, 'r' },
		{ "plugins-path", required_argument, 0, 'p' },
	};

	// Iterate over our command line arguments
	int option, optionIndex = 0;
	while ((option = getopt_long(argc, argv, "wr:p:", cmdOptions, &optionIndex)) != -1)
	{
		switch (option)
		{
			case 'w':
				windowed = true;
				break;
			case 'r':
				resolution = optarg;
				break;
			case 'p':
				pluginsPath = optarg;
				break;
			default:
				return 1;
				break;
		}
	}

	log(LOG_DEBUG, "main: Running with libaura version: %d.%d.%d\n", aura_get_version() / 1000000, aura_get_version() % 1000000 / 1000, aura_get_version() % 1000);

	try
	{
		int result = aura_init();
		if (result != 0)
		{
			throw AuraException(AURA_ERR_LIBAURAINIT, "libaura initialisation failed");
		}

		AuraLive& auraLive = AuraLive::initInstance(pluginsPath);
		SDL_Delay(5000);
	}
	catch (AuraException e1)
	{
		log(LOG_FATAL, "Error: Code %d: %s\n", e1.getCode(), e1.what());
		return 1;
	}
	catch (exception e2)
	{
		log(LOG_FATAL, "Unhandled exception: %s\n", e2.what());
		return 1;
	}

	return 0;
}


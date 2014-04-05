// Copyright (c) 2014 Clayton Peters
// This program is distributed under the terms of the GNU Lesser General Public
// License (LGPL). A copy of the license is included in the file COPYING.LESSER

#ifndef AURA_LIVE_H_INCLUDED
#define AURA_LIVE_H_INCLUDED

// Includes:
#include <libaura/aura.h>
#include <string>
#include <SDL.h>
#include "PluginLoader.h"

// Namespaces:
using namespace std;

/// The AuraLive class is the main application class.
/// @author Clayton Peters
class AuraLive
{
	public:
		/// Get the single instance of the application
		static AuraLive& getInstance();

		/// Initialise the single instance of the application.
		static AuraLive& initInstance(string pluginPath);

		/// Destroys an AuraLive object
		~AuraLive();

		/// The plugin loader object
		PluginLoader pluginLoader;

	private:
		/// Private constructor. Constructs a new AuraLive object
		AuraLive(string pluginPath);

		/// The single global instance
		static AuraLive* globalInstance;

		/// The SDL main window
		SDL_Window* mainWindow;

		/// The SDL GL context
		SDL_GLContext mainContext;
};

#endif


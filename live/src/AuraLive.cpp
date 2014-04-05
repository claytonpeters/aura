// Copyright (c) 2014 Clayton Peters
// This program is distributed under the terms of the GNU Lesser General Public
// License (LGPL). A copy of the license is included in the file COPYING.LESSER

// Includes:
#include "AuraLive.h"
#include "AuraException.h"
#include "Log.h"
#include <GL/gl.h>

/// Single static global instance (singleton)
AuraLive* AuraLive::globalInstance = NULL;

/// Private constructor. Constructs a new AuraLive object
AuraLive::AuraLive(string pluginPath) :
	pluginLoader(pluginPath),
	mainWindow(NULL),
	mainContext(NULL)
{
	// Initialise SDL
	log(LOG_DEBUG, "AuraLive::AuraLive: Initialising SDL");
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		throw AuraException(AURA_ERR_SDLINITFAILED, SDL_GetError());
	}

	// Set OpenGL 3.2
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	// Create the main window
	log(LOG_DEBUG, "AuraLive::AuraLive: Creating main window");
	mainWindow = SDL_CreateWindow("Aura Live!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if (mainWindow == NULL)
	{
		throw AuraException(AURA_ERR_SDLWINDOWFAILED, SDL_GetError());
	}

	// Create the OpenGL context
	log(LOG_DEBUG, "AuraLive::AuraLive: Creating OpenGL 3.2 context");
	mainContext = SDL_GL_CreateContext(mainWindow);
	if (mainContext == NULL)
	{
		throw AuraException(AURA_ERR_SDLGLCONTEXTFAILED, SDL_GetError());
	}

	glClearColor(1.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
	SDL_GL_SwapWindow(mainWindow);
}

/// Destroys an AuraLive object
AuraLive::~AuraLive()
{
	// Tidy up context and window
	if (mainContext != NULL)
	{
		SDL_GL_DeleteContext(mainContext);
	}
	if (mainWindow != NULL)
	{
		SDL_DestroyWindow(mainWindow);
	}

	// Tidy up SDL
	SDL_Quit();
}

AuraLive& AuraLive::initInstance(string pluginPath)
{
	// If we already have an instance, throw an exception
	if (globalInstance)
	{
		throw AuraException(AURA_ERR_GOTINSTANCE, "Attempted to create a second AuraLive instance");
	}

	// Initialise the global instance (this may throw an AuraException)
	globalInstance = new AuraLive(pluginPath);

	// Return the new instance
	return *globalInstance;
}

AuraLive& AuraLive::getInstance()
{
	// If we don't have an instance, throw an exception
	if (globalInstance == NULL)
	{
		throw AuraException(AURA_ERR_NOINSTANCE, "AuraLive instance has not yet been instantiated");
	}

	return *globalInstance;
}

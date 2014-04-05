// Copyright (c) 2014 Clayton Peters
// This program is distributed under the terms of the GNU Lesser General Public
// License (LGPL). A copy of the license is included in the file COPYING.LESSER

// Includes:
#include "Log.h"

/// Log level, defaults to LOG_WARN
static int g_logLevel = LOG_WARN;

// Default to logging to stderr
static FILE* g_logFP = stderr;

/// Set up logging to log to another file handle
/// @param fp The file pointer of the file/stream to log to
void log_set_handle(FILE* fp)
{
	g_logFP = fp;
}

/// Set the log level. Any message set to log at a level higher than this will not be printed
/// @param logLevel The new log level
void log_set_level(int logLevel)
{
	g_logLevel = logLevel;
}

/// Gets the current log level
int log_get_level()
{
	return g_logLevel;
}

/// Logging function
/// @param logLevel The loglevel to format at
/// @param format The format string, followed by arguments
void log(int logLevel, const char* format, ...)
{
	if (logLevel <= g_logLevel)
	{
		va_list args;
		va_start(args, format);
		vfprintf(g_logFP, format, args);
		fprintf(g_logFP, "\n");
		va_end(args);
	}
}


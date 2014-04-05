// Copyright (c) 2014 Clayton Peters
// This program is distributed under the terms of the GNU Lesser General Public
// License (LGPL). A copy of the license is included in the file COPYING.LESSER

#ifndef LOG_H_INCLUDED
#define LOG_H_INCLUDED

// Includes:
#include <stdarg.h>
#include <stdio.h>

// Verbosity levels:
#define LOG_INFO   40
#define LOG_DEBUG  30
#define LOG_WARN   20
#define LOG_ERROR  10
#define LOG_FATAL  0

/// Set up logging to log to another file handle
/// @param fp The file pointer of the file/stream to log to
void log_set_handle(FILE* fp);

/// Set the log level. Any message set to log at a level higher than this will not be printed
/// @param logLevel The new log level
void log_set_level(int logLevel);

/// Gets the current log level
int log_get_level();

/// Logging function
/// @param logLevel The loglevel to format at
/// @param format The format string, followed by arguments
void log(int logLevel, const char* format, ...);

#endif


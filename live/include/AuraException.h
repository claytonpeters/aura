// Copyright (c) 2014 Clayton Peters
// This program is distributed under the terms of the GNU Lesser General Public
// License (LGPL). A copy of the license is included in the file COPYING.LESSER

#ifndef AURA_EXCEPTION_H_INCLUDED
#define AURA_EXCEPTION_H_INCLUDED

// Includes:
#include <stdexcept>

// Namespaces:
using namespace std;

// Definitions:
#define AURA_ERR_NOINSTANCE	      1
#define AURA_ERR_GOTINSTANCE	      2
#define AURA_ERR_BADPLUGINDIR	      3
#define AURA_ERR_SDLINITFAILED	      4
#define AURA_ERR_SDLWINDOWFAILED      5
#define AURA_ERR_SDLGLCONTEXTFAILED   6
#define AURA_ERR_LIBAURAINIT          7

/// The AuraException class is a class for exceptions in Aura that are specific to the application
/// @author Clayton Peters
class AuraException : public runtime_error
{
	public:
		AuraException(int _code, const string& _reason) : runtime_error(_reason), code(_code) {}
		virtual ~AuraException() {}
		virtual int getCode() { return code; }

	private:
		int code;
};

#endif


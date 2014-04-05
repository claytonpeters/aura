// Copyright (c) 2014 Clayton Peters
// This program is distributed under the terms of the GNU Lesser General Public
// License (LGPL). A copy of the license is included in the file COPYING.LESSER

// Includes:
#include <curl/curl.h>
#include <string>
#include <sstream>
#include <map>
#include "aura.h"

using namespace std;

typedef map<string, aura_property_t*> property_map_t;

/// Gets the version of libaura
int aura_init()
{
	// Perform global CURL initialisation
	CURLcode result = curl_global_init(CURL_GLOBAL_ALL);
	if (result != 0)
	{
		return 1;
	}

	return 0;
}

/// Called from CURL when we have data
static size_t curl_callback(char* ptr, size_t size, size_t nmemb, void* userdata)
{
	// Get our buffer from the custom userdata, put the data from ptr in the buffer, snf yrll
	// CURL we processed everythong
	stringbuf* buffer = (stringbuf*)userdata;
	buffer->sputn(ptr, size * nmemb);
	return size * nmemb;
}

/// Uses CURL to synchronously download the given resource
aura_download_data_t* aura_download_sync(const char* url)
{
	aura_download_data_t* response = NULL;

	// Initialise CURL for this request
	CURL* curl = curl_easy_init();
	if (curl == NULL)
	{
		return NULL;
	}

	// Set up a string buffer
	stringbuf buffer;

	// Set up what we want CURL to do
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_callback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
	curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 15);
	//curl_easy_setopt(curl, CURLOPT_PROXY, TODO);
	//curl_easy_setopt(curl, CURLOPT_PROXYPORT, TODO);
	//curl_easy_setopt(curl, CURLOPT_PROXYTYPE, TODO);

	// Perform the operation
	CURLcode result = curl_easy_perform(curl);
	if (result == 0)
	{
		string bufferData = buffer.str();

		// Build our response object
		response = new aura_download_data_t;
		response->dataLength = bufferData.length();

		// Copy in the response data (we add one to add a null terminator)
		response->data = new char[bufferData.length() + 1];
		memcpy(response->data, bufferData.c_str(), bufferData.length());
		response->data[bufferData.length()] = '\0';

		// Get the HTTP response code
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response->responseCode);

		// Get the elapsed time
		curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &response->totalTime);
	}
	else
	{
	}

	// Tidy up
	curl_easy_cleanup(curl);

	return response;
}

/// Frees all the information associated with a download
void aura_free_download_data(aura_download_data_t* downloadData)
{
	if (downloadData->data != NULL)
	{
		delete [] downloadData->data;
	}
	delete downloadData;
}

/// Creates a property list
aura_properties_t aura_create_property_list()
{
	return new property_map_t();
}

/// Frees a property map list, deleting all the properties within it
void aura_delete_property_list(aura_properties_t properties)
{
	property_map_t* propertyMap = (property_map_t*)properties;

	// TODO: Delete all the properties first
	delete propertyMap;
}

/// Takes an aura_properties_t property list and a string for the name of the property to get and
/// returns that property from the list
aura_property_t* aura_get_property(aura_properties_t properties, const char* name)
{
	property_map_t& propertyMap = *(property_map_t*)properties;
	auto propertyIter = propertyMap.find(name);
	return (propertyIter == propertyMap.end()) ? NULL : propertyIter->second;
}

/// Takes an aura_properties_t property list and deletes the given property from the list
void aura_delete_property(aura_properties_t properties, const char* name)
{
	property_map_t& propertyMap = *(property_map_t*)properties;
	auto propertyIter = propertyMap.find(name);
	if (propertyIter != propertyMap.end())
	{
		propertyMap.erase(propertyIter);
	}
}

/// Takes an aura_properties_t property list and adds the given property to the list
void aura_add_property(aura_properties_t properties, aura_property_t* property)
{
	property_map_t& propertyMap = *(property_map_t*)properties;
	propertyMap[property->name] = property;
}


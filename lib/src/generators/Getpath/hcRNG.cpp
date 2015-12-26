
/* @file hcRNG.cpp
* @brief Implementation of functions defined in hcRNG.h
*/
#include "hcRNG/hcRNG.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#define CASE_ERR_(code,msg) case code: base = msg; break
#define CASE_ERR(code)      CASE_ERR_(HCRNG_ ## code, MSG_ ## code)


//tern char errorString[1024];
char errorString[1024]                          = "";

static const char MSG_DEFAULT[]                 = "unknown status";
static const char MSG_SUCCESS[]                 = "success";
static const char MSG_OUT_OF_RESOURCES[]        = "out of resources";
static const char MSG_INVALID_VALUE[]           = "invalid value";
static const char MSG_INVALID_RNG_TYPE[]        = "invalid type of RNG";
static const char MSG_INVALID_STREAM_CREATOR[]  = "invalid stream creator";
static const char MSG_INVALID_SEED[]            = "invalid seed";

const char* hcrngGetErrorString()
{
	return errorString;
}


static char lib_path_default1[] = "/usr";
static char lib_path_default1_check[] = "/usr/include/hcRNG/hcRNG.h";
static char lib_path_default2[] = ".";

const char* hcrngGetLibraryRoot()
{
	const char* lib_path = getenv("HCRNG_PATH");

	if (lib_path == NULL) {
		// check if lib_path_default1_check exists
		if (
		access(lib_path_default1_check, F_OK) != -1
		)
			return lib_path_default1;
		// last resort
		return lib_path_default2;
	}
	else
		return lib_path;
}


static char lib_includes[1024];

const char* hcrngGetLibraryDeviceIncludes(int* err)
{
	if (err) 
		*err = HCRNG_SUCCESS;

	int nbytes;
	nbytes = snprintf(
		lib_includes,
		sizeof(lib_includes),
		"-I\"%s/include\"",
		hcrngGetLibraryRoot());

	if (nbytes >= sizeof(lib_includes)) {
		if (err)
			*err = hcrngSetErrorString(HCRNG_OUT_OF_RESOURCES, "value of HCRNG_PATH too long (max = %u)", sizeof(lib_includes) - 16);
		return NULL;
	}
	return lib_includes;
	}

hcrngStatus hcrngSetErrorString(int err, const char* msg, ...)
{
    char formatted[1024];
    const char* base;
    switch (err) {
        CASE_ERR(SUCCESS);
        CASE_ERR(OUT_OF_RESOURCES);
        CASE_ERR(INVALID_VALUE);
        CASE_ERR(INVALID_RNG_TYPE);
        CASE_ERR(INVALID_STREAM_CREATOR);
        CASE_ERR(INVALID_SEED);
        default: base = MSG_DEFAULT;
    }
    va_list args;
    va_start(args, msg);
    vsprintf(formatted, msg, args);
    sprintf(errorString, "[%s] %s", base, formatted);
    va_end(args);
        return (hcrngStatus)err;
}

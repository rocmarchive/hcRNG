/*
Copyright (c) 2015-2016 Advanced Micro Devices, Inc. All rights reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

/* @file hcRNG.cpp
* @brief Implementation of functions defined in hcRNG.h
*/

#include "include/hcRNG/hcRNG.h"
#include <stdio.h>
#include <stdlib.h>
#define CASE_ERR_(code, msg) \
  case code:                 \
    base = msg;              \
    break
#define CASE_ERR(code) CASE_ERR_(HCRNG_##code, MSG_##code)

char errorString[1024] = "";
static const char MSG_DEFAULT[] = "unknown status";
static const char MSG_SUCCESS[] = "success";
static const char MSG_OUT_OF_RESOURCES[] = "out of resources";
static const char MSG_INVALID_VALUE[] = "invalid value";
static const char MSG_INVALID_RNG_TYPE[] = "invalid type of RNG";
static const char MSG_INVALID_STREAM_CREATOR[] = "invalid stream creator";
static const char MSG_INVALID_SEED[] = "invalid seed";
static const char MSG_FUNCTION_NOT_IMPLEMENTED[] = "function not implemented";

const char* hcrngGetErrorString() { return errorString; }

static char lib_path_default[] = ".";

const char* hcrngGetLibraryRoot() {
  const char* lib_path = getenv("HCRNG_PATH");

  if (lib_path == NULL) {
    return lib_path_default;
  } else {
    return lib_path;
  }
}

hcrngStatus hcrngSetErrorString(int err, const char* msg, ...) {
  char formatted[1024];
  const char* base;
  switch (err) {
    CASE_ERR(SUCCESS);
    CASE_ERR(OUT_OF_RESOURCES);
    CASE_ERR(INVALID_VALUE);
    CASE_ERR(INVALID_RNG_TYPE);
    CASE_ERR(INVALID_STREAM_CREATOR);
    CASE_ERR(INVALID_SEED);
    CASE_ERR(FUNCTION_NOT_IMPLEMENTED);
    default:
      base = MSG_DEFAULT;
  }
  va_list args;
  va_start(args, msg);
  vsprintf(formatted, msg, args);
  sprintf(errorString, "[%s] %s", base, formatted);
  printf("%s\n", errorString);
  va_end(args);
  return (hcrngStatus)err;
}

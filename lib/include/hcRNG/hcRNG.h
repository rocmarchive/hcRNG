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

/*
*
*  FILENAME : hcRNG.h
*  This file is the top level header file which includes the HcRNGlibrary class
*  for performing three random number generations.
*
*/

#pragma once
#ifndef HCRNG_H
#define HCRNG_H

/** \internal
 */

#ifdef HCRNG_SINGLE_PRECISION
#define _HCRNG_FPTYPE float
#else
#define _HCRNG_FPTYPE double
#endif
#define _HCRNG_TAG_FPTYPE(name) _HCRNG_TAG_FPTYPE_(name, _HCRNG_FPTYPE)
#define _HCRNG_TAG_FPTYPE_(name, fptype) _HCRNG_TAG_FPTYPE__(name, fptype)
#define _HCRNG_TAG_FPTYPE__(name, fptype) name##_##fptype
/** \endinternal
 */

#define HCRNGAPI

/*! @brief Error codes
 *
 *  Most library functions return an error status indicating the success or
 *  error state of the operation carried by the function.
 *  In case of success, the error status is set to `HCRNG_SUCCESS`.
 *  Otherwise, an error message can be retrieved by invoking
 *  hcrngGetErrorString().
 *
 *  @note In naming this type hcrngStatus, we follow the convention from hcFFT
 *  and hcBLAS, where the homologous types are name hcfftStatus and
 *  hcblasStatus, respectively.
 */
typedef enum hcrngStatus_ : short {
  HCRNG_SUCCESS = 0,
  HCRNG_OUT_OF_RESOURCES = -1,
  HCRNG_INVALID_VALUE = -2,
  HCRNG_INVALID_RNG_TYPE = -3,
  HCRNG_INVALID_STREAM_CREATOR = -4,
  HCRNG_INVALID_SEED = -5,
  HCRNG_FUNCTION_NOT_IMPLEMENTED = -6
} hcrngStatus;

static int rngtyp;

/*! @brief Retrieve the last error message.
 *
 *  The buffer containing the error message is internally allocated and must
 *  not be freed by the client.
 *
 *  @return     Error message or `NULL`.
 */
HCRNGAPI const char* hcrngGetErrorString();

/*! @brief Retrieve the library installation path
 *
 *  @return Value of the HCRNG_ROOT environment variable, if defined; else,
 *  `/usr` if the file `/usr/include/hcRNG/hcRNG.h` exists; or, the current
 *  directory (.) of execution of the program otherwise.
 */
HCRNGAPI const char* hcrngGetLibraryRoot();
/*! @brief Set the current error string
 *
 *  The error string will be constructed based on the error code \c err and on
 *  the optional message \c msg.
 *
 *  @param[in]  err     Error code.
 *  @param[in]  msg     Additional error message (format string).  Can be
 * `NULL`.
 *  @param[in]  ...     Additional arguments for the format string.
 *  @return     The value of err (for convenience).
 */
hcrngStatus hcrngSetErrorString(int err, const char* msg, ...);

#endif /* HCRNG_H */

/*
 * vim: syntax=c.doxygen spell spelllang=en fdm=syntax fdls=0
 */

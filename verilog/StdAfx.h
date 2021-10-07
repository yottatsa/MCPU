// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers

// Windows Header Files:
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#endif

#ifndef _WIN32
#error "is not supposed to work"
#define _itoa_s(a, b, c, d) ;
#endif
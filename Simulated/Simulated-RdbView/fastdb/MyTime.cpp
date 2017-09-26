// -*- C++ -*-
/*!
 * @file Time_posix.h
 * @brief Time functions
 * @date $Date$
 * @author Noriaki Ando <n-ando@aist.go.jp>
 *
 * Copyright (C) 2008
 *     Task-intelligence Research Group,
 *     Intelligent Systems Research Institute,
 *     National Institute of
 *         Advanced Industrial Science and Technology (AIST), Japan
 *     All rights reserved.
 *
 * $Id$
 *
 */
#include "MyTime.h"
#include <stdlib.h>
//#include <coil/config_coil.h>
//#include <coil/TimeValue.h>
#define BUFF_SIZE 1024 
 
wchar_t * ANSIToUnicode( const char* str ) 
{ 
     int textlen ; 
     wchar_t * result; 
     textlen = MultiByteToWideChar( CP_ACP, 0, str,-1, NULL,0 ); 
     result = (wchar_t *)malloc((textlen+1)*sizeof(wchar_t)); 
     memset(result,0,(textlen+1)*sizeof(wchar_t)); 
     MultiByteToWideChar(CP_ACP, 0,str,-1,(LPWSTR)result,textlen ); 
     return result; 
} 
char * UnicodeToANSI( const wchar_t* str ) 
{ 
     char* result; 
     int textlen; 
     textlen = WideCharToMultiByte( CP_ACP, 0, str, -1, NULL, 0, NULL, NULL ); 
     result =(char *)malloc((textlen+1)*sizeof(char)); 
     memset( result, 0, sizeof(char) * ( textlen + 1 ) ); 
     WideCharToMultiByte( CP_ACP, 0, str, -1, result, textlen, NULL, NULL ); 
     return result; 
} 
 
wchar_t * UTF8ToUnicode( const char* str ) 
{ 
     int textlen ; 
     wchar_t * result; 
     textlen = MultiByteToWideChar( CP_UTF8, 0, str,-1, NULL,0 ); 
     result = (wchar_t *)malloc((textlen+1)*sizeof(wchar_t)); 
     memset(result,0,(textlen+1)*sizeof(wchar_t)); 
     MultiByteToWideChar(CP_UTF8, 0,str,-1,(LPWSTR)result,textlen ); 
     return result; 
} 
 
char* UTF8ToANSI(const char* str) 
{ 
     return UnicodeToANSI(UTF8ToUnicode(str)); 
} 
char * UnicodeToUTF8( const wchar_t* str ) 
{ 
     char* result; 
     int textlen; 
     textlen = WideCharToMultiByte( CP_UTF8, 0, str, -1, NULL, 0, NULL, NULL ); 
     result =(char *)malloc((textlen+1)*sizeof(char)); 
     memset(result, 0, sizeof(char) * ( textlen + 1 ) ); 
     WideCharToMultiByte( CP_UTF8, 0, str, -1, result, textlen, NULL, NULL ); 
     return result; 
} 

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

#ifndef COIL_TIME_H
#define COIL_TIME_H
#ifdef WIN32
#include <math.h>
#include <windows.h>
#include <winsock.h>
//#pragma comment(lib, "WS2_32.LIB")
#include <time.h>
//#include <coil/config_coil.h>
//#include <coil/TimeValue.h>
wchar_t * ANSIToUnicode( const char* str ) ;
char * UnicodeToANSI( const wchar_t* str ) ;

 
wchar_t * UTF8ToUnicode( const char* str ) ;
 
 
char* UTF8ToANSI(const char* str) ;

char * UnicodeToUTF8( const wchar_t* str ) ;

#define snprintf _snprintf
	#define EPOCHFILETIME (116444736000000000i64)
struct timezone {
	  int tz_minuteswest;
	  int tz_dsttime;
  };
inline unsigned int sleep(unsigned int seconds)
  {

    ::Sleep( seconds *1000 );
    return 0;
  }


  inline int usleep(unsigned int usec)
  {
    Sleep(usec/1000);
    return 0;

  }

  inline int gettimeofday(struct timeval *tv, struct timezone *tz)
  {
	  FILETIME        ftime;
	  LARGE_INTEGER   lint;
	  __int64         val64;
	  static int      tzflag;
	  if (tv != NULL)
	  {
		  ::GetSystemTimeAsFileTime(&ftime);
		  lint.LowPart  = ftime.dwLowDateTime;
		  lint.HighPart = ftime.dwHighDateTime;
		  val64 = lint.QuadPart;
		  val64 = val64 - EPOCHFILETIME;
		  val64 = val64 / 10;
		  tv->tv_sec  = (long)(val64 / 1000000);
		  tv->tv_usec = (long)(val64 % 1000000);
	  }
	  if (tz)
	  {
		  if (!tzflag)
		  {
			  ::_tzset();
			  ++tzflag;
		  }
		  long tzone = 0;
		  ::_get_timezone(&tzone);
		  tz->tz_minuteswest = tzone / 60;
		  int dlight = 0;
		  ::_get_daylight(&dlight);
		  tz->tz_dsttime = dlight;
	  }
	  return 0;
  }

  //inline TimeValue gettimeofday()
  //{
  //  timeval tv;
  //  coil::gettimeofday(&tv, 0);
  //  return TimeValue(tv.tv_sec, tv.tv_usec);
  //}

  inline int settimeofday(const struct timeval *tv , const struct timezone *tz)
  {

	  SYSTEMTIME systime;
	  FILETIME ftime;
	  LARGE_INTEGER lint;
	  __int64 val64;
	  int bias(0);

	// tv,tz -> ftime
	if (tv != NULL)
	{
		if (tz != NULL)
		{
			bias = tz->tz_minuteswest;
		}

		val64 = (tv->tv_sec + bias * 60) * 1000000;
		val64 = val64 + tv->tv_usec;
		lint.QuadPart = val64;
		ftime.dwHighDateTime = lint.LowPart;
		ftime.dwHighDateTime = lint.HighPart;
		::FileTimeToSystemTime(&ftime, &systime);
		::SetSystemTime(&systime);
	}

    return 0;
  }

#define snprintf _snprintf
#endif
#endif // COIL_TIME_H

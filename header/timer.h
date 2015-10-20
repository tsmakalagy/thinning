#ifndef TIMER_H_
#define TIMER_H_

/*!
* \file timer.h
*
* Useful functions for time measures
*
* \date 10/11/2010
* \author Arnaud Ramey, Víctor González (vgonzale@ing.uc3m.es)
*/



// c includes
#include <stdlib.h>
#include <stdio.h>
#include "cdebug.h"


#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <stdint.h> // portable: uint64_t   MSVC: __int64 

// MSVC defines this in winsock2.h!?
typedef struct timeval {
    long tv_sec;
    long tv_usec;
} timeval;

int gettimeofday(struct timeval * tp, struct timezone * tzp)
{
    // Note: some broken versions only have 8 trailing zero's, the correct epoch has 9 trailing zero's
    static const uint64_t EPOCH = ((uint64_t) 116444736000000000ULL);

    SYSTEMTIME  system_time;
    FILETIME    file_time;
    uint64_t    time;

    GetSystemTime( &system_time );
    SystemTimeToFileTime( &system_time, &file_time );
    time =  ((uint64_t)file_time.dwLowDateTime )      ;
    time += ((uint64_t)file_time.dwHighDateTime) << 32;

    tp->tv_sec  = (long) ((time - EPOCH) / 10000000L);
    tp->tv_usec = (long) (system_time.wMilliseconds * 1000);
    return 0;
}


/*#if defined(_MSC_VER) || defined(__MINGW32__)
#include "Windows.h"
int gettimeofday(struct timeval* tp, void* tzp) {
    DWORD t;
    t = timeGetTime();
    tp->tv_sec = t / 1000;
    tp->tv_usec = t % 1000;
    
    return 0;
}
#endif
*/

/*!
 * simple functions
 */
long int getSeconds();
long int getMiliSeconds();



/* Just gets current system time in milliseconds */
double getSystemTimeMSec(){

  struct timeval cur_time;
  double cur_time_msec, cur_time_msec_usec;

  /* get current time */
  gettimeofday(&cur_time, NULL);
  cur_time_msec = (double)cur_time.tv_sec;
  cur_time_msec_usec = (double)cur_time.tv_usec;
  cur_time_msec = (cur_time_msec*1000.0 + cur_time_msec_usec/1000.0) + 0.5;
  return cur_time_msec;
}




#ifdef __cplusplus // Fer C++ function
#include <string>
std::string getDateTime();
#endif


/*!
 * victor functions
 */
#ifdef __cplusplus
extern "C" {
#endif

/* Calculate time diferences */
long int timeDifferenceUsec (struct timeval * before, struct timeval * after);
long int timeDifferenceMsec (struct timeval * before, struct timeval * after);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
/*!
 * a timer class - it uses 'float' as a time type
 */
class Timer {
public:
  typedef float Time;
  const static Time NOTIME;

  Timer() {
    reset();
  }

  ~Timer() {
  }

  //! reset time to 0
  virtual inline void reset() {
    gettimeofday(&start, NULL);
    //usleep(2000);
  }

  //! get the time since ctor or last reset (milliseconds)
  virtual inline Time getTimeMilliseconds() const {
    struct timeval end;
    gettimeofday(&end, NULL);
    return (Time) (// seconds
                   (end.tv_sec - start.tv_sec)
                   * 1000 +
                   // useconds
                   (end.tv_usec - start.tv_usec)
                   / 1000.f);
  }

  //! get the time since ctor or last reset (seconds)
  virtual inline Time getTimeSeconds() const {
    return getTimeMilliseconds() / 1000.f;
  }

  //! get the time since ctor or last reset (milliseconds)
  virtual inline Time time() const {
    return getTimeMilliseconds();
  }

  //! print time needed for a task identified by its string
  virtual inline void printTime(const char* msg) {
    maggiePrint("Time for %s : %g ms.", msg, getTimeMilliseconds());
  }

  //! print time needed for a task identified by its string
  virtual inline void printTime_factor(const char* msg, const int times) {
    maggiePrint("Time for %s (%i times) : %g ms.",
                msg, times, getTimeMilliseconds() / times);
  }

private:
  struct timeval start;
};

#endif // C++

#endif /*TIMER_H_*/


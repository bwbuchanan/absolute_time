/*
    Copyright (C) 2011 Brian Buchanan. All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions
    are met:
    1. Redistributions of source code must retain the above copyright
       notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
       notice, this list of conditions and the following disclaimer in the
       documentation and/or other materials provided with the distribution.

    THIS SOFTWARE IS PROVIDED BY AUTHOR AND CONTRIBUTORS ``AS IS'' AND
    ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
    ARE DISCLAIMED.  IN NO EVENT SHALL AUTHOR OR CONTRIBUTORS BE LIABLE
    FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
    DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
    OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
    HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
    LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
    OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
    SUCH DAMAGE.
*/

#include <ruby.h>
#include <time.h>
#ifdef HAVE_MACH_H
#include <mach/mach.h>
#endif
#ifdef HAVE_MACH_MACH_TIME_H
#include <mach/mach_time.h>
#endif

static double get_absolute_time(void);

/*
 * Document-module: AbsoluteTime
 *
 * Ruby interface to monotonically-increasing system timer.
 */

/*
 * call-seq:
 *   AbsoluteTime.now() -> float
 *
 * Returns the current value of the system timer as a floating-point number of seconds.
 * Although the units of the return value are seconds, they cannot be safely
 * interpreted as a wall clock time.  The return value is only useful when compared to
 * the return value of a previous or subsequent call to AbsoluteTime.now().
 *
 * If a monotonically increasing system clock is not available, this function falls back
 * to using the wall clock time.  You can check to see whether a monotonic clock is
 * available using AbsoluteTime.monotonic?()
 */
static VALUE
module_now(VALUE self_)
{
    return rb_float_new(get_absolute_time());
}

/*
 * call-seq:
 *   AbsoluteTime.realtime() { || ... } -> float
 *
 * Like Benchmark.realtime(), returns the elapsed time to execute the specified block,
 * as a floating-point number of seconds.
 */
static VALUE
module_realtime(VALUE self_)
{
    double start_time, end_time;
    
    start_time = get_absolute_time();
    rb_yield(Qnil);
    end_time = get_absolute_time();
    
    return rb_float_new(end_time - start_time);
}

/*
 * call-seq:
 *   AbsoluteTime.monotonic?() -> Boolean
 *
 * Returns true if this module is able to use a guaranteed monotonically-increasing clock,
 * false otherwise.
 */
static VALUE
module_is_monotonic(VALUE self_)
{
#if defined(HAVE_CLOCK_GETTIME) && defined(CLOCK_MONOTONIC) || defined(HAVE_MACH_MACH_TIME_H)
    return Qtrue;
#else
    return Qfalse;
#endif
}

void
Init_absolute_time(void)
{
    VALUE absolute_time_module = rb_define_module ("AbsoluteTime");
    rb_define_singleton_method (absolute_time_module, "now", module_now, 0);
    rb_define_singleton_method (absolute_time_module, "realtime", module_realtime, 0);
    rb_define_singleton_method (absolute_time_module, "monotonic?", module_is_monotonic, 0);
}

static double
get_absolute_time(void)
{
#if defined(HAVE_CLOCK_GETTIME) && defined(CLOCK_MONOTONIC)
    struct timespec ts;
    
    if (clock_gettime(CLOCK_MONOTONIC, &ts) == -1) {
        rb_sys_fail("clock_gettime");
    }
    return (double)ts.tv_sec + (double)ts.tv_nsec / 1e9;
#else
#  if defined(HAVE_MACH_MACH_TIME_H)
    uint64_t t;
    static mach_timebase_info_data_t timebase_info;
    
    /* If this is the first time we've run the function, then get the timebase info. */
    if (timebase_info.denom == 0)
        mach_timebase_info(&timebase_info);
    
    t = mach_absolute_time();
    return (double)t * timebase_info.numer / timebase_info.denom / 1e9;
#  else
    struct timeval tv;
    if (gettimeofday(&tv, 0) < 0)
        rb_sys_fail("gettimeofday");
    return (double)tv.tv_sec + (double)tv.tv_usec / 1e6;
#  endif
#endif
}

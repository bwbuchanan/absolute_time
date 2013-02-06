# AbsoluteTime

This gem provides a monotonically increasing timer to permit safe measurement of time intervals.

Using Time.now() for measuring intervals is not reliable—and sometimes unsafe—because the
system clock may be stepped forwards or backwards between the two measurements, or may be
running slower or faster than real time in order to effect clock synchronization with UTC.

The module uses OS-specific functions such as mach_absolute_time() and clock_gettime() to
access the system tick counter.  The time values returned by this module cannot be interpreted
as real time clock values; they are only useful for comparison with another time value from
this module.

Also, please note that because the timer is dependent on the time since the system was
booted, it is not meaningful to compare values of AbsoluteTime.now across reboots or between
different machines.  It should, however, be safe to compare a timer value recorded by one process
with a value recorded by a different process on the same machine.

The resolution of the timer is system-dependent.

## Usage:

    # Is a monotonically increasing timer available on this system?
    >> AbsoluteTime.monotonic?
    => true

    # Time an action by recording the start and end times.
    >> start_time = AbsoluteTime.now
    >> value = function_that_takes_a_long_time()
    >> end_time = AbsoluteTime.now
    >> puts "Function took #{end_time - start_time} seconds to complete."
    
    Function took 5.0002783499658108 seconds to complete.

    # Time an action using AbsoluteTime.realtime, which acts like Benchmark.realtime.
    >> AbsoluteTime.realtime { function_that_takes_a_long_time() }
    => 5.0002783499658108

Supported platforms: Darwin (Mac OS X), FreeBSD, Linux

If you can add support for another platform, please do so and send me a pull request on GitHub.  A Windows implementation using GetTickCount64() / GetTickCount() / QueryPerformanceCounter() would be much appreciated!

## Signed Gem

For your protection, this gem is cryptographically signed by the author. The signing certificate is available at:

https://raw.github.com/bwbuchanan/absolute_time/master/gem-public_cert.pem

You can add this certificate to your local list of trusted gem signers by running these commands:

    curl -O https://raw.github.com/bwbuchanan/absolute_time/master/gem-public_cert.pem
    gem cert --add gem-public_cert.pem

And once you have done that, you can take advantage of RubyGems' signed gem security by installing
the gem with this command:

    gem install absolute_time -P HighSecurity

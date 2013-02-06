# -*- encoding: utf-8 -*-
Gem::Specification.new do |s|
  s.name        = "absolute_time"
  s.version     = '1.0.0'
  s.authors     = ["Brian Buchanan"]
  s.email       = ["bwb@holo.org"]
  s.homepage    = ""
  s.summary     = %q{Reliable monotonically increasing timer for measuring time intervals}
  s.description = %q{This gem provides a monotonically increasing timer to permit safe measurement of time intervals.

Using Time.now for measuring intervals is not reliable (and sometimes unsafe) because the
system clock may be stepped forwards or backwards between the two measurements, or may be
running slower or faster than real time in order to effect clock synchronization with UTC.

The module uses OS-specific functions such as mach_absolute_time() and clock_gettime() to
access the system tick counter.  The time values returned by this module cannot be interpreted
as real time clock values; they are only useful for comparison with another time value from
this module.
}

  s.rubyforge_project = "absolute_time"
  s.licenses = ['BSD']
  s.homepage = 'https://github.com/bwbuchanan/absolute_time'

  s.files         = `git ls-files`.split("\n")
  s.test_files    = `git ls-files -- {test,spec,features}/*`.split("\n")
  s.executables   = `git ls-files -- bin/*`.split("\n").map{ |f| File.basename(f) }
  s.extensions    = 'ext/extconf.rb'
  s.has_rdoc      = true
  s.extra_rdoc_files = `git ls-files -- ext/*.c`.split("\n")
  s.cert_chain    = ['gem-public_cert.pem']
  s.signing_key   = "#{ENV['HOME']}/.keys/gem-private_key.pem"
end

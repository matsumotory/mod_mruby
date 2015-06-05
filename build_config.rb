MRuby::Build.new do |conf|

  toolchain :gcc

  conf.gembox 'full-core'

  #
  # Recommended for mod_mruby
  #
  conf.gem :github => 'iij/mruby-io'
  conf.gem :github => 'iij/mruby-env'
  conf.gem :github => 'iij/mruby-dir'
  conf.gem :github => 'iij/mruby-digest'
  conf.gem :github => 'iij/mruby-process'
  conf.gem :github => 'iij/mruby-pack'
  conf.gem :github => 'mattn/mruby-json'
  conf.gem :github => 'mattn/mruby-onig-regexp'
  conf.gem :github => 'matsumoto-r/mruby-redis'
  conf.gem :github => 'matsumoto-r/mruby-vedis'
  conf.gem :github => 'matsumoto-r/mruby-sleep'
  conf.gem :github => 'matsumoto-r/mruby-userdata'
  conf.gem :github => 'matsumoto-r/mruby-uname'
  conf.gem :github => 'matsumoto-r/mruby-cache'
  conf.gem :github => 'matsumoto-r/mruby-mutex'

  # mod_mruby extended class
  conf.gem '../mrbgems/mod_mruby_mrblib'

  # use memcached
  # conf.gem :github => 'matsumoto-r/mruby-memcached'

  # build error on travis ci 2014/12/01, commented out mruby-file-stat
  # conf.gem :github => 'ksss/mruby-file-stat'

  # use markdown on mod_mruby
  # conf.gem :github => 'matsumoto-r/mruby-discount'

  # have GeoIPCity.dat
  # conf.gem :github => 'matsumoto-r/mruby-geoip'

  # Linux only for mod_mruby
  # conf.gem :github => 'matsumoto-r/mruby-capability'
  # conf.gem :github => 'matsumoto-r/mruby-cgroup'

  # C compiler settings
  conf.cc do |cc|
    if ENV['BUILD_TYPE'] == "debug"
      cc.flags << '-fPIC -g3 -Wall -Werror-implicit-function-declaration'
    else
      cc.flags << '-fPIC'
    end
    if ENV['BUILD_BIT'] == "64"
      cc.flags << ' -DMRB_INT64'
    end
  end

  # Linker settings
  conf.linker do |linker|
    if ENV['BUILD_BIT'] == "64"
      linker.flags = '-DMRB_INT64'
    end
  end
end

MRuby::Build.new do |conf|
  # load specific toolchain settings
  toolchain :gcc

  # Use mrbgems
  # conf.gem 'examples/mrbgems/ruby_extension_example'
  # conf.gem 'examples/mrbgems/c_extension_example' do |g|
  #   g.cc.flags << '-g' # append cflags in this gem
  # end
  # conf.gem 'examples/mrbgems/c_and_ruby_extension_example'
  # conf.gem :github => 'masuidrive/mrbgems-example', :branch => 'master'
  # conf.gem :git => 'git@github.com:masuidrive/mrbgems-example.git', :branch => 'master', :options => '-v'

  # include the default GEMs
  conf.gembox 'default'

  #
  # Recommended for mod_mruby
  #
  # comment out because of moving error.h
  conf.gem :git => 'git://github.com/iij/mruby-io.git'
  conf.gem :git => 'git://github.com/iij/mruby-process.git'
  conf.gem :git => 'git://github.com/iij/mruby-pack.git'
  conf.gem :git => 'git://github.com/iij/mruby-env.git'
  conf.gem :git => 'git://github.com/iij/mruby-dir.git'
  conf.gem :git => 'git://github.com/iij/mruby-digest.git'
  conf.gem :git => 'git://github.com/mattn/mruby-json.git'
  conf.gem :git => 'git://github.com/matsumoto-r/mruby-redis.git'
  conf.gem :git => 'git://github.com/matsumoto-r/mruby-vedis.git'
  #conf.gem :git => 'git://github.com/matsumoto-r/mruby-memcached.git'
  conf.gem :git => 'git://github.com/matsumoto-r/mruby-sleep.git'
  conf.gem :git => 'git://github.com/matsumoto-r/mruby-userdata.git'
  conf.gem :git => 'git://github.com/matsumoto-r/mruby-uname.git'
  conf.gem :git => 'git://github.com/mattn/mruby-onig-regexp.git'

  # mod_mruby extended class
  conf.gem :git => 'git://github.com/matsumoto-r/mruby-mod-mruby-ext.git'

  # use markdown on mod_mruby
  #conf.gem :git => 'git://github.com/matsumoto-r/mruby-discount.git'

  # Linux only for mod_mruby
  #conf.gem :git => 'git://github.com/matsumoto-r/mruby-capability.git'
  #conf.gem :git => 'git://github.com/matsumoto-r/mruby-cgroup.git'

  # C compiler settings
  conf.cc do |cc|
  #   cc.command = ENV['CC'] || 'gcc'
    if ENV['BUILD_TYPE'] == "debug"
      cc.flags << '-fPIC -g3 -Wall -Werror-implicit-function-declaration'
    else
      cc.flags << '-fPIC'
    end
    if ENV['BUILD_BIT'] == "64"
      cc.flags << ' -DMRB_INT64'
    end
  #   cc.include_paths = ["#{root}/include"]
  #   cc.defines = %w(DISABLE_GEMS)
  #   cc.option_include_path = '-I%s'
  #   cc.option_define = '-D%s'
  #   cc.compile_options = "%{flags} -MMD -o %{outfile} -c %{infile}"
  end

  # mrbc settings
  # conf.mrbc do |mrbc|
  #   mrbc.compile_options = "-g -B%{funcname} -o-" # The -g option is required for line numbers
  # end

  # Linker settings
  conf.linker do |linker|
  #   linker.command = ENV['LD'] || 'gcc'
  #   linker.flags = [ENV['LDFLAGS'] || []]
  #   linker.flags_before_libraries = []
  #   linker.libraries = %w()
  #   linker.flags_after_libraries = []
  #   linker.library_paths = []
  #   linker.option_library = '-l%s'
  #   linker.option_library_path = '-L%s'
    if ENV['BUILD_BIT'] == "64"
      linker.flags = '-DMRB_INT64'
    end
  #   linker.link_options = "%{flags} -o %{outfile} %{objs} %{libs}"
  end

  # Archiver settings
  # conf.archiver do |archiver|
  #   archiver.command = ENV['AR'] || 'ar'
  #   archiver.archive_options = 'rs %{outfile} %{objs}'
  # end

  # Parser generator settings
  # conf.yacc do |yacc|
  #   yacc.command = ENV['YACC'] || 'bison'
  #   yacc.compile_options = '-o %{outfile} %{infile}'
  # end

  # gperf settings
  # conf.gperf do |gperf|
  #   gperf.command = 'gperf'
  #   gperf.compile_options = '-L ANSI-C -C -p -j1 -i 1 -g -o -t -N mrb_reserved_word -k"1,3,$" %{infile} > %{outfile}'
  # end

  # file extensions
  # conf.exts do |exts|
  #   exts.object = '.o'
  #   exts.executable = '' # '.exe' if Windows
  #   exts.library = '.a'
  # end

  # file separetor
  # conf.file_separator = '/'
end

# Define cross build settings
# MRuby::CrossBuild.new('32bit') do |conf|
#   toolchain :gcc
#
#   conf.cc.flags << "-m32"
#   conf.linker.flags << "-m32"
#
#   conf.build_mrbtest_lib_only
#
#   conf.gem 'examples/mrbgems/c_and_ruby_extension_example'
#
#   conf.test_runner.command = 'env'
#
# end

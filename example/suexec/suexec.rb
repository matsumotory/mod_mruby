#
# use mruby-process, mruby-random, mruby-capability
#

uid = 501

pid = Process.fork {
  c = Capability.new
  r = Apache::Request.new
  c.get_proc
  cap = [Capability::CAP_SETUID, Capability::CAP_SETGID]
  c.set Capability::CAP_PERMITTED, cap
  c.set Capability::CAP_EFFECTIVE, cap
  c.setuid(uid)
  c.clear Capability::CAP_EFFECTIVE, cap
  c.clear Capability::CAP_PERMITTED, cap
  r.run_handler
  Apache.errlogger 4, "suexec for #{r.filename}. set to uid #{uid}."
}

Process.waitpid pid

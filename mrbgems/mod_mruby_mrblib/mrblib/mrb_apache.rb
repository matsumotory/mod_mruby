class Apache
  class Request
    def reverse_proxy path
      self.handler  = "proxy-server"
      self.proxyreq = Apache::PROXYREQ_REVERSE
      self.filename = "proxy:" + path
    end
  end
  class Headers_in
    def user_agent; self["User-Agent"]; end
    def referer; self["Referer"]; end
  end
  class Filter
    def body
      data = self.flatten
      self.cleanup
      data
    end
    def body= output
      self.insert_tail output
      self.insert_eos
    end
  end
end
module Kernel
  def get_server_class
    Apache
  end
end

class Hoge
  def self.fuga
    Apache::rputs "hello"
  end
end

Hoge::fuga
Apache::rputs self.class.constants.to_s

self.class.remove_const :Hoge
Apache::rputs self.class.constants.to_s


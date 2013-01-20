r = Apache::Request.new
io = WiringPi::GPIO.new
pin = 7

#Apache.rputs "filename = #{r.filename}"
#Apache.rputs "uri = #{r.uri}"

if r.uri == "/led/on"
  io.write(pin, WiringPi::HIGH)
  Apache.return 200
elsif r.uri == "/led/off"
  io.write(pin, WiringPi::LOW)
  Apache.return 200
elsif r.uri == "/led/flash"
  5.times do |v|
    io.write(pin, WiringPi::HIGH)
    Sleep::sleep 1
    io.write(pin, WiringPi::LOW)
    Sleep::sleep 1
  end
  Apache.return 200
else
  Apache.return Apache::DECLINED
end


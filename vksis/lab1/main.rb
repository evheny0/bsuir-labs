# {0=>0, 50=>1, 75=>2, 110=>3, 134=>4, 150=>5, 200=>6, 300=>7, 600=>8, 1200=>9, 1800=>10, 2400=>11, 4800=>12, 9600=>13, 19200=>14, 38400=>15, 57600=>4097, 115200=>4098, 230400=>4099, 460800=>4100, 500000=>4101, 576000=>4102, 921600=>4103, 1000000=>4104, 1152000=>4105, 1500000=>4106, 2000000=>4107, 2500000=>4108, 3000000=>4109, 3500000=>4110, 4000000=>4111}


require './sender'
require './receiver'

logger = Logger.new('./log.txt')

if ARGV[0].nil?
  puts 'No port number given!'
  exit(1)
end

baud_rate = (ARGV[1] || 9600).to_i

serial_port = Serial.new "/dev/pts/#{ARGV[0]}", baud_rate
logger.info("Start at port '#{ARGV[0]}' with baud rate '#{baud_rate}'")


receiver = Receiver.new serial_port, ARGV[0], logger
sender = Sender.new serial_port, ARGV[0], logger
receiver.start
sender.start

sender.wait_for_exit
receiver.stop
logger.info("Closed connection to port '#{ARGV[0]}'")

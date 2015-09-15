require 'rubyserial'
require 'thread'
require 'logger'

class Receiver
  def initialize(serial_port, serial_port_name, logger)
    @serial_port_name = serial_port_name
    @serial_port = serial_port
    @logger = logger
  end

  def start
    @thread = Thread.new do
      loop do
        data = @serial_port.gets
        puts ">> #{data}"
        @logger.info("Recieved message '#{data.chomp}' from port #{@serial_port_name}")
      end
    end
  end

  def stop
    @thread.kill
  end
end

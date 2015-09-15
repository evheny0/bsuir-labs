require 'rubyserial'
require 'thread'
require 'logger'

class Sender
  def initialize(serial_port, serial_port_name, logger)
    @serial_port_name = serial_port_name
    @serial_port = serial_port
    @logger = logger
  end

  def start
    @thread = Thread.new do
      loop do
        text = STDIN.gets
        break if text == "q\n"
        @serial_port.write(text)
        @logger.info("Sent message '#{text.chomp}' to port #{@serial_port_name}")
      end
    end
  end

  def wait_for_exit
    @thread.join
  end
end

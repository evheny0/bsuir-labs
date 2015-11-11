require 'rubyserial'
require 'thread'
require 'logger'
require './package_controller'


class Sender
  def initialize(serial_port, serial_port_name)
    @serial_port_name = serial_port_name
    @serial_port = serial_port
  end

  def start
    @thread = Thread.new do
      loop do
        text = STDIN.gets.chomp
        break if text == "q"
        send_data(text)
      end
    end
  end

  def wait_for_exit
    @thread.join
  end

  private

  def send_data(data)
    packages = PackageController.new(data: data).build_packages
    packages.each do |package|
      @serial_port.write(package.encoded_message.join)
      @serial_port.write("\n")
    end
  end
end

# hamming

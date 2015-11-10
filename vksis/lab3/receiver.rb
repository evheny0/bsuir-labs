require 'rubyserial'
require 'thread'
require 'logger'
require './package'


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
        handle_package(data)
      end
    end
  end

  def stop
    @thread.kill
  end

  private

  def handle_package(data)
    package = PackageController.new(encoded_message: data).parse_package
    @logger << "Package: #{package.raw_data.join(' ')}\n"
    puts ">> #{package.data}"
  end
end

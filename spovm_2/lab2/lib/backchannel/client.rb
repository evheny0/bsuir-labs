require "socket"
require "thread"
require "ipaddr"
require "securerandom"
require "pry"

class Client
  attr_reader :listeners
  MULTICAST_ADDR = "224.6.8.11"
  BIND_ADDR = "0.0.0.0"
  BROADCAST_ADDR = '<broadcast>'
  PORT = 6811

  def initialize(handle, mode)
    @handle = handle
    @mode = mode
    @client_id = SecureRandom.hex(5)
    @listeners = []
  end

  def add_message_listener(listener)
    listen unless listening?
    @listeners << listener
  end

  def transmit(content)
    message = Message.new(
      "client_id" => @client_id,
      "handle"    => @handle,
      "content"   => content
    )
    if @mode == "BROADCAST"
      socket.send(message.to_json, 0, BROADCAST_ADDR, PORT)
    else
      socket.send(message.to_json, 0, MULTICAST_ADDR, PORT)
    end
    message
  end

  private

  def listen
    if @mode == "BROADCAST"
      socket.bind(BROADCAST_ADDR, PORT)
    else
      socket.bind(BIND_ADDR, PORT)
    end

    Thread.new do
      loop do
        attributes, _ = socket.recvfrom(1024)
        message = Message.inflate(attributes)

        unless message.client_id == @client_id
          @listeners.each { |listener| listener.new_message(message) }
        end
      end
    end

    @listening = true
  end

  def listening?
    @listening == true
  end

  def socket
    @socket ||= UDPSocket.open.tap do |socket|
      if @mode == "BROADCAST"
        socket.setsockopt(:SOL_SOCKET, :SO_BROADCAST, 1)
        socket.setsockopt(:SOL_SOCKET, :SO_REUSEADDR, 1)
      else
        socket.setsockopt(Socket::IPPROTO_IP, Socket::IP_ADD_MEMBERSHIP, bind_address)
        socket.setsockopt(Socket::IPPROTO_IP, Socket::IP_MULTICAST_TTL, 1)
        socket.setsockopt(Socket::SOL_SOCKET, Socket::SO_REUSEADDR, 1)
      end
    end
  end

  def bind_address
    IPAddr.new(MULTICAST_ADDR).hton + IPAddr.new(BIND_ADDR).hton
  end
end
# http://stackoverflow.com/questions/14112955/how-to-get-my-machines-ip-address-from-ruby-without-leveraging-from-other-ip-ad

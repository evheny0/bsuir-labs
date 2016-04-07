require "socket"
require "thread"
require "ipaddr"
require "securerandom"
require "pry"

class Client
  attr_reader :listeners, :sock, :mode
  MULTICAST_ADDR = "224.0.0.251"
  BIND_ADDR = "0.0.0.0"
  BROADCAST_ADDR = '192.168.25.255'
  # BROADCAST_ADDR = "<broadcast>"
  PORT = 33333

  def initialize(handle, mode)
    @handle = handle
    @mode = mode
    @client_id = SecureRandom.hex(5)
    @listeners = []
    @sock = ''
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
      @sock.send(message.to_json, 0, BROADCAST_ADDR, PORT)
    else
      @sock.send(message.to_json, 0, MULTICAST_ADDR, PORT)
    end
    message
  end

  def block_member_multicast
    print("Input block source ip:")
    @sock.setsockopt(Socket::IPPROTO_IP,Socket::IP_BLOCK_SOURCE, optval)
  end

  def unblock_member_multicast
    @sock.setsockopt(Socket::IPPROTO_IP,Socket::IP_UNBLOCK_SOURCE, IPAddr.new(MULTICAST_ADDR).hton + IPAddr.new("192.168.25.3").hton + IPAddr.new("192.168.25.2").hton)
  end

  def join_group
    ip =  IPAddr.new(MULTICAST_ADDR).hton + IPAddr.new(local_device_ip).hton
    @sock.setsockopt(Socket::IPPROTO_IP, Socket::IP_ADD_MEMBERSHIP, ip)
  end

  def leave_group
    # binding.pry
    @sock.setsockopt(Socket::IPPROTO_IP, Socket::IP_DROP_MEMBERSHIP, IPAddr.new(MULTICAST_ADDR).hton + IPAddr.new(local_device_ip).hton)
  end

  private

  def listen
    ip =  IPAddr.new(MULTICAST_ADDR).hton + IPAddr.new(local_device_ip).hton
    @sock = UDPSocket.new
    if @mode == "BROADCAST"
      @sock.setsockopt(:SOL_SOCKET, :SO_BROADCAST, 1)
    else
      @sock.setsockopt(Socket::IPPROTO_IP, Socket::IP_ADD_MEMBERSHIP, ip)
      @sock.setsockopt(Socket::IPPROTO_IP, Socket::IP_MULTICAST_IF, IPAddr.new(local_device_ip).hton)
    end
    @sock.bind("", PORT)
    @sock.setsockopt(Socket::SOL_SOCKET, Socket::SO_REUSEADDR, 1) # позвояем использовать порт повторно
    Thread.new do
      loop do
        attributes, _ = @sock.recvfrom(1024)
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

  # def socket
  #   @socket ||= UDPSocket.open.tap do |socket|
  #     if @mode == "BROADCAST"
  #       socket.setsockopt(:SOL_SOCKET, :SO_BROADCAST, 1)
  #       socket.setsockopt(:SOL_SOCKET, :SO_REUSEADDR, 1)
  #     else
  #         socket.setsockopt(Socket::IPPROTO_IP, Socket::IP_MULTICAST_IF, IPAddr.new(local_device_ip).hton)
  #         socket.setsockopt(Socket::IPPROTO_IP, Socket::IP_ADD_MEMBERSHIP, bind_address)#подключение к группе

  #         socket.setsockopt(Socket::IPPROTO_IP, Socket::IP_MULTICAST_LOOP, 1)

  #         socket.setsockopt(Socket::IPPROTO_IP, Socket::IP_MULTICAST_TTL, 1)
  #         socket.setsockopt(Socket::SOL_SOCKET, Socket::SO_REUSEADDR, 1) # позвояем использовать порт повторно
  #     end
  #   end
  # end

  def bind_address
    IPAddr.new(MULTICAST_ADDR).hton + IPAddr.new(local_device_ip).hton
  end

  def local_device_ip
    Socket::ip_address_list[1].inspect_sockaddr
  end

  def optval
    IPAddr.new(MULTICAST_ADDR).hton + IPAddr.new("192.168.25.3").hton + IPAddr.new("192.168.25.2").hton
  end

  # def lala
  #  ipadr = Socket::getaddrinfo(Socket.gethostname,"echo",Socket::AF_INET)[0][3]
  #  IPAddr.new("192.168.25.3").hton
  # end
end
# http://stackoverflow.com/questions/14112955/how-to-get-my-machines-ip-address-from-ruby-without-leveraging-from-other-ip-ad
# https://spin.atomicobject.com/2013/09/30/socket-connection-timeout-ruby/

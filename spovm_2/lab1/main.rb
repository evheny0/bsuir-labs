require "socket"

class ICMPPing
  attr_accessor :host, :data, :sequence, :timeout

  ICMP_TYPE = 8
  ICMP_CODE = 0
  ICMP_ECHOREPLY = 0

  def initialize(host)
    @host = host
  end

  def data=(data_input)
    @data = data_input
    @data_size = @data.size
    @sequence = 0
    @message = ''
    @timeout = 100
  end

  def prepare_socket
    @socket = Socket.new(Socket::PF_INET, Socket::SOCK_RAW, Socket::IPPROTO_ICMP)

    checksum = 0
    @message = [ICMP_TYPE, ICMP_CODE, checksum, ping_id, sequence, @data].pack(pack_string)

    checksum = make_checksum(@message)
    @message = [ICMP_TYPE, ICMP_CODE, checksum, ping_id, sequence, @data].pack(pack_string)
  end

  def ping
    begin
      saddr = Socket.pack_sockaddr_in(0, host)
    rescue Exception
      @socket.close unless @socket.closed?
    end

    start_time = Time.now

    @socket.send(@message, 0, saddr)

    io_array = select([@socket], nil, nil, timeout)

    if io_array.nil? || io_array[0].empty?
      raise 'Timeout'
    end

    recv_data = @socket.recvfrom(1500).first
    type = recv_data[20, 2].unpack('C2').first

    case type
      when ICMP_ECHOREPLY
        if recv_data.length >= 28
          ping_id, seq = recv_data[24, 4].unpack('n3')
        end
      else
        if recv_data.length > 56
          ping_id, seq = recv_data[52, 4].unpack('n3')
        end
    end

    puts Time.now - start_time
  end

  private

  def pack_string
    'C2 n3 A' << @data_size.to_s
  end

  def ping_id
    Process.pid
  end

  def make_checksum(message)
    length = message.length
    num_short = length / 2
    check = 0

    message.unpack("n#{num_short}").each do |short|
      check += short
    end

    if length % 2 > 0
      check += message[length - 1, 1].unpack('C').first << 8
    end

    check = (check >> 16) + (check & 0xffff)
    (~((check >> 16) + check) & 0xffff)
  end
end


ping = ICMPPing.new('google.com')
ping.data = '-'
ping.prepare_socket
ping.ping

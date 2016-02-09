require 'socket'
require 'pry'
require 'pry-nav'
require 'timeout'
require '../lib/IP.rb'

include Socket::Constants

class ICMPPing
  attr_accessor :host, :data, :sequence, :timeout, :host_ip

  ICMP_TYPE = 8
  ICMP_CODE = 0
  ICMP_ECHOREPLY = 0

  PORT = 33434
  MAX_HOPS = 30

  def initialize(host, spoof_ip='')
    @host = host
    @tr_ttl  = 1
    @sequence = 0
    @message = ''
    @timeout = 54
    @socket = Socket.new(Socket::PF_INET, Socket::SOCK_RAW, Socket::IPPROTO_ICMP)
    @socket.setsockopt(Socket::SOL_IP, Socket::IP_HDRINCL, true)
    @spoof_ip = spoof_ip

  end

  def data=(data_input)
    @data = data_input
    @data_size = @data.size
  end

  def ping
    begin
      @host_ip = IPSocket.getaddress(host)
      saddr = Socket.pack_sockaddr_in(0, @host_ip)
    rescue Exception
      @socket.close unless @socket.closed?
      return
    end

    loop do
      start_time = Time.now

      @message = next_message
      @socket.send(@message.to_s, 0, saddr)

      while true
        recv_data, sender = @socket.recvfrom(1500, Socket::MSG_PEEK)
        recv_pid, seq = recv_data[24, 4].unpack('n3')
        if recv_pid == ping_id
          recv_data, sender = @socket.recvfrom(1500)
          break
        end
      end

      resv_pid, seq = recv_data[24, 4].unpack('n3')
      time = ((Time.now - start_time) * 1000).round(1)
      puts "#{@data_size} bytes from #{sender.ip_address}: icmp_seq=#{seq} ttl=#{ttl} time=#{time} ms PID=#{resv_pid}"
      sleep 1
    end
  end

  def traceroute
    begin
      host_address = IPSocket.getaddress(@host)
    rescue Exception => e
      puts "Can not resolve #{@host}"
      puts e.message
      return
    end

  while true
    recv_socket = Socket.new(Socket::AF_INET, Socket::SOCK_RAW, Socket::IPPROTO_ICMP)
    recv_socket.bind(Socket.pack_sockaddr_in(PORT, ""))

    send_socket = Socket.new(Socket::AF_INET, Socket::SOCK_DGRAM, Socket::IPPROTO_UDP)
    send_socket.setsockopt(0, Socket::IP_TTL, @tr_ttl)
    send_socket.connect(Socket.pack_sockaddr_in(PORT, @host))
    send_socket.puts ""

    curr_addr = nil
    curr_name = nil

    begin
      Timeout.timeout(1) {
        data, sender = recv_socket.recvfrom(8192)
        curr_addr = Socket.unpack_sockaddr_in(sender)[1].to_s
      }

      begin
        curr_name = Socket.getaddrinfo(curr_addr, 0, Socket::AF_UNSPEC, Socket::SOCK_STREAM, nil, Socket::AI_CANONNAME)[0][2]
      rescue SocketError => e
        curr_name = curr_addr
      end

      if curr_name.empty?
        curr_host = "*"
      else
        curr_host = "#{curr_name} (#{curr_addr})"
      end
      puts "#{@tr_ttl}\t#{curr_host}"

      if curr_addr == host_address or @tr_ttl > MAX_HOPS
        break
      end
    rescue Timeout::Error
      puts "#{@tr_ttl}\t*"
    ensure
      recv_socket.close
      send_socket.close
    end

    @tr_ttl += 1
  end
end

  def close_socket
    @socket.close unless @socket.closed?
  end

  private

  def pack_string
    'C2 n3 A' << @data_size.to_s
  end

  def ping_id
    @ping_id ||= Process.pid
  end

  def next_message
    @message = make_ip_header(@spoof_ip, @host_ip, make_icmp_header)
  end

  def make_icmp_header
    @sequence += 1
    checksum = 0
    icmp_header = [ICMP_TYPE, ICMP_CODE, checksum, ping_id, sequence, @data].pack(pack_string)

    checksum = make_checksum(icmp_header)
    icmp_header = [ICMP_TYPE, ICMP_CODE, checksum, ping_id, sequence, @data].pack(pack_string)
  end

  def make_checksum(mess)
    length = mess.length
    num_short = length / 2
    check = 0

    mess.unpack("n#{num_short}").each do |short|
      check += short
    end

    if length % 2 > 0
      check += mess[length - 1, 1].unpack('C').first << 8
    end

    check = (check >> 16) + (check & 0xffff)
    (~((check >> 16) + check) & 0xffff)
  end

  def ttl
    ttl ||= @socket.getsockopt(:IP, :TTL).int
  end

  def make_ip_header(source, dest, body)
    ip = IP.new do |b|       # ip_v and ip_hl are set for us by IP class
      b.ip_tos  = 0
      b.ip_id   = 1
      b.ip_off  = 0
      b.ip_ttl  = 64
      b.ip_p    = Socket::IPPROTO_ICMP
      b.ip_src  = source
      b.ip_dst  = dest
      b.body    = body
      b.ip_len  = b.length
      b.ip_sum  = 0 # linux will calculate this for us (QNX won't?)
    end
    ip
  end
end

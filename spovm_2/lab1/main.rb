#!/usr/bin/env ruby

require "socket"
require "pry"
require 'pry-nav'


class ICMPPing
  attr_accessor :host, :data, :sequence, :timeout

  ICMP_TYPE = 8
  ICMP_CODE = 0
  ICMP_ECHOREPLY = 0

  def initialize(host)
    @host = host
    @sequence = 0
    @message = ''
    @timeout = 54
    @socket = Socket.new(Socket::PF_INET, Socket::SOCK_RAW, Socket::IPPROTO_ICMP)
  end

  def data=(data_input)
    @data = data_input
    @data_size = @data.size
  end

  def ping
    begin
      saddr = Socket.pack_sockaddr_in(0, host)
    rescue Exception
      @socket.close unless @socket.closed?
      return
    end

    loop do
      start_time = Time.now

      @message = next_message
      @socket.send(@message, 0, saddr)
      io_array = select([@socket], nil, nil, timeout)

      raise 'Timeout' if io_array.nil? || io_array[0].empty?

      recv_data, sender = @socket.recvfrom(1500)
      type = recv_data[20, 2].unpack('C2').first
      ping_id, seq = recv_data[24, 4].unpack('n3')

      time = ((Time.now - start_time) * 1000).round(1)
      
      puts "#{@data_size} bytes from #{sender.ip_address}: icmp_seq=#{seq} ttl=#{ttl} time=#{time} ms"
      sleep 1
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
    @sequence += 1
    checksum = 0
    @message = [ICMP_TYPE, ICMP_CODE, checksum, ping_id, sequence, @data].pack(pack_string)

    checksum = make_checksum(@message)
    @message = [ICMP_TYPE, ICMP_CODE, checksum, ping_id, sequence, @data].pack(pack_string)
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

  def ttl
    ttl ||= @socket.getsockopt(:IP, :TTL).int
  end
end


ping = ICMPPing.new(ARGV[0])
ping.data = '*' * 56

trap("INT") do
  puts "\nShutting down."
  ping.close_socket
  exit
end

ping.ping

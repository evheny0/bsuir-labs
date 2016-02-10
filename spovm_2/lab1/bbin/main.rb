#!/usr/bin/env ruby
require '../lib/ICMPPing.rb'
case ARGV[0]
when "-h"
  p "---For Ping use:          ./main.rb <host_name>"
  p "---For IP-spoofing use:   ./main.rb -s <host_name> <spoof_ip>"
when "-s"
  ping = ICMPPing.new(ARGV[1], ARGV[2])
else
  ping = ICMPPing.new(ARGV[0])
end


ping.data = '*' * 56

trap("INT") do
  puts "\nShutting down."
  ping.close_socket
  exit
end

ping.ping

require 'colorize'
require './model.rb'

m = Model.new
m.start 1_000
log = m.log
log = log.inject(Hash.new { |h, k| h[k] = 0 }) { |h, l| h[l] += 1; h }

size = log.inject(0) { |s, a| s += a[1] }.to_f
puts "0. 000: #{(log[[1,0,0,0]] + log[[0,0,0,0]]) / size}".colorize(:light_blue)
puts "1. 100: #{(log[[1,1,0,0]] + log[[0,1,0,0]]) / size}".colorize(:light_blue)
puts "2. 001: #{(log[[1,0,0,1]] + log[[0,0,0,1]]) / size}".colorize(:light_blue)
puts "3. 101: #{(log[[1,1,0,1]] + log[[0,1,0,1]]) / size}".colorize(:light_blue)
puts "4. 111: #{(log[[1,1,1,1]] + log[[0,1,1,1]]) / size}".colorize(:light_blue)
puts "5. 211: #{(log[[1,2,1,1]] + log[[0,2,1,1]]) / size}".colorize(:light_blue)
puts "6. 011: #{(log[[1,0,1,1]] + log[[0,0,1,1]]) / size}".colorize(:light_blue)

puts "denial: #{(log[[1,1,0,0]] + 
                 log[[1,1,0,1]] + 
                 log[[1,1,1,1]] + 
                 log[[1,2,1,1]]) / size}".colorize(:red)

0.step(10, 2) do |i| 
  m = Model.new i / 10
  m.start 100_000
  log = m.log
  log = log.inject(Hash.new { |h, k| h[k] = 0 }) { |h, l| h[l] += 1; h }
  size = log.inject(0) { |s, a| s += a[1] }.to_f
  puts "#{i / 10.0}: #{(log[[1,1,0,0]] + 
                   log[[1,1,0,1]] + 
                   log[[1,1,1,1]] + 
                   log[[1,2,1,1]]) / size}".colorize(:red)

end

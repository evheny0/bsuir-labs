require 'colorize'
require './model.rb'

COUNT = 10000
m = Model.new 0.1
m.start COUNT
log = m.log
log = log.inject(Hash.new { |h, k| h[k] = 0 }) { |h, l| h[l] += 1; h }

puts "\n        1     2     3     4     5     6     7     8     9     10 ".colorize(:red)
puts "    ---------------------------------------------------------------".colorize(:red)
[0.1, 0.3, 0.5].each do |probability|
  wasted = []
  printf "#{probability} | ".colorize(:cyan)
  1.upto(10) do |n|
    m = Model.new probability, n
    m.start COUNT
    wasted << ((m.log.select { |i| i.include? 2 }.size * 100.0) / COUNT)
  end
  wasted.each { |w| printf "%5.2f ", w }
  puts ""
end
puts ""

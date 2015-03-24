require 'colorize'
require './model.rb'

COUNT = 10000
m = Model.new 0.1
m.start COUNT
log = m.log
log = log.inject(Hash.new { |h, k| h[k] = 0 }) { |h, l| h[l] += 1; h }

p log
wasted = m.log.select { |i| i.include? 2 }
p wasted.count * 100.0 / COUNT


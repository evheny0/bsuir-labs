require 'colorize'
require './model.rb'
# require 'perftools'
require 'ruby-prof'

RubyProf.start

m = Model.new
m.start 1_000_000
log = m.log
# log = log.inject(Hash.new { |h, k| h[k] = 0 }) { |h, l| h[l] += 1; h }
# p log

result = RubyProf.stop

# Print a flat profile to text
printer = RubyProf::CallStackPrinter.new(result)
printer.print(STDOUT)

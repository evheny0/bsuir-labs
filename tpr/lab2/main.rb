#!/usr/bin/env ruby
require File.dirname(__FILE__) + "/perceptron.rb"
require File.dirname(__FILE__) +  "/../lib/iris_reader.rb"

LEARNING_SET_START = 0
LEARNING_SET_END = 19
CONTROL_SET_START = 20
CONTROL_SET_END = 39


data = DataSet.new to_hash: true
p = Perceptron.new 4, 3
i = 0
loop do
  i += 1
  p.learn_by_set(data.values)
  percent_of_classified = p.percent_of_classified(data.values, LEARNING_SET_START, LEARNING_SET_END)
  break if percent_of_classified >= LEARNING_TRESHOLD
end

puts("Size of learning set: #{(LEARNING_SET_END - LEARNING_SET_START + 1) * 3}")
puts("Size of control set: #{(CONTROL_SET_END - CONTROL_SET_START + 1) * 3}")
printf "\n\n"
puts "Iterations count is: #{i}\n"
printf(" - Recognotion ability: %d%\n", p.percent_of_classified(data.values, LEARNING_SET_START, LEARNING_SET_END) * 100)
printf(" - Generalization ability: %d%\n", p.percent_of_classified(data.values, CONTROL_SET_START, CONTROL_SET_END) * 100)
printf "\n"
p.print_all_outputs(data.values, LEARNING_SET_START, CONTROL_SET_END)

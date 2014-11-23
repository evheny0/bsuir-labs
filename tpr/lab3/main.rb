#!/usr/bin/env ruby
require File.dirname(__FILE__)  + "/desicion_tree.rb"
require "../lib/iris_reader.rb"


data = DataSet.new to_array: true
data.values.shuffle!
d = DesicionTree.new data.values[LEARNING_SET_START..LEARNING_SET_END]

open('index.html', 'w') do |f|
  f.puts "<link type=\"text/css\" rel=\"stylesheet\" href=\"style.css\">"
  f.puts d.print_tree_to_html
end

d.print_all_values(data.values[LEARNING_SET_START..CONTROL_SET_END])
puts("Size of learning set: #{LEARNING_SET_END - LEARNING_SET_START}")
puts("Size of control set: #{CONTROL_SET_END - CONTROL_SET_START}")
printf "\n"
printf(" - Recognotion ability: %d%\n", d.percent_of_classified(data.values[LEARNING_SET_START..LEARNING_SET_END]) * 100)
printf(" - Generalization ability: %d%\n", d.percent_of_classified(data.values[CONTROL_SET_START..CONTROL_SET_END]) * 100)

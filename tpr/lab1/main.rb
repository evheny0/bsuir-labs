require File.dirname(__FILE__)  + "/classifiers.rb"

TRAINING_SET_START = 0
TRAINING_SET_END = 19
CONTROL_SET_START = TRAINING_SET_END + 1
CONTROL_SET_END = 39


l = Learning.new

puts("Size of learning set: #{(TRAINING_SET_END - TRAINING_SET_START + 1) * 3}")
puts("Size of control set: #{(CONTROL_SET_END - CONTROL_SET_START + 1) * 3}")

puts("\n\nAverage method")
puts(" - Recognotion ability:")
l.check_recognition_ability(TRAINING_SET_START, TRAINING_SET_END, :average)
puts(" - Generalization ability:")
l.check_recognition_ability(CONTROL_SET_START, CONTROL_SET_END, :average)
puts("\n\nSiblings method")
puts(" - Recognotion ability:")
l.check_recognition_ability(TRAINING_SET_START, TRAINING_SET_END, :siblings)
puts(" - Generalization ability:")
l.check_recognition_ability(CONTROL_SET_START, CONTROL_SET_END, :siblings)

l.print_table(TRAINING_SET_START, CONTROL_SET_END)

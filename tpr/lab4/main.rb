#!/usr/bin/env ruby



TRAINING_SET_START = LEARNING_SET_START = 0
TRAINING_SET_END = LEARNING_SET_END = 24
CONTROL_SET_START = TRAINING_SET_END + 1
CONTROL_SET_END = 50
LEARNING_SET_COUNT = LEARNING_SET_END - LEARNING_SET_START

require 'pry'
require "../lib/iris_reader.rb"
require "../lab1/classifiers.rb"
require "../lab2/perceptron.rb"
require "../lab3/desicion_tree.rb"

# nFP = FN / nP
# nFN = FP / nN
# Pre = TP / (TP + FP)
# Rec = TP / (TP + FN)
# Se = TP / (TP + FN)
# Sp = TN / (TN + FP)


l = Learning.new


data = DataSet.new to_array: true
training_set = data.values[(50 + TRAINING_SET_START)..(50 + TRAINING_SET_END)] + data.values[(100 + TRAINING_SET_START)..(100 + TRAINING_SET_END)]
control_set = data.values[(50 + CONTROL_SET_START)..(50 + CONTROL_SET_END)] + data.values[(100 + CONTROL_SET_START)..(100 + CONTROL_SET_END)]

d = DesicionTree.new training_set
tp = d.true_count(control_set, "Iris-versicolor")
np = control_set.count { |v| v[:type] == "Iris-versicolor" }
tn = d.true_count(control_set, "Iris-virginica")
nn = control_set.count { |v| v[:type] == "Iris-virginica" }


puts("Size of learning set: #{LEARNING_SET_END - LEARNING_SET_START}")
puts("Size of control set: #{CONTROL_SET_END - CONTROL_SET_START}")
printf "\n"

puts "nFP = #{((np - tp) / np * 100).round(3)}%"
puts "nFN = #{((nn - tn) / nn * 100).round(3)}%"
puts "Pre = #{(tp / np * 100).round(3)}%"
puts "Rec = #{(tp / (tp + nn - tn) * 100).round(3)}%"
puts "Sp = #{(tn / (tn + np - tp) * 100).round(3)}%"

# d.print_all_values(control_set)
printf(" - Recognition ability: %d%\n", d.percent_of_classified(training_set) * 100)
printf(" - Generalization ability: %d%\n", d.percent_of_classified(control_set) * 100)



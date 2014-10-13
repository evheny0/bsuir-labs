#!/usr/bin/env ruby
require 'matrix'
require 'pry'
require "../lib/iris_reader.rb"


T = 0.5  # threshold
START_WEIGHT = [0.3, 0.3, 0.3, 0.3]  # W0, not used
TRUE_OUTPUT = { "Iris-setosa" => [1, 0, 0], "Iris-versicolor" => [0, 1, 0], "Iris-virginica" => [0, 0, 1] }  # D


LEARNING_SET_START = 0
LEARNING_SET_END = 9
CONTROL_SET_START = 10
CONTROL_SET_END = 19
LEARNING_TRESHOLD = 0.95
ETA = 0.5 # learning coefficient
# learning treshold is 91%:
# 1 => 1
# 0.5 => 1..2
# 0.1 => 2..27
# 0.5 => 2..42
# 0.05 => 2..99
# 0.005 => 37..5720
# 0.0005 => 126119

# learning treshold is 95%:
# 1 => 63..4120
# 0.5 => 76..471
# 0.05 => 177..2858
# 0.005 => 11222

def rand_weight
  [rand, rand, rand, rand]
end


class Neuron
  def initialize
    @weight = rand_weight
    @threshold = T
  end

  def weighted_sum input_elems
    Vector.elements(input_elems).inner_product(Vector.elements(@weight)) - @threshold
  end

  def activation_function input
    1 / (1 + Math.exp(-input))
  end

  def weight
    @weight
  end

  def recalculate_weight input, delta, output
    @weight.map!.with_index do |w, i|
      w += ETA * delta * output * (1 - output) * input[i] # delta_w_i = eta * delta * df(e)/de * x_i
    end
  end

  def get_y input_elems
    activation_function(weighted_sum(input_elems))
  end
end



class NeuronLayer < Array
  def calculate_output value
    @input = value
    @output = []
    each do |neuron|
      @output << neuron.get_y(value)
    end
    @output
  end

  def calculate_delta prev_layer_delta, prev_layer_weight
    @delta = []
    count.times do |i|
      @delta << Vector.elements(prev_layer_delta).inner_product(Vector.elements(prev_layer_weight[i]))
    end
    @delta
  end

  def delta=(value)
    @delta = value
  end

  def recalculate_weights
    each_with_index do |neuron, i|
      neuron.recalculate_weight(@input, @delta[i], @output[i])
    end
  end

  def print
    puts ' ############## ' * count
    4.times do |i|
      each do |neuron|
        printf(" # \033[1;30mw%d:\033[0m %+2.3f # ", i + 1, neuron.weight[i])
      end
      printf "\n"
    end
    puts ' ############## ' * count

    @output.each do |value|
      printf("      %2.3f     ", value)
    end
    printf "\n"
  end

  def weights
    input_num.times.map { |i| get_weights_of_input i }
  end

  private

  def get_weights_of_input n
    map { |neuron| neuron.weight[n] }
  end

  def input_num
    num ||= last.weight.count
  end
end



class Perceptron
  def initialize n_hidden_layer, n_output_layer
    @hidden_layer = NeuronLayer.new(n_hidden_layer) { Neuron.new }
    @output_layer = NeuronLayer.new(n_output_layer) { Neuron.new }
  end

  def learn_by_set set
    set.each do |type, values|
      values[LEARNING_SET_START..LEARNING_SET_END].each do |value|
        calculate_output(value)
        calculate_deltas(type)

        # print_net value, type

        recalculate_weight
      end
    end
    # print_net nil, nil
  end

  def calculate_output value
    hidden_y = @hidden_layer.calculate_output value
    @output_y = round_values(@output_layer.calculate_output(hidden_y))
  end

  def calculate_deltas type
    result_deltas = calculate_output_delta(@output_y, type)
    @output_layer.delta = result_deltas
    @hidden_layer.calculate_delta(result_deltas, @output_layer.weights)
  end

  def recalculate_weight
    @output_layer.recalculate_weights
    @hidden_layer.recalculate_weights
  end

  def print_net input, type
    input.each do |value|
      printf("      %2.3f     ", value)
    end if input
    printf "\n\n"
    @hidden_layer.print
    printf "\n"
    @output_layer.print

    p @output_y
    p TRUE_OUTPUT[type]
    printf "\n\n"
    sleep 0.1
  end

  def percent_of_classified set, set_start, set_end
    correct_otput_count = 0
    set.each do |type, values|
      values[set_start..set_end].each do |value|
        correct_otput_count += 1 if (classify(value) == type)
        # print_net value, type
      end
    end
    correct_otput_count / ((set_end - set_start + 1) * 3.0)
  end

  def classify value
    get_type_by_result(calculate_output(value))
  end

  private

  def calculate_output_delta output, type
    Vector.elements(TRUE_OUTPUT[type]) - Vector.elements(output)
  end

  def round_values values
    values.map { |value| value.round }
  end

  def get_type_by_result result
    TRUE_OUTPUT.invert[result]
  end
end

data = DataSet.new
p = Perceptron.new 4, 3
i = 0
loop do
  i += 1
  p.learn_by_set(data.values)
  break if p.percent_of_classified(data.values, LEARNING_SET_START, LEARNING_SET_END) >= LEARNING_TRESHOLD
end


printf(" - Recognotion ability: %d%\n", p.percent_of_classified(data.values, LEARNING_SET_START, LEARNING_SET_END) * 100)
printf(" - Generalization ability: %d%\n", p.percent_of_classified(data.values, CONTROL_SET_START, CONTROL_SET_END) * 100)

puts "\nIterations count is: #{i}"






def count
  s = Vector[5.4, 3.8800000000000003, 1.48, 0.34]
  v = Vector[5.9799999999999995, 2.7, 4.279999999999999, 1.3]
  sum = ((s.magnitude)**2 - (v.magnitude)**2) / 2
  printf("%f*x1 + %f*x2 + %f*x3 + %f*x4 + %f", s[0] - v[0], s[1] - v[1], s[2] - v[2], s[3] - v[3], sum)
end

require "matrix"
require "../lib/iris_reader.rb"

T = 0.5  # threshold
LEARNING_RATE = 0.01  # alpha
START_WEIGHT = [0.5, 0.5, 0.5, 0.5]  # W0
START_OUTPUT = { "Iris-setosa" => 0, "Iris-versicolor" => 1 }  # D

class Neuron
  def initialize w, threshold
    @weight = w
    @threshold = threshold
  end

  def get_weighted_sum input_elems
    (Vector.elements(input_elems) + Vector.elements(@weight)).inject(:+) - @threshold
  end

  def get_y input
    1 / (1 + Math.exp(-input))
  end

  def set_w n, value
    @weight[n] = value
  end

  def recalculate_w y, values, type
    (0..3).each do |n|
      @weight[n] = @weight[n] + (LEARNING_RATE * values[n] * (START_OUTPUT[type] - y.round))
    end
  end

  def get_w n
    @weight[n]
  end

  def type_correct? type, y
    y.round == START_OUTPUT[type]
  end

  def learn_by set
    set.each do |type, values|
      next if type == "Iris-virginica"
      values.each do |value|
        y = get_y(get_weighted_sum(value))
        recalculate_w(y, value, type)
      end
    end
  end

  def print_results set
    i = 0
    set.each do |type, values|
      next if type == "Iris-virginica"
      values.each do |value|
        y = get_y(get_weighted_sum(value))
        printf(" %s #", (type_correct?(type, y)) ? "1" : "\033[1;31m0\033[0m")
        printf("result: %2.2f # %s # %15s # ", y, value.to_s, type)
        printf("%s %f # \n", @weight.to_s, @threshold)
        i += 1 if type_correct?(type, y)
      end
    end
    printf("Recognotion ability is: %d%\n", i)
  end
end

n = Neuron.new START_WEIGHT, T
data = DataSet.new
n.learn_by data.values
n.learn_by data.values
n.learn_by data.values
n.learn_by data.values
n.print_results data.values




def count
  s = Vector[5.4, 3.8800000000000003, 1.48, 0.34]
  v = Vector[5.9799999999999995, 2.7, 4.279999999999999, 1.3]
  sum = ((s.magnitude)**2 - (v.magnitude)**2) / 2
  printf("%f*x1 + %f*x2 + %f*x3 + %f*x4 + %f", s[0] - v[0], s[1] - v[1], s[2] - v[2], s[3] - v[3], sum)

end

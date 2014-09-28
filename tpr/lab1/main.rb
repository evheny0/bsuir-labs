require "matrix"
require "../lib/iris_reader.rb"

TRAINING_SET_START = 15
TRAINING_SET_END = 19
CONTROL_SET_START = TRAINING_SET_END + 1
CONTROL_SET_END = 29



class Classifier
  def initialize data_set
    init_training_sets data_set
  end

  def set_average_training_set(values, type)
    @training_set_average[type] = values.inject(Vector[0, 0, 0, 0]) { |sum, value| sum + Vector.elements(value) }
    @training_set_average[type] = [@training_set_average[type].map { |elem| elem / values.count }]
  end

  def set_siblings_training_set(values, type)
    @training_set_siblings[type] = values.map { |value| Vector.elements(value) }
  end

  def get_type(value, options = {})
    training_set = (options[:method] == :average) ? @training_set_average : @training_set_siblings
    self.send(options[:metric], training_set, Vector.elements(value))
  end

  private

  def init_training_sets data_set
    @training_set_average = {}
    @training_set_siblings = {}
    data_set.values.each do |type, data|
      set_siblings_training_set(data[TRAINING_SET_START..TRAINING_SET_END], type)
      set_average_training_set(data[TRAINING_SET_START..TRAINING_SET_END], type)
    end
  end

  # (a*b) / (||a|| * ||b||)
  def direction_cosines(training_set, value)
    max_index = probable_type = nil
    training_set.each do |key, values|
      values.each do |elem|
        index = value.inner_product(elem) / (value.magnitude * elem.magnitude)
        if max_index.nil? or index > max_index
          max_index = index
          probable_type = key
        end
      end
    end
    [probable_type, max_index]
  end

  # sqrt(sum((a[i] - b[i])^2))
  def euclidean(training_set, value)
    min_index = probable_type = nil
    training_set.each do |key, values|
      values.each do |elem|
        index = Math.sqrt((elem.zip(value).map { |row| (row[0] - row[1])**2 }).reduce(:+))
        if min_index.nil? or index < min_index
          min_index = index
          probable_type = key
        end
      end
    end
    [probable_type, min_index]
  end

  # (a*b) / (||a||^2+||b||^2-(a*b))
  def tanimoto(training_set, value)
    max_index = probable_type = nil
    training_set.each do |key, values|
      values.each do |elem|
        index = value.inner_product(elem) / ((value.magnitude ** 2) + (elem.magnitude ** 2) - value.inner_product(elem))
        if max_index.nil? or index > max_index
          max_index = index
          probable_type = key
        end
      end
    end
    [probable_type, max_index]
  end
end

class Learning
  attr_reader :average

  def initialize
    @data_set = DataSet.new
    @learning_set = Classifier.new @data_set
  end

  def check_recognition_ability(start_value, end_value, method)
    i = 0
    count = Hash.new(0)

    @data_set.values.each do |type, values|
      values[start_value..end_value].each do |elem|
        [:direction_cosines, :euclidean, :tanimoto].each do |metric|
          count[metric] += 1 if @learning_set.get_type(Vector.elements(elem), method: method, metric: metric)[0] == type
        end
        i += 1
      end
    end

    puts "Direction cosines: #{(100.0 * count[:direction_cosines] / i).round(2)}%"
    puts " Euclidean metric: #{(100.0 * count[:euclidean] / i).round(2)}%"
    puts "  Tanimoto metric: #{(100.0 * count[:tanimoto] / i).round(2)}%"
  end

  def print_table(start_value, end_value)
    puts "##############################################################################################################################################################"
    puts " #         Value        #      Class      #   Agv. Cosines   #   Neib. Cosines  #  Avg. Euclidean  #  Neib. Euclidean #   Avg. Tanimoto  #   Neib. Tanimoto # "
    puts "##############################################################################################################################################################"
    @data_set.values.each do |type, values|
      values[start_value..end_value].each do |elem|
        printf(" # %s # %15s # ", elem.to_s, type)
        [:direction_cosines, :euclidean, :tanimoto].each do |metric|
          result = @learning_set.get_type(Vector.elements(elem), method: :average, metric: metric)
          printf("   %.5f : %s   # ", result[1], (result[0] == type) ? "1" : "\033[1;31m0\033[0m")
          result = @learning_set.get_type(Vector.elements(elem), method: :siblings, metric: metric)
          printf("   %.5f : %s   # ", result[1], (result[0] == type) ? "1" : "\033[1;31m0\033[0m")
        end
        puts
      end
    end
  end

end



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

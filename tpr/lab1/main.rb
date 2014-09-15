require "matrix"

TRAINING_SET_START = 0
TRAINING_SET_END = 10
CONTROL_SET_START = TRAINING_SET_END
CONTROL_SET_END = 15
VALUES_COUNT = 50
DATA_FILE_NAME = "sample.iris.data"



class DataSet
  attr_reader :values

  def initialize filename
    @values = Hash.new { |hash, key| hash[key] = [] }
    read_from_file filename
  end

  def read_from_file filename
    File.foreach(filename) do |line|
      next if line == "\n"
      value = line.split(',')
      @values[value[4].chomp] = @values[value[4].chomp] << value[0..3].collect { |i| i.to_f }
    end
  end
end

class LearningSet
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
    probable_type
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
    probable_type
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
    probable_type
  end
end

class Learning
  attr_reader :average

  def initialize
    @data_set = DataSet.new DATA_FILE_NAME
    @learning_set = LearningSet.new @data_set
  end

  def check_recognition_ability(start_value, end_value, method)
    i = 0
    count = Hash.new(0)

    @data_set.values.each do |type, values|
      values[start_value..end_value].each do |elem|
        [:direction_cosines, :euclidean, :tanimoto].each do |metric|
          count[metric] += 1 if @learning_set.get_type(Vector.elements(elem), method: method, metric: metric) == type
        end
        i += 1
      end
    end

    puts "Direction cosines classifier ability is: #{(100.0 * count[:direction_cosines] / i).round(2)}%"
    puts "Euclidean metric classifier ability is: #{(100.0 * count[:euclidean] / i).round(2)}%"
    puts "Tanimoto classifier ability is: #{(100.0 * count[:tanimoto] / i).round(2)}%"
  end

  def print_line(i, elem, type, cosines, euclidean, tanimoto)
    printf("%3d, %15s, %d, %d, %d\n", i, type, cosines ? 1 : 0, euclidean ? 1 : 0, tanimoto ? 1 : 0)
  end

end



l = Learning.new

puts("\n\nAverage method")
puts(" - Learning set")
l.check_recognition_ability(TRAINING_SET_START, TRAINING_SET_END, :average)
puts(" - Control set")
l.check_recognition_ability(TRAINING_SET_END, CONTROL_SET_END, :average)
puts("\n\nSiblings method")
puts(" - Learning set")
l.check_recognition_ability(TRAINING_SET_START, TRAINING_SET_END, :siblings)
puts(" - Control set")
l.check_recognition_ability(TRAINING_SET_END, CONTROL_SET_END, :siblings)

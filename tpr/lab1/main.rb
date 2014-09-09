require "matrix"

TRAINING_SET_START = 0
TRAINING_SET_END = 10
VALUES_COUNT = 50

class Learning
  attr_reader :average

  def initialize
    @average = {}
  end

  def set_training_set(values, type)
    @average[type] = values.inject(Vector[0, 0, 0]) { |sum, value| sum + Vector.elements(value) }
    @average[type] = @average[type].map { |elem| elem / values.count }
  end

  def check_by_direction_cosines(value, type)
    max_index = probable_type = nil
    @average.each do |key, elem|
      # (a*b) / (||a|| * ||b||)
      index = value.inner_product(elem) / (value.magnitude * elem.magnitude)
      if max_index.nil? or index > max_index
        max_index = index
        probable_type = key
      end
    end
    probable_type == type ? true : false
  end

  def check_by_euclidean_metric(value, type)
    min_index = probable_type = nil
    @average.each do |key, elem|
      # sqrt(sum((a[i] - b[i])^2))
      index = Math.sqrt((elem.zip(value).map { |row| (row[0] - row[1])**2 }).reduce(:+))
      if min_index.nil? or index < min_index
        min_index = index
        probable_type = key
      end
    end
    probable_type == type ? true : false
  end

  def check_by_tanimoto(value, type)
    max_index = probable_type = nil
    @average.each do |key, elem|
      # (a*b) / (||a||+||b||-(a*b))
      index = value.inner_product(elem) / ((value.magnitude ** 2) + (elem.magnitude ** 2) - value.inner_product(elem))
      if max_index.nil? or index > max_index
        max_index = index
        probable_type = key
      end
    end
    probable_type == type ? true : false
  end

  def check_raspozn_ability(values_to_check, type)
    direction_cosines_count = euclidean_metric_count = tanimoto_count = 0
    values_to_check.each do |elem|
      direction_cosines_count += 1 if check_by_direction_cosines(Vector.elements(elem), type)
      euclidean_metric_count += 1 if check_by_euclidean_metric(Vector.elements(elem), type)
      tanimoto_count += 1 if check_by_tanimoto(Vector.elements(elem), type)
    end
    puts "Direction cosines classifier ability is: #{100.0 * direction_cosines_count / VALUES_COUNT}%"
    puts "Euclidean metric classifier ability is: #{100.0 * euclidean_metric_count / VALUES_COUNT}%"
    puts "Tanimoto classifier ability is: #{100.0 * tanimoto_count / VALUES_COUNT}%"
  end
end

def read_file
  File.foreach("sample.iris.data") do |line|
    next if line == "\n"
    value = line.split(',')
    @data[value[4].chomp] = @data[value[4].chomp] << value[0..2].collect { |i| i.to_f }
  end
end

def init_training_sets
  iris = Learning.new
  iris.set_training_set(@data['Iris-setosa'][TRAINING_SET_START..TRAINING_SET_END], 'Iris-setosa')
  iris.set_training_set(@data['Iris-versicolor'][TRAINING_SET_START..TRAINING_SET_END], 'Iris-versicolor')
  iris.set_training_set(@data['Iris-virginica'][TRAINING_SET_START..TRAINING_SET_END], 'Iris-virginica')
  iris.check_raspozn_ability(@data['Iris-virginica'], 'Iris-virginica')
end



@data = Hash.new { |hash, key| hash[key] = [] }
read_file
init_training_sets


# i = 0
# Vector.elements(@data['Iris-virginica']).each do |elem|
#   i += 1 if iris.check_by_tanimoto(Vector.elements(elem), 'Iris-virginica')
# end
# puts "Classifier ability is: #{100.0 * i / VALUES_COUNT}%"

require 'csv'

IRIS_FILE_NAME = "../sample.iris.data"


class DataSet
  attr_reader :values

  def initialize
    @values = Hash.new { |hash, key| hash[key] = [] }
    read_from_file IRIS_FILE_NAME
  end

  def initialize sumb
    @values = Array.new
    read_to_array IRIS_FILE_NAME
  end

  def read_from_file filename
    CSV.foreach(filename) do |line|
      next if line.empty?
      @values[line[4].chomp] = @values[line[4].chomp] << line[0..3].collect { |i| i.to_f }
    end
  end

  def read_to_array filename
    CSV.foreach(filename) do |line|
      next if line.empty?
      @values << {sepal_length: line[0].to_f, sepal_width: line[1].to_f, petal_length: line[2].to_f, petal_width: line[3].to_f, type: line[4]}
    end
  end
end

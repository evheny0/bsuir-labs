require 'csv'

IRIS_FILE_NAME = "../sample.iris.data"


class DataSet
  attr_reader :values

  def initialize
    @values = Hash.new { |hash, key| hash[key] = [] }
    read_from_file IRIS_FILE_NAME
  end

  def read_from_file filename
    CSV.foreach(filename) do |line|
      next if line.empty?
      @values[line[4].chomp] = @values[line[4].chomp] << line[0..3].collect { |i| i.to_f }
    end
  end
end

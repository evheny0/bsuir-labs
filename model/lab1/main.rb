require './magic_rand.rb'
require './rand_analyzer'

def print_histogram array, size
  array.each do |_k, v|
    puts('*' * (v * 1000 / size))
  end
end

analyzer = RandAnalyzer.new(MagicRand.new)
puts " ** HISTOGRAM:"
print_histogram analyzer.histogram, analyzer.size
puts " ** EXPECTATION: #{analyzer.expectation}"
puts " ** DISPERSION: #{analyzer.dispersion}"
puts " ** STANDARD DEVIATION: #{analyzer.standard_deviation}"
puts " ** VALUES IN CIRCLE: #{analyzer.check_uniformity}, PI/4 = #{Math::PI / 4}"
puts " ** PERIOD: #{analyzer.period}"

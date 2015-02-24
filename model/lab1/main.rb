require './magic_rand.rb'
require './rand_analyzer'
require 'gnuplot'

def print_histogram(array, size)
  array.each do |_k, v|
    puts('*' * (v * 1000 / size))
  end
end

def get_new_rand
  new_rand = MagicRand.new
  printf "Enter new params? (y/n): "
  if gets.chomp == 'y'
    printf "Enter M: "
    new_rand.m = gets.to_i
    printf "Enter A: "
    new_rand.a = gets.to_i
    printf "Enter seed: "
    new_rand.seed = gets.to_i
  else
    puts "Using default params..."
  end
  new_rand
end

analyzer = RandAnalyzer.new(get_new_rand)
hist = analyzer.histogram
puts " ** EXPECTATION: #{analyzer.expectation}"
puts " ** DISPERSION: #{analyzer.dispersion}"
puts " ** STANDARD DEVIATION: #{analyzer.standard_deviation}"
puts " ** VALUES IN CIRCLE: #{analyzer.check_uniformity} ~= #{Math::PI / 4}"
puts " ** PERIOD: #{analyzer.period}\n\n"
puts '================================================='

hist = hist.sort_by { |a, _| a }.to_h
x = hist.keys.map { |i| i.round 4 }
y = hist.values.map { |i| i.to_f / analyzer.size }

Gnuplot.open do |gp|
  Gnuplot::Plot.new(gp) do |plot|
    plot.title 'Histogram'
    plot.style 'data histograms'
    plot.xtics 'nomirror rotate by -45'
    plot.yrange '[0.045:0.055]'
    plot.data << Gnuplot::DataSet.new([x, y]) do |ds|
      ds.using = '2:xtic(1)'
      ds.title = '1'
    end
  end
end

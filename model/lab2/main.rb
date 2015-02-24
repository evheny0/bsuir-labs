require 'gnuplot'
require './uniform_distribution.rb'
require './gauss_distribution'
require './exponential_distribution'
require './gamma_distribution'
require './triangle_distribution'
require './simpson_distribution'
require '../lab1/rand_analyzer'



def print_distribution_params
  analyzer = RandAnalyzer.new(rand_class.new)
  puts '================================================='
  puts " ***** #{rand_class.to_s} *****"
  puts " ** EXPECTATION: #{analyzer.expectation}"
  puts " ** DISPERSION: #{analyzer.dispersion}"
  puts " ** STANDARD DEVIATION: #{analyzer.standard_deviation}"
  puts "=================================================\n\n"
end

def display_histogram
  hist = analyzer.histogram.sort_by { |a, _| a }.to_h
  x = hist.keys.map { |i| i.round 4 }
  y = hist.values.map { |i| i.to_f / analyzer.size }

  Gnuplot.open do |gp|
    Gnuplot::Plot.new(gp) do |plot|
      plot.title rand_class.to_s
      plot.style 'data histograms'
      plot.xtics 'nomirror rotate by -45'
      # plot.yrange '[0.0:1]'
      plot.data << Gnuplot::DataSet.new([x, y]) do |ds|
        ds.using = '2:xtic(1)'
        ds.title = '1'
      end
    end
  end
end

def print_analysis_results(rand_class)
  print_distribution_params
  display_histogram
end

print_analysis_results UniformDistribution
print_analysis_results GaussDistribution
print_analysis_results ExponentialDistribution
print_analysis_results GammaDistribution
print_analysis_results TriangleDistribution
print_analysis_results SimpsonDistribution

require './uniform_distribution.rb'

class SimpsonDistribution
  def initialize
    @lower = 5
    @upper = 10
    @first_rand = UniformDistribution.new(@lower / 2, @upper / 2)
    @second_rand = UniformDistribution.new(@lower / 2, @upper / 2)
  end

  def get
    @first_rand.get + @second_rand.get
  end
end

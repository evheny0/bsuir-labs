class GammaDistribution
  def initialize
    @nu = 10
    @lambda = 10
  end

  def get
    - (1.0 / @lambda) * some_sum
  end

  def some_sum
    result = 0
    @nu.times { |_| result += Math.log(next_rand) }
    result
  end

  def next_rand
    rand
  end
end

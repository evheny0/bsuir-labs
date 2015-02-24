class ExponentialDistribution
  def initialize
    @lambda = 10
  end

  def get
    - (1.0 / @lambda) * Math.log(next_rand)
  end

  def next_rand
    rand
  end
end

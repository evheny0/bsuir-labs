class UniformDistribution
  def initialize(lower = 0, upper = 10)
    @lower = lower
    @upper = upper
  end

  def get
    @lower + (@upper - @lower) * next_rand
  end

  def next_rand
    rand
  end
end

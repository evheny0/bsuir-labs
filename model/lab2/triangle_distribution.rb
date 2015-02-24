class TriangleDistribution
  def initialize
    @upper = 5
    @lower = 10
  end

  def get
    @lower + (@upper - @lower) * min_rand
  end

  def min_rand
    [next_rand, next_rand].min
  end

  def max_rand
    [next_rand, next_rand].max
  end

  def next_rand
    rand
  end
end

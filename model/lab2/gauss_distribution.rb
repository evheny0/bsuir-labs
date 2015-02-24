class GaussDistribution
  def initialize
    @expectation = 0
    @deviation = 1
  end

  def get
    @expectation + @deviation * 1.41 * some_sum
  end

  def some_sum
    result = 0
    6.times { |_| result += next_rand }
    result - 3
  end

  def next_rand
    rand
  end
end

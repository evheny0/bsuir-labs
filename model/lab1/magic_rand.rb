# Random number realization
class MagicRand
  # M = 4_611_686_018_427_387_903
  attr_accessor :seed, :a, :m
  
  def initialize
    @seed = @prev = 10_000
    @m = 999_999_999
    @a = @m - 9
  end

  def seed=(value)
    @seed = @prev = value
  end

  def get
    @prev = (@a * @prev) % @m
    @prev.to_f / @m
  end
end

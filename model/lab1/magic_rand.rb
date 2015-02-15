# Random number realization
class MagicRand
  # M = 4_611_686_018_427_387_903
  M = 2**20
  A = M - 3
  attr_writer :seed

  def initialize
    @seed = @prev = Time.now.to_i
  end

  def get
    @prev = (A * @prev) % M
    @prev.to_f / M
  end
end

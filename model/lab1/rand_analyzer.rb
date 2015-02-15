# Random analyzer
class RandAnalyzer
  NUM_OF_INTERVALS = 20
  REALIZATION_SIZE = 1_000_000

  def initialize(rand_obj, realization_size = REALIZATION_SIZE)
    @rand = rand_obj
    @realization = (1..realization_size).map { |_| @rand.get }
    @realization_size = realization_size
  end

  def histogram(interals_num = NUM_OF_INTERVALS)
    delta = (@realization.max - @realization.min) / interals_num
    size_of_chunks @realization.sort, delta
  end

  def expectation
    @expectation ||= @realization.inject(&:+) / size
  end

  def dispersion
    coef = 1 / (size - 1.0)
    @dispersion ||= @realization.inject(0) { |a, e| a + (e - expectation)**2 } * coef
  end

  def standard_deviation
    dispersion**(0.5)
  end

  def check_uniformity
    @round_matches ||= nil
    return @round_matches unless @round_matches.nil?
    count = 0
    (0..(size - 1)).step(2) do |i|
      count += 1 if in_circle? @realization[i], @realization[i + 1]
    end
    @round_matches = count * 2.0 / size
  end

  def period
    @period ||= nil
    return @period unless @period.nil?
    number = @realization[1]
    @realization[1] = 0
    @period = @realization.index(number)
    @realization[1] = number
    @period
  end

  def check_uniformity_using_chunks
    coef = 2.0 / size
    @uniformity ||= chunks(@realization).select do |i|
      i[0]**2 + i[1]**2 < 1
    end.count * coef
  end

  def size
    @size ||= @realization.size
  end

  private

  def size_of_chunks(array, delta)
    output = Hash.new(0)
    lower_bound = array.first
    array.each do |i|
      lower_bound += delta if i > (lower_bound + delta)
      output[lower_bound] += 1
    end
    output
  end

  def in_circle?(num1, num2)
    (num1**2 + num2**2) < 1
  end

  def chunks(array)
    result = []
    (0..(array.size - 1)).step(2) { |i| result << array[i, 2] }
    result
  end
end

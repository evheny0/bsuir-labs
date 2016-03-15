class Classifier
  NUM_OF_PARAMS = 2

  def initialize(data)
    @data = data
    @points = data.combination(2).to_a
    @centers = []
  end

  # def find_centers
  #   @centers = []
  #   least_sum_second = 0
  #   least_sum_first = 0

  #   @points.each do |first, second|
  #     # sum = 0
  #     sum_first = 0
  #     sum_second = 0

  #     @data.each do |elem|
  #       sum_first = diff(first.params_normalized, elem.params_normalized)
  #       sum_second = diff(second.params_normalized, elem.params_normalized)
  #       # sum += sum_to_num(diff(first.params_normalized, elem.params_normalized), diff(second.params_normalized, elem.params_normalized))
  #     end

  #     # if (sum < least_sum) || (least_sum == 0)
  #     #   least_sum = sum
  #     #   @centers = [first, second]
  #     # end

  #     if ((sum_first < least_sum_first) || (least_sum_first == 0))
  #       least_sum_first = sum_first
  #       @centers[0] = first
  #     end

  #     if ((sum_second < least_sum_second) || (least_sum_second == 0))
  #       least_sum_second = sum_second
  #       @centers[1] = second
  #     end
  #   end
  # end

  def find_centers
    @centers[0] = @data[0]
    @centers[1] = @data[1]
    @centers[2] = @data[2]

    loop do
      @firsts = []
      @seconds = []
      @thirds = []

      @data.each do |elem|
        first_diff = diff(elem.params_normalized, @centers[0].params_normalized)
        second_diff = diff(elem.params_normalized, @centers[1].params_normalized)
        third_diff = diff(elem.params_normalized, @centers[2].params_normalized)

        # (first_diff < second_diff ? @firsts : @seconds) << elem
        l = lowest3(first_diff, second_diff, third_diff)
        case l
        when first_diff then @firsts
        when second_diff then @seconds
        when third_diff then @thirds
        end << elem
      end
      
      found_center_first = find_local_center(@firsts)
      found_center_second = find_local_center(@seconds)
      found_center_third = find_local_center(@thirds)
      
      if (found_center_first == @centers[0]) && (found_center_second == @centers[1]) &&
        (found_center_third == @centers[2])
        return true
      else
        @centers[0] = found_center_first
        @centers[1] = found_center_second
        @centers[2] = found_center_third
      end
    end
  end

  def find_local_center(locals)
    center = nil
    least_diff = 0
    center_of_mass = locals.inject(Array.new(NUM_OF_PARAMS, 0)) { |result, e| sum(result, e.params_normalized) }.map { |e| e / locals.size }
    locals.each do |elem|
      found_diff = diff(elem.params_normalized, center_of_mass)
      if center.nil? || found_diff < least_diff
        least_diff = found_diff
        center = elem
      end
    end
    center
  end

  def classify(area)
    distanse_to_first = diff(@centers[0].params_normalized, area.params_normalized)
    distanse_to_second = diff(@centers[1].params_normalized, area.params_normalized)
    distanse_to_third = diff(@centers[2].params_normalized, area.params_normalized)
    if lowest3(distanse_to_first, distanse_to_second, distanse_to_third) == distanse_to_first
      :first
    elsif lowest3(distanse_to_first, distanse_to_second, distanse_to_third) == distanse_to_second
      :second
    else
      :third
    end
  end

  private

  def lowest(first, second)
    (first < second) ? first : second
  end

  def lowest3(first, second, third)
    if first < second
      lowest(first, third)
    else
      lowest(second, third)
    end
  end

  def sum(first, second)
    first.map.with_index { |elem, i| elem + second[i] }
  end

  # def diff(first, second)
  #   first.map.with_index { |elem, i| (elem - second[i]).abs }
  # end

  def diff(first, second)
    Math.sqrt(first.map.with_index { |elem, i| (elem - second[i]) ** 2 }.inject(:+))
  end

  def sum_to_num(*params)
    params.map { |e| e.inject(:+) }.inject(:+)
  end
end

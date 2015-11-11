class HammingCoder
  CODE_SIZE = 7
  CODE_SIZE_FOR_TIMES = CODE_SIZE - 1

  def initialize(data)
    @data = data.map(&:to_i)
    @correction_code = Array.new(CODE_SIZE, 0)
    @correction_code_from_data = Array.new(CODE_SIZE, 0)
  end

  def encode
    extend_data
    fill_correction_codes
    set_correction_codes_to_data
    @data
  end

  def decode
    fetch_correction_codes
    if has_mistake?
      fix_mistake
    end
    remove_correction_codes_from_data
    @data
  end

  private

  def extend_data
    CODE_SIZE.times { |i| @data.insert(2**i - 1, 0) }
  end

  def set_correction_codes_to_data
    CODE_SIZE.times { |i| @data[2**i - 1] = @correction_code[i] }
  end

  def fill_correction_codes
    CODE_SIZE.times do |num|
      i = 2 ** num - 1
      (i...@data.size).step(2**(num + 1)).each do |j|
        @data[j..(j + i)].each { |element| @correction_code[num] ^= element.to_i }
      end
    end
  end

  def fetch_correction_codes
    CODE_SIZE.times { |i| @correction_code_from_data[i] = @data[2**i - 1].to_i }
  end

  def remove_correction_codes_from_data
    (CODE_SIZE - 1).downto(0) { |i| @data.delete_at(2**i - 1) }
  end

  def has_mistake?
    fill_correction_codes
    @correction_code.include?(1)
  end

  def fix_mistake
    positions = @correction_code.each_index.select { |i| @correction_code[i] == 1 }
    position = positions.inject(0) { |sum, i| sum += 2**i } - 1
    @data[position] = 1 - @data[position]
  end
end

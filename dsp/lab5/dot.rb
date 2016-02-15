class Dot
  attr_accessor :value, :area, :origin

  def initialize
    @value = :undefined
    @area = 0
  end

  def filled?
    value == :filled
  end

  def clean?
    value == :clean
  end

  def fill!
    value = :filled
  end

  def clean!
    value = :clean
  end

  def bin_value
    if clean?
      0
    elsif filled?
      1
    else
      2
    end
  end
end

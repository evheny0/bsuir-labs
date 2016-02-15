class Integer
  def positive_or_null
    self < 0 ? 0 : self
  end
end

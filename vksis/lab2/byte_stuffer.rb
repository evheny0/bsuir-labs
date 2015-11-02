class ByteStuffer
  FLAG = "01111110"
  ESCAPE_CHAR = "01111101"
  FLAG_REPLACE = ESCAPE_CHAR + "01011110"
  ESCAPE_REPLACE = ESCAPE_CHAR + "01011101"

  def initialize(data)
    @data = data
  end

  def do_stuff
    @data.gsub!(ESCAPE_CHAR, ESCAPE_REPLACE)
    @data.gsub!(/#{FLAG}/, FLAG_REPLACE)
    @data
  end

  def do_unstuff
    @data.gsub!(/#{ESCAPE_REPLACE}/, ESCAPE_CHAR)
    @data.gsub!(/#{FLAG_REPLACE}/, FLAG)
    @data
  end
end

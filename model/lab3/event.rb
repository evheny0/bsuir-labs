class Event
  def initialize block
    @block = block
  end

  def handle
    # puts "#{@block.class}".colorize(:light_blue)
    @block.action
  end
end

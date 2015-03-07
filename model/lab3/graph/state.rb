class State
  attr_accessor :name

  def initialize
  end

  def fill_in(name, links)
    @name ||= name
    @links ||= links unless links.empty?
    self
  end

  def next
    @links[rand(@links.count)] if @links
  end

  def to_s
    "@name: #{@name}, @links: #{@links.map(&:name) if @links}"
  end
end

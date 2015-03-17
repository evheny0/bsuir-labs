require './event.rb'
require './source.rb'
require './channel.rb'
require './queue.rb'
require './end_channel.rb'

class Model
  def initialize(pi1 = 0.5)
    initialize_blocks pi1
    link_blocks
    @events = [@blocks.first]
    @states_log = []
  end

  def add_event(event)
    @events << event
  end

  def start(count)
    count.times do |_|
      handle_events
      save_state
    end
  end

  def log
    @states_log
  end

  private

  def handle_events
    processed_events = @events
    @events = []
    processed_events.each(&:action)
  end

  def save_state
    @states_log << @blocks.map(&:state)
  end

  def initialize_blocks(pi1)
    @blocks = []
    @blocks << Blocks::Source.new(self, probability: 0.5)
    @blocks << Blocks::Channel.new(self, probability: pi1)
    @blocks << Blocks::Queue.new(self)
    @blocks << Blocks::EndChannel.new(self, probability: 0.4)
  end

  def link_blocks
    @blocks.each_with_index do |bl, i|
      bl.prev_block = @blocks[i - 1]
      bl.next_block = @blocks[i + 1]
    end
  end
end

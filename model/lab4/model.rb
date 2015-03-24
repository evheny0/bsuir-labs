require './event.rb'
require './source.rb'
require './channel.rb'
require './end_channel.rb'

class Model
  def initialize(prob = 0.5)
    initialize_blocks prob
    link_blocks
    @states_log = []
  end

  def start(count)
    count.times do |_|
      increase_counter
      save_state
    end
  end

  def log
    @states_log
  end

  private

  def increase_counter
    @blocks.each { |bl| bl.count_up }
  end

  def save_state
    @states_log << @blocks.map(&:state)
    # p @blocks.map(&:state)
  end

  def initialize_blocks(prob)
    @blocks = []
    @blocks << Blocks::Source.new(self, intensity: 9)
    @blocks << Blocks::Channel.new(self, intensity: 3, probability: prob)
    @blocks << Blocks::Channel.new(self, intensity: 3, probability: prob)
    @blocks << Blocks::Channel.new(self, intensity: 3, probability: prob)
    @blocks << Blocks::EndChannel.new(self, intensity: 5)
  end

  def link_blocks
    @blocks[0].next_blocks << @blocks[1]
    @blocks[0].next_blocks << @blocks[2]
    @blocks[0].next_blocks << @blocks[3]
    @blocks[1].next_blocks << @blocks[4]
    @blocks[2].next_blocks << @blocks[4]
    @blocks[3].next_blocks << @blocks[4]
  end
end

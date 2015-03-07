require './event.rb'

module Blocks
  class Block
    attr_reader :blocked, :state
    attr_writer :prev_block, :next_block

    def initialize model, params = {}
      @model = model
      @probability = params[:probability] || 1
      @state = 0
      @blocked = false
    end

    def action
    end

    def add_request
      @state += 1
      generate_event
    end

    def generate_event
      @model.add_event Event.new self
    end

    def event_happend?
      rand > @probability
    end

    def event_happend &block  # check THIS!!!
      if rand > @probability
        yield block
      end
    end
  end
end

# Source > p1 > queue > p2
#             <

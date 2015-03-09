require './event.rb'

module Blocks
  class Block
    attr_reader :blocked, :state
    attr_writer :prev_block, :next_block

    def initialize(model, params = {})
      @model = model
      @probability = params[:probability] || 1
      @state = 0
      @blocked = false
    end

    def action
    end

    def add_request
    end

    def generate_event
      @model.add_event self
    end

    def event_happend?
      rand > @probability
    end

    def event_happend(&block)
      yield block if rand > @probability
    end
  end
end

# Source > p1 > queue > p2
#             <

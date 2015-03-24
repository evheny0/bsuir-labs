require './event.rb'

module Blocks
  class Block
    attr_reader :blocked, :state
    attr_accessor :prev_block, :next_blocks

    def initialize(model, params = {})
      @model = model
      @intensity = 1.0 / params[:intensity] * 9
      @probability = params[:probability]
      @state = 0
      @blocked = false
      @next_blocks = []
      @counter = 0
    end

    def count_up
      clean_up_channels
      @counter += 1
      if @counter >= @intensity
        @counter = 0
        action
      end
    end

    def action
    end

    def add_request
    end

    def busy?
      @state == 1      
    end

    def event_happend?
      rand < @probability
    end

    private

    def clean_up_channels
      @state = 0 if @state == 2
    end
  end
end

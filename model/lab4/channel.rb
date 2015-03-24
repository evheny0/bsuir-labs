require './block.rb'

module Blocks
  class Channel < Block
    BLOCKED = 2

    def action
      @state = 0
      add_to_additional_block if event_happend?
    end

    def add_request
      @state = 1
      @counter = 0
    end

    private

    def add_to_additional_block
      if @next_blocks.first.busy?
        @state = 2
      else
        @next_blocks.first.add_request
      end
    end
  end
end

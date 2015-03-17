require './block.rb'

module Blocks
  class Source < Block
    def action
      generate_event
      if event_happend?
        @state = 1
        @next_block.add_request
      else
        @state = 0
      end
    end
  end
end

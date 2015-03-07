require './block.rb'

module Blocks
  class Source < Block
    def action
      generate_event
      if event_happend?
        @next_block.add_request
      end
    end
  end
end

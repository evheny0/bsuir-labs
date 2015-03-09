require './block.rb'

module Blocks
  class Source < Block
    def action
      generate_event
      @next_block.add_request if event_happend?
    end
  end
end

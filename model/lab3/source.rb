require './block.rb'

module Blocks
  class Source < Block
    def action
      generate_event
      event_happend { @next_block.add_request }
    end
  end
end

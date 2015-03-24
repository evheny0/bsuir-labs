require './block.rb'

module Blocks
  class Source < Block
    def action
      @next_blocks.each do |bl|
        return bl.add_request unless bl.busy?
      end
    end
  end
end

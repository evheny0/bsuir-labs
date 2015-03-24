require './block.rb'

module Blocks
  class EndChannel < Block
    def action
      @state = 0
    end

    def add_request
      @state = 1
    end
  end
end

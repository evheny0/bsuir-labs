require './block.rb'

module Blocks
  CAPACITY = 1

  class Queue < Block
    def add_request
      return false if state == CAPACITY
      unless @next_block.add_request
        @state += 1
      end
      true
    end

    def clear_pending_request
      return 0 if @state == 0
      @state += @prev_block.clear_pending_request - 1
      1
    end
  end
end

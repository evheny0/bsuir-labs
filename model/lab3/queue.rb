require './block.rb'

module Blocks
  CAPACITY = 1

  class Queue < Block
    def add_request
      return false if state == CAPACITY
      @state += 1 unless @next_block.add_request
      true
    end

    def pop_pending_request
      return 0 if @state == 0
      @state += @prev_block.pop_pending_request - 1
      1
    end
  end
end

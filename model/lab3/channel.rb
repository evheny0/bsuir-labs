require './block.rb'

module Blocks
  class Channel < Block
    BLOCKED = 2

    def action
      return generate_event unless event_happend?
      if @next_block.add_request
        @state -= 1
      else
        @state = BLOCKED
      end
    end

    def add_request  # STILL NEEDS SPECIAL STATE
      return false if state > 0
      @state = 1
      generate_event
    end

    def pop_pending_request
      return 0 if @state != BLOCKED
      @state = 0
      1
    end
  end
end

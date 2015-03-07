require './block.rb'

module Blocks
  class EndChannel < Block
    def action
      return generate_event unless event_happend?
      if @prev_block.clear_pending_request == 0
        @state -= 1
      else
        generate_event
      end
    end

    def add_request
      if @state == 1
        false
      else
        @state += 1
        generate_event
      end
    end
  end
end

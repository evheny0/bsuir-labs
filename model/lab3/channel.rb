require './block.rb'

module Blocks
  class Channel < Block
    def action
      if event_happend?
        if @next_block.add_request
          @state -= 1
        else
          @blocked = true
        end
      else
        generate_event
      end
    end

    def add_request  # STILL NEEDS SPECIAL STATE
      return false if state == 1
      @state += 1
      generate_event
    end

    def clear_pending_request
      return 0 unless @blocked
      @blocked = false
      @state -= 1
      1
    end
  end
end

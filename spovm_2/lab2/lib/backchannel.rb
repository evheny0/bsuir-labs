require_relative "backchannel/client"
require_relative "backchannel/window"
require_relative "backchannel/message"

class Backchannel
  def self.start(handle, mode)
    client = Client.new(handle, mode)
    window = Window.new(client)
    window.start
  end
end

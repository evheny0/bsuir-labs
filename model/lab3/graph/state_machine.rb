require './state'
require 'pry'

class StateMachine
  def initialize(states)
    build states
    @current_state = @available_states.first[1]
  end

  def next
    binding.pry
    next_state = @current_state.next
    @current_state = next_state
  end

  def print
    @available_states.each { |s| puts s[1] }
  end

  private

  def build(states)
    @available_states = Hash.new { |h, k| h[k] = State.new }
    states.each do |state_info|
      update_or_create state_info[0], state_info[1]
    end
  end

  def update_or_create info, links = []
    @available_states[info].fill_in(info, links.map { |l| update_or_create l[0] })
  end
end

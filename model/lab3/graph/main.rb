require 'yaml'
require '../lab2/exponential_distribution'
require './state_machine'

states = [['0000', ['0100']],
          ['0100', ['1100', '0001']],
          ['1100', ['0100']],
          ['0001', ['0000', '0101']],
          ['0101', ['0001', '1101', '0011']],
          ['1101', ['0101']],
          ['0011', ['0001', '0111']],
          ['0111', ['0101', '1111', '0111']],
          ['1111', ['0111']]]

f = YAML.load_file 'input.yml'
s = StateMachine.new f
s.next
binding.pry



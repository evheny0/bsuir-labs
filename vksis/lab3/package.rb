require './byte_stuffer'
require './hamming_encoder'
require './logger'

class Package
  FLAG_ARRAY = [0, 1, 1, 1, 1, 1, 1, 0]
  DESTINATION_ARRAY = [0] * 8
  SOURCE_ARRAY = [0] * 8
  CRC_ARRAY = [0] * 8
  DATA_LOCATION = (FLAG_ARRAY.size + DESTINATION_ARRAY.size + SOURCE_ARRAY.size)..(-1)
  BITS_IN_PACKAGE = 12 * 8
  @@weird_param_for_random_mistake_is_true = false

  attr_accessor :data, :encoded_message



  def initialize(params = {})
    @encoded_message = params[:encoded_message]
    @data = params[:data]
  end

  def build
    @data_with_hamming = HammingCoder.new(@data).encode
    # p @data_with_hamming.map(&:to_i)
    @@weird_param_for_random_mistake_is_true = !@@weird_param_for_random_mistake_is_true
    make_mistake if @@weird_param_for_random_mistake_is_true
    frame_with(@data_with_hamming)
  end

  def parse
    @raw_data = @encoded_message[DATA_LOCATION].split('')
    @raw_data.pop
    # p @raw_data.map(&:to_i)
    @raw_data = HammingCoder.new(@raw_data).decode
    @data = to_string(@raw_data)
  end

  def raw_data
    @encoded_message.scan(/\d{8}/).map{ |e| e.to_i(2).to_s(16).rjust(2, '0') }
  end

  def encoded_message
    @encoded_message ||= build
    @encoded_message
  end

  private

  def frame_with(data)
    frame = []
    frame += FLAG_ARRAY
    frame += DESTINATION_ARRAY
    frame += SOURCE_ARRAY
    frame += data
    frame
  end

  def to_string(data)
    data.each_slice(8).map{|e| e.join.to_i(2).chr }.join
  end

  def make_mistake
    LabLogger.info("Send with error")
    position = rand(@data_with_hamming.size)
    @data_with_hamming[position] = 1 - @data_with_hamming[position]
  end
end

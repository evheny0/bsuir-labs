require "./byte_stuffer"
require "./hamming_encoder"

class Package
  FLAG_ARRAY = [0, 1, 1, 1, 1, 1, 1, 0]
  DESTINATION_ARRAY = [0] * 8
  SOURCE_ARRAY = [0] * 8
  CRC_ARRAY = [0] * 8
  DATA_LOCATION = (FLAG_ARRAY.size)..(-1)
  BITS_IN_PACKAGE = 12 * 8

  attr_accessor :data, :encoded_message



  def initialize(params = {})
    @encoded_message = params[:encoded_message]
    @data = params[:data]
  end

  def build
    frame_with(@data)
  end

  def parse
    @stuffed_data = @encoded_message[DATA_LOCATION]
    @data = to_string(@stuffed_data)
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
    frame += CRC_ARRAY
    frame
  end

  def to_string(data)
    data.split('').each_slice(8).map{|e| e.join.to_i(2).chr }.join
  end
end

require "./byte_stuffer"

class Package
  FLAG_ARRAY = [0, 1, 1, 1, 1, 1, 1, 0]
  DESTINATION_ARRAY = [0] * 8
  SOURCE_ARRAY = [0] * 8
  CRC_ARRAY = [0] * 8
  DATA_LOCATION = (FLAG_ARRAY.size + DESTINATION_ARRAY.size + SOURCE_ARRAY.size)..(-CRC_ARRAY.size - 2)
  BITS_IN_PACKAGE = 12 * 8

  attr_accessor :data, :encoded_message



  def initialize(params = {})
    @encoded_message = params[:encoded_message]
    @data = params[:data]
  end

  def build
    @stuffed_data = ByteStuffer.new(@data.join).do_stuff.split('')
    frame_with(@stuffed_data)
  end

  def parse
    @stuffed_data = @encoded_message[DATA_LOCATION]
    @data = to_string(ByteStuffer.new(@stuffed_data).do_unstuff)
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
    frame += full_data(data)
    frame += CRC_ARRAY
    frame
  end

  def to_string(data)
    data.split('').each_slice(8).map{|e| e.join.to_i(2).chr }.join
  end

  def full_data(data)
    data.unshift(0) while data.size < BITS_IN_PACKAGE
    data
  end
end

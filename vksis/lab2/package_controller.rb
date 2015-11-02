require './package'

BITS_IN_PACKAGE = 12 * 8


class PackageController
  def initialize(params = {})
    @data = params[:data]
    @encoded_message = params[:encoded_message]
  end

  def build_packages
    @data_to_packages = string_to_bits(@data)
    @data_to_packages.each_slice(BITS_IN_PACKAGE).map { |p| Package.new(data: p) }
  end

  def parse_package
    package = Package.new(encoded_message: @encoded_message)
    package.parse
    package
  end

  private

  def string_to_bits(string)
    # .unpack('B*')
    string.unpack("c*").map {|char| char_to_byte(char) }.flatten
  end

  def char_to_byte(char)
    array = char.to_s(2).split("").map(&:to_i)
    array.unshift(0) while array.size != 8
    array
  end
end

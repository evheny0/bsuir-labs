require "rmagick"
require "./image_representation"

class CoolImage
  attr_reader :columns, :rows, :pixel, :image, :pixels, :grid

  MAX_BRIGHTNESS = 255 * 255
  MIN_BRIGHTNESS = 0
  BRIGHTNESS_THRESHOLD = 60_000

  def initialize(filename)
    @image = Magick::ImageList.new(filename) { self.depth = 8 }[0]
    @columns = image.columns
    @rows = image.rows
    @pixels = image.get_pixels(0, 0, columns, rows)
    @grid = ImageRepresentation.new(columns, rows)
    # @view = Magick::Image::View.new(a, 0, 0, a.columns, a.rows)
  end

  def to_shades_of_gray
    pixels.each do |pixel|
      avg = (pixel.red + pixel.green + pixel.blue) / 3
      pixel.red = avg
      pixel.blue = avg
      pixel.green = avg
    end
  end

  def pass_brightness_threshold
    pixels.each do |pixel|
      sum = (pixel.red + pixel.green + pixel.blue)
      if sum > BRIGHTNESS_THRESHOLD
        pixel.red = MAX_BRIGHTNESS
        pixel.blue = MAX_BRIGHTNESS
        pixel.green = MAX_BRIGHTNESS
        @grid
      else
        pixel.red = MIN_BRIGHTNESS
        pixel.blue = MIN_BRIGHTNESS
        pixel.green = MIN_BRIGHTNESS        
      end
    end
  end

  def make_grid
    image.each_pixel do |pixel, col, row|
      sum = (pixel.red + pixel.green + pixel.blue)
      if sum > BRIGHTNESS_THRESHOLD
        @grid[col, row] = :filled
      else
        @grid[col, row] = :clean
      end
    end
  end

  def serial_scan
    @grid.serial_scan
    # @grid.simplify_through_dictionary
  end

  def store_and_display
    image.store_pixels(0,0, columns, rows, pixels)
    image.display
  end
end

img = CoolImage.new('1.jpg')
img.make_grid
img.serial_scan
img.grid.print_all
# img.store_and_display

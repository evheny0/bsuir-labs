require "rmagick"
require "./image_representation"

class CoolImage
  attr_reader :columns, :rows, :pixel, :image, :pixels, :grid

  MAX_BRIGHTNESS = 255 * 255
  MIN_BRIGHTNESS = 0
  BRIGHTNESS_THRESHOLD = 80_000
  BLUR = 10.0

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

  def blurred_image
    @image.gaussian_blur(0.0, BLUR)
    # @image.median_filter(BLUR)
  end

  def make_grid
    # binding.pry
    blurred_image.each_pixel do |pixel, col, row|
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
    @grid.form_domains_by_areas
    @grid.filter_domains
  end

  def store_and_display
    image.store_pixels(0, 0, columns, rows, pixels)
    image.display
  end

  def paint_it_black
    grid.domains.each do |domain|
      # color = (domain.klass == :first) ? 0 : MAX_BRIGHTNESS
      # color = (domain.klass == :first) ? :blue : :red
      domain.cells.each do |cell|
        hightlight(pixels[cell[1] * columns + cell[0]], domain.klass)
        # pixels[cell[1] * columns + cell[0]].red = color
        # pixels[cell[1] * columns + cell[0]].blue = color
        # pixels[cell[1] * columns + cell[0]].green = color 
      end
    end
  end

  def hightlight(cell, color)
    cell.red = 0
    cell.green = 0
    cell.blue = 0
    if color == :first
      cell.red += 30000
    elsif color == :second
      cell.green += 30000
    else
      cell.blue += 30000
    end
  end

  def show_domains_with_classes
    grid.domains.sort_by(&:klass).each do |domain|
      puts "#{domain.params_normalized} #{domain.klass}"
    end
  end
end

img = CoolImage.new(ARGV[0])
# img.blur
img.make_grid
img.serial_scan
# img.grid.print_chars
img.grid.classify
img.paint_it_black
img.show_domains_with_classes

# img.grid.print_all
img.store_and_display

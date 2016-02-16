require "./dot"
require "./utils"
require "./domain"
require "pry"

class ImageRepresentation
  attr_reader :columns, :rows

  def initialize(columns, rows)
    @columns = columns
    @rows = rows
    @data = Array.new(columns) { Array.new(rows) { Dot.new } }
  end

  def [](x, y)
    @data[x][y]
  end

  def []=(x, y, value)
    @data[x][y].value = value
  end

  def serial_scan
  #   current_area = 1
  #   @areas_dictionary = { 0 => 0, current_area => current_area }
  #   (0...columns).each do |col|
  #     (0...rows).each do |row|
  #       if @data[col][row].filled?
  #         first_area = @data[(col - 1).positive_or_null][row].area
  #         second_area = @data[col][(row - 1).positive_or_null].area
  #         if (first_area == 0) && (second_area == 0)
  #           @data[col][row].area = current_area
  #           current_area += 1
  #           @areas_dictionary[current_area] = current_area
  #         elsif (first_area == second_area) || (first_area == 0) || (second_area == 0)
  #           @data[col][row].area = not_zero(first_area, second_area)
  #         else
  #           if first_area < second_area
  #             # make_alias(first_area, second_area)
  #             @areas_dictionary[second_area] = first_area
  #             @data[col][row].area = first_area
  #           else
  #             @areas_dictionary[first_area] = second_area
  #             @data[col][row].area = second_area
  #           end
  #           # @data[col][row].area = first_area
  #           # @areas_dictionary[second_area] += [first_area]
  #         end
  #       end
  #     end
  #   end
  # end

    current_area = 1
    @areas_dictionary = {}
    (0...columns).each do |col|
      (0...rows).each do |row|
        if @data[col][row].filled?
          first_area = @data[(col - 1).positive_or_null][row].area
          second_area = @data[col][(row - 1).positive_or_null].area
          if (first_area == 0) && (second_area == 0)
            current_area += 1
            @data[col][row].area = current_area
            @areas_dictionary[current_area] = [[col, row]]
          elsif (first_area == second_area) || (first_area == 0) || (second_area == 0)
            @areas_dictionary[not_zero(first_area, second_area)] << [col, row]
            @data[col][row].area = not_zero(first_area, second_area)
          else
            @areas_dictionary[first_area] += @areas_dictionary[second_area]
            @areas_dictionary[second_area].each do |cell|
              @data[cell[0]][cell[1]].area = first_area
            end
            @areas_dictionary.delete second_area
            @areas_dictionary[first_area] << [col, row]
            @data[col][row].area = first_area
          end
        end
      end
    end
  end

  def form_domains_by_areas
    @domains = @areas_dictionary.map do |id, cells|
      Domain.new(id, cells, self)
    end
  end

  def print_chars
    @domains.each do |domain|
      puts "#{domain.id}| total_area: #{domain.total_area}, perimeter: #{domain.perimeter}, principal_axis_of_inertia: #{domain.principal_axis_of_inertia}"
    end
  end

  # def simplify_through_dictionary
  #   (0...columns).each do |col|
  #     (0...rows).each do |row|
  #       @data[col][row].area = @areas_dictionary[@data[col][row].area]
  #     end
  #   end
  # end

  def print_all
    @data.each do |row|
      row.each do |item|
        print " ", item.area
      end
      print "\n"
    end
  end

  private

  attr_accessor :areas_dictionary

  def not_zero(first, second)
    (first == 0) ? second : first
  end
end

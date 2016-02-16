class Domain
  attr_accessor :id
  attr_reader :grid
  def initialize(id, cells, grid)
    @id = id
    @cells = cells
    @grid = grid
  end

  def total_area
    @total_area ||= @cells.size
  end

  def center_of_mass_x
    @center_of_mass_x ||= @cells.inject(0) { |sum, n| sum + n[0] } / total_area
  end

  def center_of_mass_y
    @center_of_mass_н ||= @cells.inject(0) { |sum, n| sum + n[1] } / total_area
  end

  def perimeter
    @perimeter ||= @cells.select do |cell|
                     sum = 0
                     ((cell[0] - 1)..(cell[0] + 1)).each do |col|
                       ((cell[1] - 1)..(cell[1] + 1)).each do |row|
                        if !(0...grid.columns).include?(col) ||
                           !(0...grid.rows).include?(row) ||
                           (grid[col, row].area == 0)
                          
                          sum += 1 
                        end
                       end
                     end
                     sum > 1
                   end.size
  end

  def density
    @density ||= perimeter**2 / total_area
  end

  def central_point(i, j)
    @central_point ||= {}
    @central_point[[i, j]] ||= @cells.inject(0) { |sum, n| sum + ((n[0] - center_of_mass_x)**i * (n[1] - center_of_mass_y)**j) }
  end

  def elongation
    @elongation ||= (central_point(2, 0) + central_point(0, 2) + Math.sqrt((central_point(2, 0) - central_point(0, 2))**2 + (4 * central_point(1, 1)**2))) /
      (central_point(2, 0) + central_point(0, 2) - Math.sqrt((central_point(2, 0) - central_point(0, 2))**2 + (4 * central_point(1, 1)**2)))
  end

  def principal_axis_of_inertia
    return 0 if central_point(2, 0) - central_point(0, 2) == 0
    @principal_axis_of_inertia ||= 0.5 * Math.atan((2 * central_point(1, 1)) / (central_point(2, 0) - central_point(0, 2)))
  end
end

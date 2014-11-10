#!/usr/bin/env ruby
require 'matrix'
require 'pry'
require "../lib/iris_reader.rb"

LEARNING_SET_START = 0
LEARNING_SET_END = 79
CONTROL_SET_START = 80
CONTROL_SET_END = 139


class TreeNode
  attr_accessor :next_true, :next_false, :return_type, :treshold, :attr_name

  def initialize attr_name, treshold, params = {}
    @attr_name = attr_name
    @treshold = treshold
    @next_true = params[:next_true]
    @next_false = params[:next_false]
    @return_type = params[:type]
  end

  def predict element
    if is_leaf?
      @return_type
    else
      if element[@attr_name] <= @treshold
        @next_true.predict element
      else
        @next_false.predict element
      end
    end
  end

  def is_leaf?
    !@return_type.nil?
  end
end


class DesicionTree
  def initialize dataset
    @root = build_tree dataset
  end

  def classify value
    @root.predict value
  end

  def print_tree_to_html
    "<div class=\"tree\"> #{tree_to_html} </div>"
  end

  def percent_of_classified dataset
    i = 0.0
    dataset.each do |value|
      i += 1 if classify(value) == value[:type]
    end
    i / dataset.size
  end

  def print_all_values dataset
    printf "####################################################\n"
    dataset.each do |value|
      evaluated_type = classify(value)
      printf(" # %s # %15s # %5s # \n", value.values[0..3].to_s, value[:type], (evaluated_type == value[:type]) ? 'true' : "\033[1;31mfalse\033[0m")
    end
    printf "####################################################\n"
  end


  private

  def build_tree dataset
    best_split = { gain: 0 }
    dataset_entropy = entropy(dataset)

    dataset.each do |element|
      element.each do |attr_name, value|
        next if attr_name == :type

        match_values, not_match_value = split(dataset, attr_name, value)
        current_gain = count_new_gain(match_values, not_match_value, dataset_entropy)

        if current_gain >= best_split[:gain]
          best_split[:match] = match_values
          best_split[:not_match] = not_match_value
          best_split[:gain] = current_gain
          best_split[:attr_name] = attr_name
          best_split[:value] = value
          best_split[:type] = element[:type]
        end
      end
    end

    if best_split[:gain] == 0
      return TreeNode.new(best_split[:attr_name], best_split[:value], type: best_split[:type])
    end

    match = build_tree(best_split[:match])
    not_match = build_tree(best_split[:not_match])

    TreeNode.new(best_split[:attr_name], best_split[:value], next_true: match, next_false: not_match)
  end

  def count_new_gain match_values, not_match_values, dataset_entropy
    match_entropy = entropy(match_values)
    not_match_entropy = entropy(not_match_values)

    new_entropy = match_entropy * match_values.size
    new_entropy += not_match_entropy * not_match_values.size
    new_entropy /= (match_values.size + not_match_values.size)
    current_gain = dataset_entropy - new_entropy
  end

  def count_types dataset
    dataset.inject(Hash.new(0)) do |result, element|
      result[element[:type]] += 1
      result
    end
  end

  def entropy dataset
    result_entropy = 0
    count_types(dataset).each do |type, count|
      p = (1.0 * count) / dataset.size
      result_entropy -= p * Math.log2(p)
    end
    result_entropy
  end

  def split dataset, attr_name, value
    match = []
    not_match = []
    dataset.each do |element|
      if element[attr_name] <= value
        match << element
      else
        not_match << element
      end
    end
    return match, not_match
  end

  def tree_to_html tree = @root
    if (tree.return_type)
      return "<ul>
                <li>
                  <a>
                    <b> #{tree.return_type} </b>
                  </a>
                </li>
              </ul>"
    end
    "<ul>
      <li>
        <a>
          <b> #{tree.attr_name} <= #{tree.treshold}</b>
        </a>
        <ul>
          <li>
            <a>yes</a>
            #{tree_to_html(tree.next_true)}
          </li>
          <li>
            <a>no</a>
            #{tree_to_html(tree.next_false)}
          </li>
        </ul>
      </li>
    </ul>"
  end
end


data = DataSet.new :to_array
data.values.shuffle!
d = DesicionTree.new data.values[LEARNING_SET_START..LEARNING_SET_END]

open('index.html', 'w') do |f|
  f.puts "<link type=\"text/css\" rel=\"stylesheet\" href=\"style.css\">"
  f.puts d.print_tree_to_html
end

d.print_all_values(data.values[LEARNING_SET_START..CONTROL_SET_END])
puts("Size of learning set: #{LEARNING_SET_END - LEARNING_SET_START + 1}")
puts("Size of control set: #{CONTROL_SET_END - CONTROL_SET_START + 1}")
printf "\n"
printf(" - Recognotion ability: %d%\n", d.percent_of_classified(data.values[LEARNING_SET_START..LEARNING_SET_END]) * 100)
printf(" - Generalization ability: %d%\n", d.percent_of_classified(data.values[CONTROL_SET_START..CONTROL_SET_END]) * 100)

#!/usr/bin/env ruby
require 'matrix'
require 'pry'
require "../lib/iris_reader.rb"


class TreeNode
  def initialize attr_name, treshold, params = {}
    @attr_name = attr_name
    @treshold = treshold
    @next_true = params[:next_true]
    @next_false = params[:next_false]
    @return_type = params[:return_type]
  end

  def predict element
    if @return_type
      @return_type
    else
      if element[attr_name] <= treshold
        next_true.predict element
      else
        next_false.predict element
      end
    end
  end
end


class DesicionTree
  def initialize dataset
    @root = build_tree dataset
  end

  def build_tree dataset
    best_split = {gain: 0}
    common_entropy = entropy(dataset, :type)
    dataset.each do |element|
      element.each do |attr_name, value|
        next if attr_name == :type
        match_values, not_match_value = split(dataset, attr_name, value)

        new_entropy = count_new_entropy(match_values, not_match_value)
        current_gain = common_entropy - new_entropy

        if current_gain > best_split[:gain]
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
      return TreeNode.new(best_split[:attr_name], best_split[:value], return_type: best_split[:type])
    end

    match = build_tree(best_split[:match])
    not_match = build_tree(best_split[:not_match])

    TreeNode.new(best_split[:attr_name], best_split[:value], next_true: match, next_false: not_match)
  end

  def count_new_entropy match_values, not_match_value
    0
  end

  def entropy dataset, attr_name
    0
  end

  def split dataset, attr_name, value

  end
end


data = DataSet.new :to_array
d = DesicionTree.new data.values

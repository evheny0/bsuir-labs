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
    @return_type = params[:type]
  end

  def predict element
    if @return_type
      @return_type
    else
      if element[@attr_name] <= @treshold
        @next_true.predict element
      else
        @next_false.predict element
      end
    end
  end
end


class DesicionTree
  def initialize dataset
    @root = build_tree dataset
  end

  def classify value
    @root.predict value
  end

  private

  def build_tree dataset
    best_split = {gain: 0}
    dataset_entropy = entropy(dataset)
    dataset.each do |element|
      element.each do |attr_name, value|
        next if attr_name == :type
        match_values, not_match_value = split(dataset, attr_name, value)

        new_entropy = count_new_entropy(match_values, not_match_value)
        current_gain = dataset_entropy - new_entropy

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

  def count_new_entropy match_values, not_match_values
    match_entropy = entropy(match_values)
    not_match_entropy = entropy(not_match_values)

    new_entropy = match_entropy * match_values.size
    new_entropy += not_match_entropy * not_match_values.size
    new_entropy /= (match_values.size + not_match_values.size)
    new_entropy
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
end


data = DataSet.new :to_array
d = DesicionTree.new data.values

s = data.values.sample
p s
p d.classify(s)

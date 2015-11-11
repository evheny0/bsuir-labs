require 'singleton'
require 'logger'



class LabLogger
  include Singleton
  attr_reader :logger

  def self.info(arg)
    logger.info(arg)
  end

  def self.logger
    @logger ||= Logger.new('./log.txt')
  end
end

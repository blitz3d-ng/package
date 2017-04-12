require 'yaml'

module Blitz3D
  class Module
    attr_accessor :id, :name, :description, :dependencies, :commands, :path, :premake5

    def self.all
      Dir.glob('src/runtime/bb/*/module.yml').map { |path| new(path) }
    end

    def initialize(config_file)
      begin
        config = YAML.load(File.open(config_file).read)
      rescue Psych::SyntaxError
        puts "YAML syntax error in #{config_file}".red
        exit 1
      end

      @path = File.dirname(config_file)
      @id = File.basename(@path)
      @name = config['name']
      @description = config['description']
      @dependencies = config['dependencies']
      @commands = (config['commands'] || []).map { |text| Command.new(self, text) }.sort_by(&:name)

      @premake5 = config['premake5'] || {}
    end

    def help_dir
      File.join(path, 'docs')
    end

    def examples_dir
      File.join(help_dir, 'examples')
    end
  end
end

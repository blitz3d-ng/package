require 'yaml'

module Blitz3D
  class Runtime
    attr_accessor :id, :name, :modules, :premake5

    def self.all
      Dir.glob('src/runtime/*.yml').map { |path| new(path) }
    end

    def self.find(id)
      all.find { |mod| mod.id == id }
    end

    def initialize(config_file)
      begin
        config = YAML.load(File.open(config_file).read)
      rescue Psych::SyntaxError
        puts "YAML syntax error in #{config_file}".red
        exit 1
      end

      @id = File.basename(config_file, '.yml')
      @name = config['name']
      @modules = config['modules'] || []
      @premake5 = config['premake5'] || {}
    end

    def dependencies
      Module.find(modules.first).complete_dependencies
    end
  end
end

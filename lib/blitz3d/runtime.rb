require 'yaml'

module Blitz3D
  class Runtime
    attr_accessor :id, :name, :platforms, :modules, :entry, :premake5

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

      @platforms = config['platforms'] || []
      @platforms += %w(win32 win64 mingw32) if @platforms.delete('windows')
      @platforms = Module::PLATFORMS.dup if @platforms.empty?

      @entry = config['entry'] || {}
      @entry['runtime'] ||= 'bbCreateUnspecifiedRuntime'

      @premake5 = config['premake5'] || {}
    end

    def dependencies(view = :raw, platform = nil)
      view ||= :raw

      case view
      when :raw
        modules.map { |id| Module.find(id) || raise(StandardError, "Cannot find module '#{id}'.") }.select { |mod| mod.supports?(platform) }
      when :tree
        deps = dependencies(:raw, platform)
        valid_deps = deps.reject { |dep| deps.map { |td| td.has_dependency?(dep, platform) }.any? }
        valid_deps.inject({}) do |hsh, mod|
          hsh[mod] = mod.dependencies(:raw, platform)
          hsh
        end
      when :list
        dependencies(:tree, platform).keys.map do |dep|
          [dep.dependencies(:list, platform), dep]
        end.flatten.uniq
      end
    end
  end
end

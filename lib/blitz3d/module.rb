require 'yaml'

module Blitz3D
  class Module
    PLATFORMS = %w(win32 win64 mingw32 macos linux).freeze

    attr_accessor :id, :name, :description, :platforms, :symbols, :commands, :path, :premake5

    def self.all
      @@store ||= {}
      Dir.glob('src/runtime/bb/*/module.yml').map { |path| @@store[path] ||= new(path) }
      @@store.values
    end

    def self.find(id)
      @@store ||= {}
      @@store.values.find { |mod| mod.id == id } || all.find { |mod| mod.id == id }
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

      @platforms = config['platforms'] || []
      @platforms += %w(win32 win64 mingw32) if @platforms.delete('windows')
      @platforms = Module::PLATFORMS.dup if @platforms.empty?

      @dependencies = [config['dependencies']].flatten.compact
      @symbols = (config['symbols'] || []).inject({}) do |syms, line|
        ident, sym = line.split(':')
        syms[ident] = sym
        syms
      end
      @commands = (config['commands'] || []).map { |text| Command.new(self, text) }.sort_by(&:name)

      @premake5 = config['premake5'] || {}
    end

    def ==(other)
      other.is_a?(self.class) && id == other.id
    end

    def supports?(platform)
      platform.nil? || platforms.include?(platform)
    end

    def has_dependency?(other, platform = nil)
      return false if other == self

      deps = dependencies(:tree, platform)
      return true if deps[other]

      deps.inject([]) do |arr, (mod, deps)|
        arr << mod.has_dependency?(other, platform)
      end.any?
    end

    def dependencies(view = :raw, platform = nil)
      view ||= :raw

      case view
      when :raw
        @dependencies.map { |id| Module.find(id) }.select { |mod| mod.supports?(platform) }
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

    def help_dir
      File.join(path, 'docs')
    end

    def examples_dir
      File.join(help_dir, 'examples')
    end

    def inspect
      "#<Blitz3D::Module @id=\"#{id}\">"
    end
  end
end

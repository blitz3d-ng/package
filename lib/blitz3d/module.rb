require 'yaml'

module Blitz3D
  class Module
    PLATFORMS = %w(win32 win64 mingw32 macos linux ios android emscripten nx).freeze

    METAPLATFORMS = {
      desktop: %w(win32 win64 macos linux),
      windows: %w(win32 win64 mingw32),
      mobile: %w(ios android),
      web: %w(emscripten),
    }

    attr_accessor :id, :name, :description, :platforms, :symbols, :commands, :path, :make, :premake5

    def self.all
      @@store ||= {}
      Dir.glob('src/modules/bb/*/module.yml').map { |path| @@store[path] ||= new(path) }
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

      platforms = config['platforms'] || []
      if platforms.empty?
        @platforms = Module::PLATFORMS.dup
      else
        @platforms = platforms.map { |plat| METAPLATFORMS[plat.to_sym] || plat }.flatten.uniq
      end

      @dependencies = [config['dependencies']].flatten.compact
      @symbols = (config['symbols'] || []).inject({}) do |syms, line|
        ident, sym = line.split(':')
        syms[ident] = sym
        syms
      end
      @commands = (config['commands'] || []).map { |text| Command.new(self, text) }.sort_by(&:name)

      @make = config['make'] || {}
      @make['files'] ||= []
      @make['include_directories'] ||= []
      @make['definitions'] ||= []

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

    def libraries(platform)
      current_platforms = PLATFORMS
      [premake5].flatten.map do |i|
        i.map do |(key, value)|
          if key == 'links' && current_platforms.include?(platform)
            value
          elsif key == 'filter'
            _, platforms = value.match(/platforms:(.*)/).to_a
            current_platforms = platforms.split(/\s+/).select { |p| PLATFORMS.include?(p) }
            nil
          end
        end
      end.flatten.compact
    end

    def needs_to_link?
      symbols.size > 0 || commands.size > 0
    end

    def public_symbols?
      needs_to_link? && commands.any? { |command| command.name !~ /^_/ }
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

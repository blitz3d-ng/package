module Blitz3D
  module Help
    module Reference
      class Module < Template
        attr_reader :modules, :mod

        def initialize(modules, mod)
          @modules = modules
          @mod = mod
        end

        def input_path
          'src/docs/commands/_module.html.erb'
        end

        def output_path
          File.join(root_path, 'modules', mod.id, 'index.html')
        end

        def module_path(mod)
          File.dirname(Module.new([], mod).output_path)
        end

        def command_path(command)
          Command.new([], command).output_path
        end

        def generate
          template = File.read(layout_path)
          out = render(template) { render(File.open(input_path).read) }
          write HtmlBeautifier.beautify(out)
        end
      end
    end
  end
end

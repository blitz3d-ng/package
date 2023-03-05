module Blitz3D
  module Help
    module Reference
      class Command < Template
        attr_accessor :modules, :command

        def initialize(modules, command)
          @modules = modules
          @command = command
        end

        def module_path(mod)
          File.dirname(Module.new([], mod).output_path)
        end

        def command_path(command)
          Command.new([], command).output_path
        end

        def output_path
          File.join(module_path(command.mod), [command.name, 'html'].join('.'))
        end

        def generate(source)
          template = File.read(layout_path)
          out = render(template) { source }

          FileUtils.mkdir_p(File.dirname(output_path))
          File.open(output_path, 'w') { |f| f.write out }
        end
      end
    end
  end
end

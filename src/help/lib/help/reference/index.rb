module Blitz3D
  module Help
    module Reference
      class Index < Template
        def input_path
          File.expand_path('../../../views/commands/_index.html.erb', __dir__)
        end

        def output_path
          File.join(root_path, 'index.html')
        end

        def module_path(mod)
          File.dirname(Module.new([], mod).output_path)
        end

        def command_path(command)
          Command.new([], command).output_path
        end

        def generate
          template = File.read(layout_path)
          out = render(template) { File.open(input_path).read }
          write out
        end
      end
    end
  end
end

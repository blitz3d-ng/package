module Blitz3D
  module Help
    module Reference
      class Ascii < Template
        def input_path
          File.expand_path('../../../views/commands/_ascii.html.erb', __dir__)
        end

        def output_path
          File.join(root_path, 'ascii.html')
        end

        def generate
          template = File.read(layout_path)
          out = render(template) { render(File.open(input_path).read) }
          write out
        end
      end
    end
  end
end

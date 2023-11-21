module Blitz3D
  module Help
    module Reference
      class ScanCodes < Template
        def input_path
          File.expand_path('../../../views/commands/_scancodes.html.erb', __dir__)
        end

        def output_path
          File.join(root_path, 'scancodes.html')
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

module Blitz3D
  module Help
    module Reference
      class Template < ::Template
        def root_path
          File.expand_path('../../../../../_release/help/reference', __dir__)
        end

        def layout_path
          File.expand_path('../../../views/commands/_layout.html.erb', __dir__)
        end

        def write(html)
          FileUtils.mkdir_p(File.dirname(output_path))
          File.open(output_path, 'w') { |f| f.write compress(html) }
        end
      end
    end
  end
end

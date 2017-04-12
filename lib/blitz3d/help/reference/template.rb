module Blitz3D
  module Help
    module Reference
      class Template < ::Template
        def root_path
          '_release/help/reference'
        end

        def layout_path
          'src/docs/commands/_layout.html.erb'
        end

        def relative_to_root
          relative_to(root_path)
        end

        def relative_to(path)
          output_parts = output_path.split('/')
          path_parts = path.split('/')
          while output_parts.first == path_parts.first && !output_parts.empty? && !path_parts.empty?
            output_parts.shift
            path_parts.shift
          end
          path = [Array.new(output_parts.size - 1) { '..' }, path_parts].flatten.join('/')
          path.present? ? path : '.'
          #throw [output_parts, path_parts, output_path, path]
        end

        def write(html)
          FileUtils.mkdir_p(File.dirname(output_path))
          File.open(output_path, 'w') { |f| f.write html }
        end
      end
    end
  end
end

module Blitz3D
  module Help
    class Index < Template
      def self.path
        'src/docs/_index.html.erb'
      end

      def generate(path)
        template = File.read(self.class.path)
        out = render(template) { source }
        out = HtmlBeautifier.beautify(out)

        File.open(path, 'w') { |f| f.write out }
      end
    end
  end
end

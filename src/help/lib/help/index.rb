module Blitz3D
  module Help
    class Index < Template
      def self.path
        File.expand_path('../../views/_index.html.erb', __dir__)
      end
    end
  end
end

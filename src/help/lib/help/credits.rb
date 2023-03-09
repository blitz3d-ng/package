module Blitz3D
  module Help
    class Credits < Template
      def self.path
        File.expand_path('../../views/_credits.html.erb', __dir__)
      end
    end
  end
end

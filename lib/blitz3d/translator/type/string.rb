module Blitz3D
  module AST
    class StringType
      def initialize(json)
      end

      def to_c
        'BBStr*'
      end
    end
  end
end

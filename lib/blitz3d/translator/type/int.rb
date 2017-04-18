module Blitz3D
  module AST
    class IntType
      def initialize(json)
      end

      def to_c
        'bb_int_t'
      end
    end
  end
end

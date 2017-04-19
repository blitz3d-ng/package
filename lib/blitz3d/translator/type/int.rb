module Blitz3D
  module AST
    class IntType < Type
      def initialize(json = {})
      end

      def to_c
        'bb_int_t'
      end
    end
  end
end

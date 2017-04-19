module Blitz3D
  module AST
    class FloatType < Type
      def initialize(json)
      end

      def to_c
        'bb_float_t'
      end
    end
  end
end

module Blitz3D
  module AST
    class FloatType < Type
      def initialize(json)
      end

      def to_c
        'bb_float_t'
      end

      def ptr
        '&_bbFltType'
      end

      def default_value
        '0.0f'
      end
    end
  end
end

module Blitz3D
  module AST
    class IntType < Type
      def initialize(json = {})
      end

      def to_c
        'bb_int_t'
      end

      def ptr
        '&_bbIntType'
      end

      def default_value
        0
      end

      def kind
        'BBTYPE_INT'
      end
    end
  end
end

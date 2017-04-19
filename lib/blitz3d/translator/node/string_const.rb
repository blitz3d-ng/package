module Blitz3D
  module AST
    class StringConstNode < ConstNode
      attr_accessor :value

      def to_c
        "_bbStrConst(#{value.inspect})"
      end
    end
  end
end

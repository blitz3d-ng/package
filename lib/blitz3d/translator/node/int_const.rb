module Blitz3D
  module AST
    class IntConstNode < ConstNode
      attr_accessor :value

      def to_c
        value
      end
    end
  end
end

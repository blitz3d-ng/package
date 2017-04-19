module Blitz3D
  module AST
    class FloatConstNode < ConstNode
      attr_accessor :value

      def to_c
        "#{value}f"
      end
    end
  end
end

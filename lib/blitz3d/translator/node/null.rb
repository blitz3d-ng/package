module Blitz3D
  module AST
    class NullNode < ExprNode
      def initialize(json)
        @sem_type = NullType.new
      end

      def to_c
        '0'
      end
    end
  end
end

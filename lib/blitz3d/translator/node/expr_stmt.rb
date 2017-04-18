module Blitz3D
  module AST
    class ExprStmtNode < Node
      attr_accessor :expr

      def initialize(json)
        @expr = Node.load(json['expr'])
      end

      def to_c
        expr.to_c
      end
    end
  end
end

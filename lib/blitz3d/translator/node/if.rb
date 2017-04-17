module Blitz3D
  module AST
    class IfNode
      attr_accessor :expr, :stmts, :else_opt

      def initialize(json)
        @expr = Node.load(json['expr'])
        @stmts = Node.load(json['stmts'])
        @else_opt = Node.load(json['elseOpt'])
      end

      def to_c
        "if( #{expr.to_c} ){\n#{stmts.to_c}}\n"
      end
    end
  end
end

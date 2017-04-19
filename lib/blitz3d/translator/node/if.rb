module Blitz3D
  module AST
    class IfNode < StmtNode
      attr_accessor :expr, :stmts, :else_opt

      def initialize(json)
        super
        @expr = Node.load(json['expr'])
        @stmts = Node.load(json['stmts'])
        @else_opt = Node.load(json['elseOpt'])
      end

      def to_c
        "if( #{expr.to_c} ){\n  #{stmts.to_c.split("\n").join("\n  ")}\n}"
      end
    end
  end
end

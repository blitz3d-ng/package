module Blitz3D
  module AST
    class IfNode < StmtNode
      attr_accessor :expr, :stmts, :else_opt

      def initialize(json)
        super
        @expr = Node.load(json['expr'])
        @stmts = Node.load(json['stmts'])
        @else_opt = Node.load(json['elseOpt']) if json['elseOpt']
      end

      def to_c(&cleanup)
        else_opt = "else{\n  #{self.else_opt.to_c(&cleanup).indent}\n}" if self.else_opt
        "if( #{expr.to_c} ){\n  #{stmts.to_c(&cleanup).split("\n").join("\n  ")}\n}#{else_opt}"
      end
    end
  end
end

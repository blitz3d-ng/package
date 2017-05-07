module Blitz3D
  module AST
    class ForNode < StmtNode
      attr_accessor :next_pos, :var, :from_expr, :to_expr, :step_expr, :stmts, :sem_brk

      def initialize(json)
        super
        @next_pos = json['nextPos']
        @var = Node.load(json['var'])
        @from_expr = Node.load(json['fromExpr'])
        @to_expr = Node.load(json['toExpr'])
        @step_expr = Node.load(json['stepExpr'])
        @stmts = Node.load(json['stmts'])
        @sem_brk = json['sem_brk']
      end

      def to_c(&cleanup)
        "for( #{var.to_c}=#{from_expr.to_c};#{var.to_c}#{step_expr.value > 0 ? '<=' : '>='}#{to_expr.to_c};#{var.to_c}+=#{step_expr.to_c} ){\n  #{stmts.to_c(&cleanup).indent}\n}"
      end
    end
  end
end

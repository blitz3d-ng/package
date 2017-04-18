module Blitz3D
  module AST
    class ForNode < Node
      attr_accessor :next_pos, :var, :from_expr, :to_expr, :step_expr, :stmts, :sem_brk

      def initialize(json)
        @next_pos = json['nextPos']
        @var = Node.load(json['var'])
        @from_expr = Node.load(json['fromExpr'])
        @to_expr = Node.load(json['toExpr'])
        @step_expr = Node.load(json['stepExpr'])
        @stmts = Node.load(json['stmts'])
        @sem_brk = json['sem_brk']
      end
      # for i = 0 to 3 step 3
      def to_c
        "for( #{var.to_c}=#{from_expr.to_c};#{var.to_c}<#{to_expr.to_c};#{var.to_c}+=#{step_expr.to_c} ){  #{stmts.to_c.indent}\n}"
      end
    end
  end
end

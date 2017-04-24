module Blitz3D
  module AST
    class ForEachNode < StmtNode
      attr_accessor :next_pos, :var, :from_expr, :to_expr, :step_expr, :stmts, :sem_brk

      def initialize(json)
        super
        @next_pos = json['nextPos']
        @var = Node.load(json['var'])
        @typeIdent = json['typeIdent']
        @stmts = Node.load(json['stmts'])
        @sem_brk = json['sem_brk']
      end

      def to_c
        "for( #{'<ForEachNode>'.red} ){  #{stmts.to_c.indent}\n}"
      end
    end
  end
end

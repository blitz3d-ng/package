module Blitz3D
  module AST
    class SelectNode < StmtNode
      attr_accessor :expr, :def_stmts, :cases, :sem_temp

      def initialize(json)
        super
        @expr = Node.load(json['expr'])
        @def_stmts = Node.load(json['defStmts']) if json['defStmts']
        @cases = json['cases'].map { |cse| Node.load(cse) }
        @sem_temp = Node.load(json['sem_temp'])
      end

      def to_c
        clauses = cases.map do |cse|
          cse.to_c(expr)
        end

        unless def_stmts.nil?
          clauses << "{ // default\n  #{def_stmts.to_c.indent}\n}"
        end

        clauses.join(' else ')
      end
    end
  end
end

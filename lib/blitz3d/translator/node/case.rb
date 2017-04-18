module Blitz3D
  module AST
    class CaseNode < Node
      attr_accessor :exprs, :stmts

      def initialize(json)
        @exprs = json['exprs'].map { |expr| Node.load(expr) }
        @stmts = Node.load(json['stmts'])
      end

      def to_c(cond)
        exprs = self.exprs.map do |expr|
          RelExprNode.new('op' => 'EQ', 'lhs' => cond, 'rhs' => expr).to_c
        end
        "if(#{exprs.join(' || ')}){\n  #{stmts.to_c.indent}\n}"
      end
    end
  end
end

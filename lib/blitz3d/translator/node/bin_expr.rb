module Blitz3D
  module AST
    class BinExprNode < ExprNode
      attr_accessor :op, :lhs, :rhs

      def initialize(json)
        super
        @op = json['op']
        @lhs = Node.load(json['lhs'])
        @rhs = Node.load(json['rhs'])
      end

      def to_c
        cop = { 'AND' => '&&', 'OR' => '||', 'XOR' => '^', 'SHL' => '<<', 'SHR' => '>>' }
        "(#{lhs.to_c} #{cop[op]} #{rhs.to_c})"
      end
    end
  end
end

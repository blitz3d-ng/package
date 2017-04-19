module Blitz3D
  module AST
    class RelExprNode < ExprNode
      attr_accessor :op, :lhs, :rhs, :opType

      def initialize(json)
        super
        @op = json['op']
        @lhs = Node.load(json['lhs'])
        @rhs = Node.load(json['rhs'])
        @opType = json['opType']
      end

      def to_c
        cop = { 'LT' => '<', 'EQ' => '==', 'GT' => '>', 'LE' => '<=', 'NE' => '!=', 'GE' => '>=' }
        "#{lhs.to_c} #{cop[op]} #{rhs.to_c}"
      end
    end
  end
end

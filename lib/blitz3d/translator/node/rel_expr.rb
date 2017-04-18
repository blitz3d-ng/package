module Blitz3D
  module AST
    class RelExprNode < Node
      attr_accessor :op, :lhs, :rhs, :opType

      def initialize(json)
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

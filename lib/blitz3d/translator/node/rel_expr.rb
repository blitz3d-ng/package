module Blitz3D
  module AST
    class RelExprNode < ExprNode
      attr_accessor :op, :lhs, :rhs, :op_type

      def initialize(json)
        super
        @op = json['op']
        @lhs = Node.load(json['lhs'])
        @rhs = Node.load(json['rhs'])
        @op_type = json['opType']
      end

      def to_c
        cop = { 'LT' => '<', 'EQ' => '==', 'GT' => '>', 'LE' => '<=', 'NE' => '!=', 'GE' => '>=' }
        if op_type.is_a?(StringType) && op == 'EQ'
          "_bbStrCompare( #{lhs.to_c},#{rhs.to_c} )"
        else
          "#{lhs.to_c} #{cop[op]} #{rhs.to_c}"
        end
      end
    end
  end
end

module Blitz3D
  module AST
    class ArithExprNode < Node
      attr_accessor :op, :lhs, :rhs

      def initialize(json)
        @op = json['op']
        @lhs = Node.load(json['lhs'])
        @rhs = Node.load(json['rhs'])
      end

      def to_c
        case op
        when 'SQR'
          "sqr(#{lhs.to_c}, #{rhs.to_c})"
        else
          cop = { 'ADD' => '+', 'SUB' => '-', 'MUL' => '*', 'DIV' => '/', 'MOD' => '%' }
          "#{lhs.to_c} #{cop[op]} #{rhs.to_c}"
        end
      end
    end
  end
end

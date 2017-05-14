module Blitz3D
  module AST
    class UniExprNode < ExprNode
      attr_accessor :op, :expr

      def initialize(json)
        super
        @op = json['op']
        @expr = Node.load(json['expr'])
      end

      def to_c
        case op
        when 'SUB'
          "-#{expr.to_c}"
        when 'ADD'
          "+#{expr.to_c}"
        when 'ABS'
          "_bbAbs(#{expr.to_c})"
        when 'SGN'
          "_bbSgn(#{expr.to_c})"
        else
          throw "invalid uni_expr op: #{op}"
        end
      end
    end
  end
end

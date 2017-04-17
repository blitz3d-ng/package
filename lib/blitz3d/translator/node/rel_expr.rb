module Blitz3D
  module AST
    class RelExprNode
      attr_accessor :op, :lhs, :rhs, :opType

      def initialize(json)
        @op = json['op']
        @lhs = Node.load(json['lhs'])
        @rhs = Node.load(json['rhs'])
        @opType = json['opType']
      end

      def to_c
        "#{lhs.to_c} #{op.chr} #{rhs.to_c}"
      end
    end
  end
end

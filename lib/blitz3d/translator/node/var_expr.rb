module Blitz3D
  module AST
    class VarExprNode < ExprNode
      attr_accessor :var

      def initialize(json)
        super
        @var = Node.load(json['var'])
      end

      def to_c
        var.to_c
      end
    end
  end
end

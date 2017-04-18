module Blitz3D
  module AST
    class VarExprNode < Node
      attr_accessor :var

      def initialize(json)
        @var = Node.load(json['var'])
      end

      def to_c
        var.to_c
      end
    end
  end
end

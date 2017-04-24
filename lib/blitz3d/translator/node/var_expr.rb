module Blitz3D
  module AST
    class VarExprNode < ExprNode
      attr_accessor :var

      def initialize(json)
        super
        @var = Node.load(json['var'])
      end

      def to_c
        if var.sem_type.is_a?(StringType)
          "_bbStrCopy(#{var.to_c})"
        else
          var.to_c
        end
      end
    end
  end
end

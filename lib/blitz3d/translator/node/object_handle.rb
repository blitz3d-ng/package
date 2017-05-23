module Blitz3D
  module AST
    class ObjectHandleNode < ExprNode
      attr_accessor :expr

      def initialize(json)
        super
        @expr = Node.load(json['expr'])
      end

      def to_c
        "_bbObjToHandle( #{expr.to_c} )"
      end
    end
  end
end

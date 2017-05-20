module Blitz3D
  module AST
    class AfterNode < ExprNode
      attr_accessor :expr

      def initialize(json)
        super
        @expr = Node.load(json['expr'])
      end

      def to_c
        "_bbObjNext( #{expr.to_c} )"
      end
    end
  end
end

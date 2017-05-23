module Blitz3D
  module AST
    class ObjectCastNode < ExprNode
      attr_accessor :expr, :type_ident

      def initialize(json)
        super
        @expr = Node.load(json['expr'])
        @type_ident = json['type_ident']
      end

      def to_c
        "_bbObjFromHandle( #{expr.to_c},&_t#{type_ident}.type )"
      end
    end
  end
end

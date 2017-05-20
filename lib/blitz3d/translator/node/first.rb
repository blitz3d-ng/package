module Blitz3D
  module AST
    class FirstNode < ExprNode
      attr_accessor :ident, :type

      def initialize(json)
        super
        @ident = json['ident']
        @type = StructType.find(ident)
      end

      def to_c
        "_bbObjFirst( &#{@type.to_type}.type )"
      end
    end
  end
end

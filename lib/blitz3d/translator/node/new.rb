module Blitz3D
  module AST
    class NewNode < ExprNode
      attr_accessor :ident

      def initialize(json)
        super
        @ident = json['ident']
      end

      def to_c
        "_bbObjNew( &#{sem_type.to_type}.type )"
      end
    end
  end
end

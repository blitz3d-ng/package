module Blitz3D
  module AST
    class DeleteNode < StmtNode
      attr_accessor :expr

      def initialize(json)
        super
        @expr = Node.load(json['expr'])
      end

      def to_c
        "_bbObjDelete( #{expr.to_c} )"
      end
    end
  end
end

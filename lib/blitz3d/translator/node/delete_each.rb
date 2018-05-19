module Blitz3D
  module AST
    class DeleteEachNode < StmtNode
      attr_accessor :type_ident

      def initialize(json)
        super
        @type_ident = json['typeIdent']
      end

      def to_c
        "_bbObjDeleteEach( &_t#{type_ident}.type )"
      end
    end
  end
end

module Blitz3D
  module AST
    class GosubNode < StmtNode
      attr_accessor :ident

      def initialize(json)
        super
        @ident = json['ident']
      end
    end
  end
end

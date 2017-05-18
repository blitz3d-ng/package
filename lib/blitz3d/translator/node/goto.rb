module Blitz3D
  module AST
    class GotoNode < StmtNode
      attr_accessor :ident

      def initialize(json)
        super
        @ident = json['ident']
      end

      def to_c(&cleanup)
        "goto #{ident}"
      end
    end
  end
end

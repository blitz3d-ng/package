module Blitz3D
  module AST
    class IdentVarNode
      attr_accessor :ident, :tag

      def initialize(json)
        @ident = json['ident']
        @tag = json['tag']
      end

      def to_c
        ident
      end
    end
  end
end

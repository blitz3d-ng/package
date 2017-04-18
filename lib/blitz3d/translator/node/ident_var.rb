module Blitz3D
  module AST
    class IdentVarNode < Node
      attr_accessor :ident, :tag

      def initialize(json)
        @ident = json['ident']
        @tag = json['tag']
      end

      def to_h
        "#{IntType.new.to_c} #{ident}"
      end

      def to_c
        ident
      end
    end
  end
end

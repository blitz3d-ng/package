module Blitz3D
  module AST
    class StmtNode < Node
      attr_accessor :pos

      def initialize(json)
        @pos = json['pos']
      end

      def row
        (pos >> 16) & 0xffff
      end

      def col
        pos & 0xffff
      end
    end
  end
end

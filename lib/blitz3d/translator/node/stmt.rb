module Blitz3D
  module AST
    class StmtNode < Node
      attr_accessor :pos

      def initialize(json)
        @pos = json['pos']
      end
    end
  end
end

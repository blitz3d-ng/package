module Blitz3D
  module AST
    class DeclNode < Node
      attr_accessor :pos, :file

      def initialize(json)
        @pos = json['pos']
        @file = json['file']
      end
    end
  end
end

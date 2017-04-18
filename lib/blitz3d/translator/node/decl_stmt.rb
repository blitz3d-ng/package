module Blitz3D
  module AST
    class DeclStmtNode < Node
      attr_accessor :decl

      def initialize(json)
        # STDERR.puts JSON.pretty_generate(json).green
        @decl = Node.load(json['decl'])
      end

      def to_c
        decl.to_c
      end
    end
  end
end

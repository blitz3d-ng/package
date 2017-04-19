module Blitz3D
  module AST
    class DeclStmtNode < StmtNode
      attr_accessor :decl

      def initialize(json)
        super
        # STDERR.puts JSON.pretty_generate(json).green
        @decl = Node.load(json['decl'])
      end

      def to_c
        decl.to_c
      end
    end
  end
end

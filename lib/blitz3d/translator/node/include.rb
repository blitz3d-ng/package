module Blitz3D
  module AST
    class IncludeNode < StmtNode
      attr_accessor :file, :stmts

      def initialize(json)
        super
        @file = json['file']
        @stmts = Node.load(json['stmts'])
      end

      def to_c
        stmts.to_c
      end
    end
  end
end

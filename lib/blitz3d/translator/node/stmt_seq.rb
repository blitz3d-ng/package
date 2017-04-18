module Blitz3D
  module AST
    class StmtSeqNode < Node
      attr_accessor :file, :stmts

      def initialize(json)
        @file = json['file']
        @stmts = json['stmts'].map { |stmt| Node.load(stmt) }
      end

      def to_c
        f = StringIO.new

        f.write "// #{file}\n"
        f.write @stmts.map(&:to_c).map { |s| s.last != '}' ? "#{s};" : s }.join("
        \n")

        f.string
      end
    end
  end
end

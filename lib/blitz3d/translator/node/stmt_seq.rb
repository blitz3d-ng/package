module Blitz3D
  module AST
    class StmtSeqNode < Node
      attr_accessor :file, :stmts

      def initialize(json)
        @file = json['file']
        @stmts = json['stmts'].map { |stmt| Node.load(stmt) }
      end

      def to_c(&cleanup)
        f = StringIO.new

        f.write(@stmts.map do |stmt|
          code = stmt.to_c(&cleanup)
          code = (code.last != '}' && code.last != ';') ? "#{code};" : code
          "\n_bbDebugStmt(#{stmt.pos},#{file.inspect});\n#{code}"
          # "#{code}\n"
        end.join(""))

        f.string
      end
    end
  end
end

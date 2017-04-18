module Blitz3D
  module AST
    class StmtSeqNode
      attr_accessor :file, :stmts

      def initialize(json)
        @file = json['file']
        @stmts = json['stmts'].map { |stmt| Node.load(stmt) }
      end

      def to_c
        f = StringIO.new

        f.write "// #{file}\n"
        @stmts.each do |stmt|
          f.write "#{stmt.to_c};\n"
        end

        f.string
      end
    end
  end
end

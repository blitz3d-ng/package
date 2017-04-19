module Blitz3D
  module AST
    class WhileNode < StmtNode
      attr_accessor :wend_pos, :expr, :stmts, :sem_brk

      def initialize(json)
        super
        @wend_pos = json['wendPos']
        @expr = Node.load(json['expr'])
        @stmts = Node.load(json['stmts'])
        @sem_brk = json['sem_brk']
      end

      def to_c
        "while(#{expr.to_c}){\n  #{stmts.to_c.indent}\n}"
      end
    end
  end
end

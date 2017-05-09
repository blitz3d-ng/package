module Blitz3D
  module AST
    class RepeatNode < StmtNode
      attr_accessor :wend_pos, :expr, :stmts, :sem_brk

      def initialize(json)
        super
        @until_post = json['untilPos']
        @stmts = Node.load(json['stmts'])
        @expr = Node.load(json['expr']) if json['expr']
        @sem_brk = json['sem_brk']
      end

      def to_c(&cleanup)
        "do{\n  #{stmts.to_c(&cleanup).indent}\n}while( !#{expr.try(:to_c) || 1} )"
      end
    end
  end
end

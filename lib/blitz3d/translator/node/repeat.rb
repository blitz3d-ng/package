module Blitz3D
  module AST
    class RepeatNode < Node
      attr_accessor :wend_pos, :expr, :stmts, :sem_brk

      def initialize(json)
        @until_post = json['untilPos']
        @stmts = Node.load(json['stmts'])
        @expr = Node.load(json['expr']) if json['expr']
        @sem_brk = json['sem_brk']
      end

      def to_c
        "do{\n  #{stmts.to_c.indent}\nwhile( #{expr.try(:to_c) || true} )"
      end
    end
  end
end

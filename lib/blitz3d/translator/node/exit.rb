module Blitz3D
  module AST
    class ExitNode < StmtNode
      attr_accessor :sem_brk

      def initialize(json)
        super
        @sem_brk = json['sem_brk']
      end

      def to_c
        'break'
      end
    end
  end
end

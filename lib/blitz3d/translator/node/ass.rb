module Blitz3D
  module AST
    class AssNode < StmtNode
      attr_accessor :var, :expr

      def initialize(json)
        super
        @var = Node.load json['var']
        @expr = Node.load json['expr']
      end

      def to_c
        "#{var.to_c} = #{expr.to_c}"
      end
    end
  end
end

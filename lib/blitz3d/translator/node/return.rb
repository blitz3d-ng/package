module Blitz3D
  module AST
    class ReturnNode < StmtNode
      attr_accessor :expr, :return_label

      def initialize(json)
        super
        @expr = Node.load(json['expr'])
        @return_label = json['returnLabel']
      end

      def to_c
        "return #{@expr.to_c}"
      end
    end
  end
end

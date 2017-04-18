module Blitz3D
  module AST
    class ReturnNode < Node
      attr_accessor :expr, :return_label

      def initialize(json)
        @expr = Node.load(json['expr'])
        @return_label = json['returnLabel']
      end

      def to_c
        "return #{@expr.to_c}"
      end
    end
  end
end

module Blitz3D
  module AST
    class ReturnNode < StmtNode
      attr_accessor :expr, :return_label

      def initialize(json)
        super
        @expr = Node.load(json['expr'])
        @return_label = json['returnLabel']
      end

      def to_c(&cleanup)
        cleanup = cleanup.yield
        cleanup << "\n" unless cleanup.blank?
        "#{cleanup}return #{@expr.to_c}"
      end
    end
  end
end

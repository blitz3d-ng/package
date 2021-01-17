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
        if cleanup
          code = cleanup.yield
          code << "\n" unless code.blank?
        else
          code = ''
        end
        "#{code}return #{expr.try(:to_c) || 0}"
      end
    end
  end
end

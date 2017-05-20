module Blitz3D
  module AST
    class InsertNode < StmtNode
      attr_accessor :expr1, :expr2, :before

      def initialize(json)
        super
        @expr1 = Node.load(json['expr1'])
        @expr2 = Node.load(json['expr2'])
        @before = json['before']
      end

      def to_c
        func = before ? '_bbObjInsBefore' : '_bbObjInsAfter'
        "#{func}( #{expr1.to_c},#{expr2.to_c} )"
      end
    end
  end
end

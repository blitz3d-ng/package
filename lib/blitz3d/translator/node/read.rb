module Blitz3D
  module AST
    class ReadNode < StmtNode
      attr_accessor :var

      def initialize(json)
        super
        @var = Node.load(json['var'])
      end
    end
  end
end

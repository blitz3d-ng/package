module Blitz3D
  module AST
    class ExprNode < Node
      attr_accessor :sem_type

      def initialize(json)
        @sem_type = Type.load(json['sem_type'])
      end
    end
  end
end

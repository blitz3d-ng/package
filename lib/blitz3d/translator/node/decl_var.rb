module Blitz3D
  module AST
    class DeclVarNode < Node
      attr_accessor :sem_decl

      def initialize(json)
        @sem_decl = Node.load(json['sem_decl'])
      end

      def to_c
        "<DeclVarNode>".yellow
      end
    end
  end
end

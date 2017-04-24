module Blitz3D
  module AST
    class StructDeclNode < DeclNode
      attr_accessor :ident, :sem_type

      def initialize(json)
        super
        @ident = json['ident']
        @sem_type = Type.load(json['sem_type'])
      end
    end
  end
end

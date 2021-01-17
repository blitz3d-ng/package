module Blitz3D
  module AST
    class VectorDeclNode < DeclNode
      attr_accessor :ident, :tag, :exprs, :kind, :sem_type

      def initialize(json)
        super
        @ident = json['ident']
        @tag = json['tag']
        @exprs = json['exprs'].map { |e| Node.load(e) }
        @kind = json['kind']
        @sem_type = Type.load(json['sem_type'])
      end

      def to_c
        "_v#{ident}=_bbVecAlloc( (BBVecType*)&vector_type#{sem_type.label} )"
      end
    end
  end
end

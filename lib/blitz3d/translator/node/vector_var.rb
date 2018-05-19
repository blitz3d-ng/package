module Blitz3D
  module AST
    class VectorVarNode < VarNode
      attr_accessor :expr, :exprs, :vec_type

      def initialize(json)
        super
        @expr     = Node.load(json['expr'])
        @exprs    = json['exprs'].map { |e| Node.load(e) }
        @vec_type = Type.load(json['vec_type'])
      end

      def to_c
        indices = exprs.map { |e| "[#{e.to_c}]" }
        "((#{vec_type.element_type.to_c}*)#{expr.to_c})#{indices.join(', ')}"
      end
    end
  end
end

module Blitz3D
  module AST
    class ArrayVarNode < VarNode
      attr_accessor :ident, :tag, :exprs, :sem_decl

      def initialize(json)
        super
        @ident = json['ident']
        @tag = json['tag']
        @exprs = json['exprs'].map { |e| Node.load(e) }
        @sem_decl = Decl.new(json['sem_decl'])
      end

      def to_c
        i = exprs.map { |e| "[#{e.to_c}]" }
        "(((#{sem_decl.type.element_type.to_c}*)_a#{ident}.base.data)#{i.join('')})"
      end
    end
  end
end

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
        i = exprs.map.with_index { |e, i| "(#{e.to_c} * #{var}.scales[#{i}])" }
        "((#{sem_decl.type.element_type.to_c}*)#{var}.data)[#{i.join(' + ')}]"
      end

      protected

      def var
        "_a#{ident}.base"
      end
    end
  end
end

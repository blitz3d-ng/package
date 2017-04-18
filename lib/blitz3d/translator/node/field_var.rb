module Blitz3D
  module AST
    class FieldVarNode < Node
      attr_accessor :expr, :ident, :tag, :sem_field

      def initialize(json)
        @expr = Node.load(json['expr'])
        @ident = json['ident']
        @tag = json['tag']
        @sem_field = Decl.new(json['sem_field'])
      end

      def to_c
        "#{expr.to_c}->#{ident}"
      end
    end
  end
end

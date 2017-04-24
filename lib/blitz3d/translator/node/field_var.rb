module Blitz3D
  module AST
    class FieldVarNode < VarNode
      attr_accessor :expr, :ident, :tag, :sem_field

      def initialize(json)
        super
        @expr = Node.load(json['expr'])
        @ident = json['ident']
        @tag = json['tag']
        @sem_field = Decl.new(json['sem_field'])
      end

      def to_c
        index = sem_field.offset / 4
        member = if sem_field.type.is_a?(IntType)
          'INT'
        elsif sem_field.type.is_a?(FloatType)
          'FLT'
        elsif sem_field.type.is_a?(StringType)
          'STR'
        elsif sem_field.type.is_a?(StructType)
          'OBJ'
        end
        throw "no member for #{sem_field.type}" if member.nil?
        "#{expr.to_c}->fields[#{index}].#{member}"
      end
    end
  end
end

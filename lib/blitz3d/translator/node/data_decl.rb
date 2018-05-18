module Blitz3D
  module AST
    class DataDeclNode < DeclNode
      attr_accessor :expr, :str_label

      def initialize(json)
        super
        @expr = Node.load(json['expr'])
        @str_label = json['str_label']
      end

      def to_c
        type_id = if expr.sem_type.is_a?(IntType)
          "{ BBTYPE_INT, #{expr.to_c} }"
        elsif expr.sem_type.is_a?(FloatType)
          "{ BBTYPE_FLT, { .FLT = #{expr.to_c} } }"
        elsif expr.sem_type.is_a?(StringType)
          "{ BBTYPE_CSTR, { .CSTR = #{expr.value.inspect} } }"
        else
          to_s.red
        end
      end
    end
  end
end

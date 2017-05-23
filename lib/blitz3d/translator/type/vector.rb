module Blitz3D
  module AST
    class VectorType < Type
      attr_accessor :element_type, :label, :sizes

      def initialize(json)
        @element_type = Type.load(json['elementType'])
        @label = json['label']
        @sizes = json['sizes']
      end

      def to_c
        'void*'
      end

      def to_h
        "struct BBVecTypeDecl vector_type#{label}={ 6,#{size},#{element_type.ptr} }"
      end

      def size
        sizes.inject(&:+)
      end

      def default_value
        0
      end

      def ptr
        "(BBType*)&vector_type#{label}"
      end

      def kind
        'BBTYPE_VEC'
      end
    end
  end
end

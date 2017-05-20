module Blitz3D
  module AST
    class ArrayType < Type
      attr_accessor :element_type, :dims

      def initialize(json)
        @element_type = Type.load(json['elementType'])
        @dims = json['dims']
      end
    end
  end
end

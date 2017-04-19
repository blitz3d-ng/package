module Blitz3D
  module AST
    class VoidType < Type
      def initialize(attrs = {})
      end

      def to_c
        'void'
      end
    end
  end
end

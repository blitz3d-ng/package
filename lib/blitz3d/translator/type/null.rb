module Blitz3D
  module AST
    class NullType < Type
      def initialize(json = {})
      end

      def to_c
        'void*'
      end
    end
  end
end

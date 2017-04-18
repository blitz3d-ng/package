module Blitz3D
  module AST
    class InvalidType < Type
      attr_accessor :klass

      def initialize(klass)
        @klass = klass
      end

      def to_c
        "<#{klass}>".red
      end
    end
  end
end

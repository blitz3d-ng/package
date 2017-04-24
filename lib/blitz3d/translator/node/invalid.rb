module Blitz3D
  module AST
    class InvalidNode < Node
      attr_accessor :klass, :sem_type

      def initialize(klass)
        @klass = klass
      end

      def to_c
        "<#{klass}>".red
      end

      def pos
        -1
      end
    end
  end
end

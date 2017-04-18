module Blitz3D
  module AST
    class IntConstNode < Node
      attr_accessor :value

      def initialize(json)
        @value = json['value']
      end

      def to_c
        value
      end
    end
  end
end

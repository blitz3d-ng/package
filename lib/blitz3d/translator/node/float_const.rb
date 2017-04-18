module Blitz3D
  module AST
    class FloatConstNode < Node
      attr_accessor :value

      def initialize(json)
        @value = json['value']
      end

      def to_c
        "#{value}f"
      end
    end
  end
end

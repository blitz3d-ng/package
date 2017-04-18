module Blitz3D
  module AST
    class StringConstNode < Node
      attr_accessor :value

      def initialize(json)
        @value = json['value']
      end

      def to_c
        "_bbStrConst(#{value.inspect})"
      end
    end
  end
end

module Blitz3D
  module AST
    class StringConstNode
      attr_accessor :value

      def initialize(json)
        @value = json['value']
      end

      def to_c
        value.inspect
      end
    end
  end
end

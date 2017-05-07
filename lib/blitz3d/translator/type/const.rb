module Blitz3D
  module AST
    class ConstType < Type
      attr_accessor :float_value, :int_value, :string_value, :value_type

      def initialize(json)
        @float_value = json['floatValue']
        @int_value = json['intValue']
        @string_value = json['stringValue']
        @value_type = Type.load(json['valueType'])
      end

      def to_c
        value_type.to_c
      end

      def default_value
        if value_type.is_a?(IntType)
          int_value
        elsif value_type.is_a?(FloatType)
          float_value
        elsif value_type.is_a?(StringType)
          string_value.inspect
        end
      end
    end
  end
end

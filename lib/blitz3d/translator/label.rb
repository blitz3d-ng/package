module Blitz3D
  module AST
    class Label
      attr_accessor :name, :def, :ref, :data_sz

      def initialize(json)
        @name = json['name']
        @def = json['def']
        @ref = json['ref']
        @data_sz = json['data_sz']
      end
    end
  end
end

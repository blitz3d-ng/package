module Blitz3D
  module AST
    class StructType
      attr_accessor :ident, :fields

      def initialize(json)
        @ident = json['ident']
        @fields = json['fields'].map { |field| Decl.new(field) }
      end

      def to_c
        "bb_#{ident}_t"
      end
    end
  end
end

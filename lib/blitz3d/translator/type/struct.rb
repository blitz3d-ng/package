module Blitz3D
  module AST
    class StructType < Type
      attr_accessor :ident, :fields

      def initialize(json)
        @ident = json['ident']
        @fields = json['fields'].map { |field| Decl.new(field) }
      end

      def to_type
        "_t#{ident}"
      end

      def to_c
        "BBObj*"
      end

      def ptr
        "&#{to_type}"
      end
    end
  end
end

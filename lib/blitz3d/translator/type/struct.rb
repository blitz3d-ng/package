module Blitz3D
  module AST
    class StructType < Type
      attr_accessor :ident, :fields

      def self.fetch(json)
        @@cache ||= {}

        type = new(json)
        @@cache[type.ident] ||= type
        @@cache[type.ident].fields = type.fields unless type.fields.nil?

        @@cache[type.ident]
      end

      def self.find(ident)
        @@cache[ident]
      end

      def initialize(json)
        @ident = json['ident']
        @fields = json['fields'].map { |field| Decl.new(field) } if json['fields']
      end

      def to_type
        "_t#{ident}"
      end

      def to_c
        "BBObj*"
      end

      def ptr
        "(BBType *)&#{to_type}"
      end

      def kind
        'BBTYPE_OBJ'
      end

      def default_value
        0
      end
    end
  end
end

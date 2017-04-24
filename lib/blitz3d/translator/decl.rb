module Blitz3D
  module AST
    class Decl
      attr_accessor :kind, :name, :offset, :type

      def initialize(json)
        @kind = json['kind']
        @name = json['name']
        @offset = json['offset']
        @type = Type.load(json['type']) if json['type']
      end

      def to_h
        "#{type.to_c} #{name}"
      end
    end
  end
end

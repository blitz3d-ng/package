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

      def to_h(prefix = '')
        "#{type.to_c} #{prefix}#{name}"
      end
    end
  end
end

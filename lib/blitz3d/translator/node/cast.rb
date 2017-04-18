module Blitz3D
  module AST
    class CastNode
      attr_accessor :type, :expr

      def initialize(json)
        @type = Type.load(json['type'])
        @expr = Node.load(json['expr'])
      end

      def to_c
        "(#{type.to_c})( #{@expr.to_c} )"
      end
    end
  end
end

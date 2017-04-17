module Blitz3D
  module AST
    class CastNode
      attr_accessor :type, :expr

      def initialize(json)
        @type = json['type']
        @expr = Node.load(json['expr'])
      end

      def to_c
        "(#{type.inspect})( #{@expr.to_c} )"
      end
    end
  end
end

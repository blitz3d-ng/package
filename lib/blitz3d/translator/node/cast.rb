module Blitz3D
  module AST
    class CastNode < ExprNode
      attr_accessor :type, :expr

      def initialize(json)
        super
        @type = Type.load(json['type'])
        @expr = Node.load(json['expr'])
      end

      def to_c
        raise "#{expr.class.name} missing sem_type" if expr.sem_type.nil? && !expr.is_a?(InvalidNode)

        if type.is_a?(expr.sem_type.class)
          expr.to_c
        elsif @type.is_a?(StringType)
          if expr.sem_type.class == FloatType
            "_bbStrFromFloat(#{expr.to_c})"
          else
            raise "Need to handle StringType->#{expr.sem_type.class.name} casts"
          end
        else
          "(#{type.to_c})(#{expr.to_c})"
        end
      end
    end
  end
end

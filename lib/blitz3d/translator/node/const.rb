module Blitz3D
  module AST
    class ConstNode < ExprNode
      def initialize(json)
        super
        @value = json['value']
      end
    end
  end
end

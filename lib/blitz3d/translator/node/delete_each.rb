module Blitz3D
  module AST
    class DeleteEachNode
      attr_accessor :type_ident

      def initialize(json)
        @type_ident = json['typeIdent']
      end
    end
  end
end

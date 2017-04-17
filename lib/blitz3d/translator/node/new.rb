module Blitz3D
  module AST
    class NewNode
      attr_accessor :ident

      def initialize(json)
        @ident = json['ident']
      end

      def to_c
        "new _bb_#{ident}()"
      end
    end
  end
end

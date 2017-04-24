module Blitz3D
  module AST
    class StringType < Type
      def initialize(json)
      end

      def to_c
        'BBStr*'
      end

      def to_h
        to_c
      end

      def ptr
        '&_bbStrType'
      end
    end
  end
end

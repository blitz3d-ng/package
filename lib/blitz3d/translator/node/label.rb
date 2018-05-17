module Blitz3D
  module AST
    class LabelNode < StmtNode
      attr_accessor :data_sz, :ident

      def initialize(json)
        super
        @data_sz = json['data_sz']
        @ident = json['ident']
      end

      def to_c(&cleanup)
        "#{ident}:"
      end

      def no_semi
        true
      end
    end
  end
end

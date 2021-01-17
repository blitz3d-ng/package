module Blitz3D
  module AST
    class IdentVarNode < VarNode
      attr_accessor :ident, :tag

      def initialize(json)
        super
        @ident = json['ident']
        @tag = json['tag']
      end

      def to_h
        "#{sem_type.to_c} _v#{ident}"
      end

      def to_c
        "_v#{ident}"
      end
    end
  end
end

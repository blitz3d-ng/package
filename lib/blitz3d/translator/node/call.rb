module Blitz3D
  module AST
    class CallNode
      attr_accessor :ident, :tag, :sem_decl, :exprs

      def initialize(json)
        @ident = json['ident']
        @tag = json['tag']
        @sem_decl = json['sem_decl']
        @exprs = json['exprs'].map { |expr| Node.load(expr) }
      end

      def to_c
        "#{@ident}()"
      end
    end
  end
end

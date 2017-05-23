module Blitz3D
  module AST
    class RestoreNode < StmtNode
      attr_accessor :ident

      def initialize(json)
        super
        @ident = json['ident']
        # @sem_label = json['sem_label']
      end
    end
  end
end

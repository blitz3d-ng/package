module Blitz3D
  module AST
    class RestoreNode < StmtNode
      attr_accessor :ident, :sem_label

      def initialize(json)
        super
        @ident = json['ident']
        @sem_label = Label.new(json['sem_label'])
      end

      def to_c
        "_bbRestore( &_DATA[#{sem_label.data_sz}] )"
      end
    end
  end
end

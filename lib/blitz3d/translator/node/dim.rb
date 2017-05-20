module Blitz3D
  module AST
    class DimNode < StmtNode
      attr_accessor :ident, :tag, :exprs, :sem_type, :sem_decl

      def initialize(json)
        super
        @ident = json['ident']
        @tag = json['tag']
        @exprs = json['exprs'].map { |e| Node.load(e) }
        @sem_type = Type.load(json['sem_type'])
        @sem_decl = Decl.new(json['sem_decl'])
      end

      def to_c(&cleanup)
        array = "_a#{ident}"
        dims = exprs.map.with_index do |e, i|
          if i == 0
            "#{array}.base.scales[0]=#{e.to_c}"
          else
            "#{array}.scales[#{i-1}]=#{e.to_c}"
          end
        end
        [dims, "_bbDimArray( &#{array}.base )"].flatten.join(';')
      end
    end
  end
end

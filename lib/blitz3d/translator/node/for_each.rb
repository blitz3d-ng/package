module Blitz3D
  module AST
    class ForEachNode < StmtNode
      attr_accessor :next_pos, :var, :type_ident, :stmts, :sem_brk

      def initialize(json)
        super
        @next_pos = json['nextPos']
        @var = Node.load(json['var'])
        @type_ident = json['typeIdent']
        @stmts = Node.load(json['stmts'])
        @sem_brk = json['sem_brk']
      end

      def to_c(&cleanup)
        "for( _bbObjEachFirst2(&#{var.to_c},&_t#{type_ident}.type);#{var.to_c}!=0;_bbObjEachNext2(&#{var.to_c}) ){\n  #{stmts.to_c(&cleanup).indent}\n}"
      end
    end
  end
end

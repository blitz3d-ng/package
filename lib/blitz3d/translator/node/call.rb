module Blitz3D
  module AST
    class CallNode < ExprNode
      attr_accessor :ident, :tag, :sem_decl, :exprs

      def initialize(json)
        super
        # STDERR.puts JSON.pretty_generate(sem_decl).yellow
        @ident    = json['ident']
        @tag      = json['tag']
        @sem_decl = Decl.new(json['sem_decl'])
        @exprs    = json['exprs'].map { |expr| Node.load(expr) }
      end

      def to_c
        ident = sem_decl.type.symbol.present? ? sem_decl.type.symbol : "_f#{sem_decl.name}"

        args = exprs.map(&:to_c)
        args = " #{args.join(',')} ".strip

        "#{ident}(#{args})"
      end
    end
  end
end

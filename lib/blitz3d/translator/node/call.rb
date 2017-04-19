module Blitz3D
  module AST
    class CallNode < ExprNode
      attr_accessor :ident, :tag, :sem_decl, :exprs

      def initialize(json)
        super
        @ident = json['ident']
        @tag = json['tag']
        @sem_decl = json['sem_decl']
        # STDERR.puts JSON.pretty_generate(sem_decl).yellow
        @exprs = json['exprs'].map { |expr| Node.load(expr) }
      end

      def to_c
        args = exprs.map(&:to_c).join(',')
        args = " #{args} " unless args.blank?

        ident = sem_decl["type"]["symbol"].present? ? sem_decl["type"]["symbol"] : sem_decl["name"]

        "#{ident}(#{args})"
      end
    end
  end
end

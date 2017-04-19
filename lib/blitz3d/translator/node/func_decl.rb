module Blitz3D
  module AST
    class FuncDeclNode < DeclNode
      attr_accessor :ident, :tag, :locals, :params, :stmts, :sem_type

      def initialize(json)
        super
        # STDERR.puts JSON.pretty_generate(json).yellow
        @ident = json['ident']
        @tag = json['tag']
        @locals = json['locals'].map { |local| Decl.new(local) }
        @params = json['params'].map { |param| Node.load(param) }
        @stmts = Node.load(json['stmts'])
        @sem_type = Type.load(json['sem_type'])
      end

      def to_h
        params = self.params.map do |param|
          decl = locals.find { |d| d.name == param.ident }
          decl.to_h
        end
        "#{sem_type.to_c} #{ident}(#{params.join(', ')})"
      end

      def to_c
        locals = self.locals.map do |decl|
          "#{decl.type.to_c} #{decl.name};"
        end.join("\n")

        "// #{file}\n#{to_h}{\n  #{locals.indent}\n\n  #{stmts.to_c.indent}\n}"
      end
    end
  end
end

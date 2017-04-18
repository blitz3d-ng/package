module Blitz3D
  module AST
    class FuncDeclNode < Node
      attr_accessor :ident, :tag, :locals, :params, :stmts, :sem_type

      def initialize(json)
        STDERR.puts JSON.pretty_generate(json).yellow
        @ident = json['ident']
        @tag = json['tag']
        @locals = json['locals'].map { |local| Decl.new(local) }
        @params = json['params'].map { |param| Node.load(param) }
        @stmts = Node.load(json['stmts'])
        @sem_type = Type.load(json['sem_type'])
      end

      def to_h
        throw locals
        params = self.params.map do |param|
          decl = locals.find { |d| d.name == param.ident }
          puts [decl.ident]
          decl.to_h
        end
        "#{sem_type.to_c} #{ident}(#{params.join(', ')})"
      end

      def to_c
        locals = self.locals.map do |decl|
          "#{decl.type.to_c} #{decl.name};"
        end.join("\n")

        "#{to_h}{\n  #{locals}\n\n  #{stmts.to_c.indent}\n}"
      end
    end
  end
end

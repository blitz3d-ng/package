module Blitz3D
  module AST
    class FuncDeclNode < DeclNode
      attr_accessor :ident, :tag, :locals, :stmts, :sem_type

      def initialize(json)
        super
        # STDERR.puts JSON.pretty_generate(json).yellow
        @ident = json['ident']
        @tag = json['tag']
        @locals = json['locals'].map { |local| Decl.new(local) }
        @stmts = Node.load(json['stmts'])
        @sem_type = Type.load(json['sem_type'])
      end

      def to_h
        params = sem_type.params.map(&:to_h)
        "#{sem_type.to_c} _f#{ident}(#{params.join(', ')})"
      end

      def to_c
        locals = self.locals.map do |decl|
          "#{decl.type.to_c} #{decl.name}=0;"
        end.join("\n")

        body = [
          string_load,
          locals,
          stmts.to_c { string_release },
        ].reject(&:blank?).join("\n")

        "// #{file}\n#{to_h}{\n  #{body.indent}\n}"
      end

      def string_load
        sem_type.params.map do |param|
          if param.type.is_a?(StringType)
            "#{param.name}=_bbStrLoad( &#{param.name} );"
          end
        end.reject(&:blank?).join("\n")
      end

      def string_release
        sem_type.params.map do |param|
          if param.type.is_a?(StringType)
            "_bbStrRelease( #{param.name} );"
          end
        end.reject(&:blank?).join("\n")
      end
    end
  end
end

module Blitz3D
  module AST
    class ProgNode < Node
      attr_accessor :modules, :types, :globals, :locals, :stmts, :funcs, :structs

      def initialize(json)
        @types   = json['types'].map { |t| Type.load(t) }
        @globals = json['globals'].map { |global| Decl.new(global) }
        @modules = json['modules']
        @locals  = json['locals'].map { |local| Decl.new(local) }
        @stmts   = Node.load(json['stmts'])
        @funcs   = json['funcs'].map { |func| Node.load(func) }
        @structs = json['structs'].map { |struct| Node.load(struct) }
        STDERR.puts JSON.pretty_generate(json['structs']).red
      end

      def to_c
        includes = modules.map { |m| "#include <bb/#{m}/commands.h>" }.join("\n")

        # struct _tplayer_decl {
        #   BBObjType type;
        #   BBType *fieldsTypes[2];
        # };
        #
        # _tplayer_decl _tplayer={
        #   BBObjType( 3,&_bbStrType ),
        #   { &_bbIntType,&_bbIntType }
        # };

        types = self.types.map do |type|
          if type.is_a?(VectorType)
            "struct BBVecTypeDecl vector_type#{type.label}={ 6,#{type.size},#{type.element_type.ptr} }"
          end
        end.compact
        types << '' unless types.empty?

        types.unshift 'struct BBVecTypeDecl{ int type;int size;BBType *elementType; }'

        types = types.join(";\n")

        structs = self.structs.map do |struct|
          fields = struct.sem_type.fields.map do |field|
            field.type.ptr
          end

          fields_ident = "#{struct.sem_type.to_type}_fields"

          "struct #{struct.sem_type.to_type}_decl{\n  BBObjType type;\n  BBType *fieldsTypes[#{struct.sem_type.fields.size}];\n};\n#{struct.sem_type.to_type}_decl #{struct.sem_type.to_type}={\n  BBObjType( #{struct.sem_type.fields.size},#{fields.shift} ),\n  { #{fields.join(',')} }\n}"
        end
        structs << '' unless structs.empty?
        structs = structs.join(";\n")

        func_decls = funcs.map(&:to_h).join(";\n")
        func_decls += ';' unless func_decls.blank?

        func_defs = funcs.map(&:to_c).join("\n\n")

        globals = self.globals.map do |decl|
          "static #{decl.type.to_c} #{decl.name}=#{decl.type.default_value};"
        end.join("\n")

        locals = self.locals.map do |decl|
          "#{decl.type.to_c} #{decl.name}=0;"
        end.join("\n")

        statements = [
          includes,
          types,
          structs,
          globals,
          func_decls,
          func_defs,
          "void bbMain(){\n  #{locals.indent}\n  #{stmts.to_c.indent}\n}"
        ].reject(&:blank?).join("\n\n")
      end
    end
  end
end

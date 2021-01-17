module Blitz3D
  module AST
    class ProgNode < Node
      attr_accessor :modules, :types, :globals, :locals, :arrays, :stmts, :funcs, :structs, :data

      def initialize(json)
        $debug = json['debug']
        @types   = json['types'].map { |t| Type.load(t) }
        @globals = json['globals'].map { |global| Decl.new(global) }
        @modules = json['modules']
        @locals  = json['locals'].map { |local| Decl.new(local) }
        @arrays  = json['arrays'].map { |array| Decl.new(array) }
        @stmts   = Node.load(json['stmts'])
        @funcs   = json['funcs'].map { |func| Node.load(func) }
        @structs = json['structs'].map { |struct| Node.load(struct) }
        @data    = json['data'].map { |struct| Node.load(struct) }
        # STDERR.puts JSON.pretty_generate(json['structs']).red
      end

      def to_c
        includes = modules.map { |m| "#include <bb/#{m}/commands.h>" }.join("\n")

        compat_decls = ['struct BBVecTypeDecl{ int type;int size;BBType *elementType; }']
        forward_decls = []

        types = self.types.map(&:to_h).compact
        types << '' unless types.empty?
        types = types.join(";\n")

        struct_init = []

        structs = self.structs.map do |struct|
          fields = struct.sem_type.fields.map do |field|
            field.type.ptr
          end

          type = "#{struct.sem_type.to_type}.type"

          decl = "struct #{struct.sem_type.to_type}_decl #{struct.sem_type.to_type}"

          compat_decls << "struct #{struct.sem_type.to_type}_decl { BBObjType type; BBType *fields[#{struct.sem_type.fields.size-1}]; }"

          forward_decls << "extern #{decl}"

          struct_init << "#{type}.used.next=#{type}.used.prev=&#{type}.used;#{type}.free.next=#{type}.free.prev=&#{type}.free"

          "#{decl}={ { {5},{0,0,0,0,-1},{0,0,0,0,-1},#{struct.sem_type.fields.size},{#{fields.shift}} }, { #{fields.join(',')} } }"
        end

        structs << '' unless structs.empty?
        structs = structs.join(";\n")

        struct_init << '' unless struct_init.empty?
        struct_init = struct_init.join(";\n")

        compat_decls << '' unless compat_decls.empty?
        compat_decls = compat_decls.join(";\n")

        forward_decls << '' unless forward_decls.empty?
        forward_decls = forward_decls.join(";\n")

        func_decls = funcs.map(&:to_h).join(";\n")
        func_decls += ';' unless func_decls.blank?

        func_defs = funcs.map(&:to_c).join("\n\n")

        data_decls = data.map(&:to_c)
        data_decls << "{ BBTYPE_END, 0 }"

        data_decls = "BBData _DATA[] = {\n#{data_decls.join(",\n")}\n};"

        arrays = self.arrays.map do |decl|
          scale = Array.new(decl.type.dims - 1).map { 0 }
          "struct _a#{decl.name}_decl { BBArray base;int scales[#{decl.type.dims-1}]; };\nstruct _a#{decl.name}_decl _a#{decl.name}={ { 0,#{decl.type.element_type.kind},#{decl.type.dims},0 },{#{ scale.join(',') }} }"
        end.compact

        arrays << '' unless arrays.empty?

        arrays = arrays.join(";\n")

        globals = self.globals.map do |decl|
          "static #{decl.type.to_c} _v#{decl.name}=#{decl.type.default_value};"
        end.join("\n")

        locals = self.locals.map do |decl|
          "#{decl.type.to_c} _v#{decl.name}=0;"
        end.join("\n")

        statements = [
          includes,
          compat_decls,
          forward_decls,
          types,
          structs,
          data_decls,
          arrays,
          globals,
          func_decls,
          func_defs,
          "void bbMain(){\n  #{struct_init.indent}\n  #{locals.indent}\n  #{stmts.to_c {}.indent}\n}"
        ].reject(&:blank?).join("\n\n")
      end
    end
  end
end

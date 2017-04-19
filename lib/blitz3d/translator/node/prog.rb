module Blitz3D
  module AST
    class ProgNode < Node
      attr_accessor :modules, :globals, :locals, :stmts, :funcs, :structs

      def initialize(json)
        @globals = json['globals'].map { |global| Decl.new(global) }
        @locals = json['locals'].map { |local| Decl.new(local) }
        @modules = json['modules']
        @stmts   = Node.load(json['stmts'])
        @funcs = json['funcs'].map { |func| Node.load(func) }
        @structs = json['structs'].map { |struct| Node.load(struct) }
      end

      def to_c
        includes = modules.map { |m| "#include <bb/#{m}/commands.h>" }.join("\n")

        func_decls = funcs.map(&:to_h).join(";\n")

        func_defs = funcs.map(&:to_c).join("\n\n")

        globals = self.globals.map do |decl|
          "static #{decl.type.to_c} #{decl.name};"
        end.join("\n")

        locals = self.locals.map do |decl|
          "#{decl.type.to_c} #{decl.name};"
        end.join("\n")

        "#{includes}\n\n#{globals};\n\n#{func_decls};\n\n#{func_defs}\n\nvoid bbMain(){\n  #{locals.indent}\n  #{stmts.to_c.indent}\n}"
      end
    end
  end
end

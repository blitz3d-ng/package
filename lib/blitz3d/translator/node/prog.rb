module Blitz3D
  module AST
    class ProgNode
      attr_accessor :modules, :stmts, :structs

      def initialize(json)
        @modules = json['modules']
        @stmts   = Node.load(json['stmts'])
        @structs = json['structs'].map { |struct| Node.load(struct) }
      end

      def to_c
        includes = modules.map { |m| "#include <bb/#{m}/commands.h>" }.join("\n")
        "#{includes}\n\nvoid bbMain(){\n  #{stmts.to_c.split("\n").join("\n  ")}\n}"
      end
    end
  end
end

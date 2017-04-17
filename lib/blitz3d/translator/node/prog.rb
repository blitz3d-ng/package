module Blitz3D
  module AST
    class ProgNode
      attr_accessor :stmts, :structs

      def initialize(json)
        @stmts   = Node.load(json['stmts'])
        @structs = json['structs'].map { |struct|  Node.load(struct) }
      end

      def to_c
        "void bbMain(){\n#{stmts.to_c}\n}"
      end
    end
  end
end

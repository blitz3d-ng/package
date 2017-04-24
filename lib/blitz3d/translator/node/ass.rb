module Blitz3D
  module AST
    class AssNode < StmtNode
      attr_accessor :var, :expr

      def initialize(json)
        super
        @var = Node.load json['var']
        @expr = Node.load json['expr']
      end

      def to_c
        if var.sem_type.is_a?(StringType)
          "_bbStrStore( &#{var.to_c},#{expr.to_c} )"
        else
          "#{var.to_c} = #{expr.to_c}"
        end
      end
    end
  end
end

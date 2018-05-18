module Blitz3D
  module AST
    class ReadNode < StmtNode
      attr_accessor :var

      def initialize(json)
        super
        @var = Node.load(json['var'])
      end

      def to_c
        case var.sem_type
        when StringType
          "_bbStrStore( &#{var.to_c},_bbReadStr() )"
        when FloatType
          "#{var.to_c} = _bbReadFloat()"
        when IntType
          "#{var.to_c} = _bbReadInt()"
        else
          self.to_s.red
        end
      end
    end
  end
end

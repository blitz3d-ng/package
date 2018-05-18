module Blitz3D
  module AST
    class ArithExprNode < ExprNode
      attr_accessor :op, :lhs, :rhs

      def initialize(json)
        super
        @op = json['op']
        @lhs = Node.load(json['lhs'])
        @rhs = Node.load(json['rhs'])
      end

      def number_to_number
        case op
        when 'SQR'
          "(bb_float_t)pow(#{lhs.to_c}, #{rhs.to_c})"
        when 'MOD'
          if lhs.sem_type.is_a?(FloatType)
            "_bbFMod(#{lhs.to_c}, #{rhs.to_c})"
          else
            "_bbMod(#{lhs.to_c}, #{rhs.to_c})"
          end
        else
          cop = { 'ADD' => '+', 'SUB' => '-', 'MUL' => '*', 'DIV' => '/' }
          "(#{lhs.to_c} #{cop[op]} #{rhs.to_c})"
        end
      end

      def to_c
        raise "#{lhs.class.name} missing sem_type" if lhs.sem_type.nil? && !lhs.is_a?(InvalidNode)
        raise "#{rhs.class.name} missing sem_type" if rhs.sem_type.nil? && !rhs.is_a?(InvalidNode)

        if lhs.sem_type.is_a?(rhs.sem_type.class)
          if lhs.sem_type.is_a?(IntType)
            return number_to_number
          elsif lhs.sem_type.is_a?(FloatType)
            return number_to_number
          elsif lhs.sem_type.is_a?(StringType)
            return "_bbStrConcat(#{lhs.to_c}, #{rhs.to_c})"
          end
        end
        raise "#{lhs.sem_type.class.name} #{op} #{rhs.sem_type.class.name} unhandled"
      end
    end
  end
end

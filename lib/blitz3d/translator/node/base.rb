module Blitz3D
  module AST
    class Node
      def self.load(attrs)
        return attrs if attrs.is_a?(Node)
        return InvalidNode.new('<nil>') if attrs.nil?
        begin
          klass = "Blitz3D::AST::#{attrs['@class']}".constantize
        rescue NameError
          $stderr.puts "Cannot find node type: #{attrs['@class']}".red
          $stderr.puts JSON.pretty_generate(attrs).red
          return InvalidNode.new(attrs['@class'])
        end
        klass.new(attrs)
      end

      def to_c
        inspect.red
      end
    end
  end
end

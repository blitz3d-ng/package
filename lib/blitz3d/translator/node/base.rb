module Blitz3D
  module AST
    class Node
      def self.load(json)
        begin
          klass = "Blitz3D::AST::#{json['@class']}".constantize
        rescue NameError
          puts "Cannot find node type: #{json['@class']}".red
          puts JSON.pretty_generate(json).red
          return
        end
        klass.new(json)
      end
    end
  end
end

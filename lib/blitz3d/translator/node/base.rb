module Blitz3D
  module AST
    class Node
      def self.load(json)
        "Blitz3D::AST::#{json['kind']}".constantize.new(json)
      rescue NameError
        puts "Cannot find node type: #{json['kind']}".red
        puts JSON.pretty_generate(json).red
      end
    end
  end
end

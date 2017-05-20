module Blitz3D
  module AST
    class Type
      def self.load(json)
        return InvalidType.new('<null>') if json.nil?

        begin
          klass = "Blitz3D::AST::#{json['@class']}".constantize
        rescue NameError
          puts "Cannot find type: #{json['@class']}".red
          puts JSON.pretty_generate(json).red
          return InvalidType.new(json['@class'])
        end
        klass.fetch(json)
      end

      def self.fetch(json)
        new(json)
      end
    end
  end
end

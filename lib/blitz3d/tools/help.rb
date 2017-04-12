module Blitz3D
  module Tools
    module Help
      def run
        puts 'Generating help...'

        modules = Blitz3D::Module.all

        modules.each do |mod|
          puts "Found #{mod.name.bold}. #{mod.commands.size} commands."
          mod.commands.each do |command|
            Blitz3D::Help::Reference::Command.new(modules, command).generate command.help
          end

          Blitz3D::Help::Reference::Module.new(modules, mod).generate
        end

        Blitz3D::Help::Index.new.generate('_release/help/index.html')
        Blitz3D::Help::Reference::Index.new(modules).generate

        puts "Writing command index to #{'index.json'.bold}..."
        index = modules.map do |mod|
          {
            name: mod.name,
            commands: mod.commands.map do |command|
              {
                name: command.name
              }
            end
          }
        end

        File.open('_release/help/reference/commands.json', 'w') { |f| f.write JSON.pretty_generate(index) }
      end
    end
  end
end

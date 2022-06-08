module Blitz3D
  module Tools
    module Help
      def run
        puts 'Generating help...'

        modules = Blitz3D::Module.all

        Blitz3D::Help::Index.new.generate('_release/help/index.html')
        Blitz3D::Help::Reference::Index.new.generate

        puts "Writing command index to #{'index.json'.bold}..."
        index = { modules: [], commands: [] }

        modules.each do |mod|
          puts "Found #{mod.name.bold}. #{mod.commands.size} commands."

          index[:modules] << {
            name: mod.name,
            description: mod.description,
            commands: mod.commands.map(&:name)
          }

          mod.commands.each do |command|
            index[:commands] << {
              name: command.name,
              module: mod.name,
              content: command.help
            }
          end
        end

        File.open('_release/help/reference/commands.js', 'w') { |f| f.write "window.commandReference = #{JSON.pretty_generate(index)}" }
      end
    end
  end
end

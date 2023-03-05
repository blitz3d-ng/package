module Blitz3D
  module Tools
    module Help
      def run
        puts 'Generating help...'

        modules = Blitz3D::Module.all

        outdir = File.expand_path('../../../../_release/help', __dir__)

        FileUtils.mkdir_p "#{outdir}/assets"
        FileUtils.cp File.expand_path('../../assets/style.scss', __dir__), "#{outdir}/assets/style.css"

        Blitz3D::Help::Index.new.generate("#{outdir}/index.html")
        Blitz3D::Help::Credits.new.generate("#{outdir}/credits.html")
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
              content: command.help.to_html
            }
          end
        end

        File.open("#{outdir}/reference/commands.js", 'w') { |f| f.write "window.commandReference = #{JSON.pretty_generate(index)}" }
      end
    end
  end
end

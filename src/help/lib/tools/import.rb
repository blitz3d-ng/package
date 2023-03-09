module Blitz3D
  module Tools
    module Import
      def run(module_name)
        mod = Blitz3D::Module.all.find { |modul| modul.id == module_name }
        if mod.blank?
          puts "Cannot find module '#{module_name}' for import.".red
          exit 1
        end

        puts "Importing commands for #{mod.name.bold}..."
        mod.commands.each do |command|
          next if command.help_exists? && command.example_exists?

          existing_help    = Dir.glob("_release/help/commands/*_commands/#{command.name}.htm").first
          existing_example = Dir.glob("_release/help/commands/*_examples/#{command.name}.bb").first

          if !command.help_exists? && existing_help.present?
            existing_help = Nokogiri::HTML(File.open(existing_help).read)
            existing_help = existing_help.xpath('//body').children
            existing_help.children.before('<span class="autoimport"></span>')
            new_help = File.join(mod.help_dir, [command.name, 'md'].join('.'))

            FileUtils.mkdir_p File.dirname(new_help)
            File.open(new_help, 'w') { |f| f.write existing_help.to_html }
            # FileUtils.mkdir_p mod.help_dir
            # FileUtils.cp existing_help,
            puts "  Copied help for #{command.name.bold}"
          end

          if !command.example_exists? && existing_example.present?
            FileUtils.mkdir_p mod.examples_dir
            FileUtils.cp existing_example, File.join(mod.examples_dir, File.basename(existing_example))
            puts "  Copied example for #{command.name.bold}"
          end
        end
      end
    end
  end
end

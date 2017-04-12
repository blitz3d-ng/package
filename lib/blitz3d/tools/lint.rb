module Blitz3D
  module Tools
    module Lint
      def run
        Module.all.each do |mod|
          puts "Checking #{mod.name.bold}..."

          justsize = mod.commands.map { |command| command.name.size }.max

          mod.commands.each do |command|
            errors, warnings = [], []

            warnings << 'missing example' unless command.example_exists?


            if command.help_exists?
              html = command.html_help

              errors << 'missing description' unless html.has_description?

              if !command.params.empty?
                if html.parameters.empty?
                  warnings << 'missing parameters'
                else
                  decl_params = command.params.map(&:identifier)
                  html_params = html.parameters.map(&:identifier)

                  command.params.each { |p| warnings << "missing \"#{p.identifier}\" in parameters list" unless html_params.include?(p.identifier) }
                  html.parameters.each { |p| warnings << "has unknown \"#{p.identifier}\" in parameters list" unless decl_params.include?(p.identifier) }
                end
              elsif html.has_parameters?
                warnings << "shouldn't have parameters"
              end
            else
              errors << 'missing help'
            end

            name = "#{command.name}:".ljust(justsize + 1)

            messages = []

            messages << (errors.to_sentence << '.').red unless errors.empty?
            messages << (warnings.to_sentence << '.').yellow unless warnings.empty?

            # messages << 'all good.'.green if errors.empty? && warnings.empty?

            puts "  #{name} #{messages.join(' ')}" unless messages.blank?
          end
        end

        exit 0
      end

      if ARGV.include?('--import')
        i = ARGV.find_index('--import')
        module_name = ARGV[i + 1]
        if module_name.blank?
          puts "Invalid module name given for import.".red
          exit 1
        end

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

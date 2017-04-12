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
      end
    end
  end
end

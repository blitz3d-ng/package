module Blitz3D
  class Command
    IDENTIFIER_REGEX = /[_A-Za-z]?[_A-Za-z0-9]*/

    attr_accessor :mod, :name, :return_type, :params, :symbol

    class Parameter
      REGEX = /^([_A-Za-z]?[_A-Za-z0-9]*)(%|#|\$)(?:=((-?[0-9]+)|".*"))?/

      attr_accessor :identifier, :type, :default

      def initialize(code)
        _, @identifier, @type, @default = code.match(REGEX).to_a
        raise "invalid param for #{code.red}" unless @identifier.present? && @type.present?
        if @default.present?
          @default = if @default =~ /^-?[0-9]+$/
            @default.to_i
          elsif @default =~ /^-?[0-9]\.[0-9]+$/
            @default.to_f
          elsif @default =~ /^"(.*)"$/
            $1
          end
        end
      end

      def to_s
        [identifier, type, default && "=#{default}"].compact.join('')
      end

      def to_rtsym
        [type, identifier, default && ['=', default.inspect]].flatten.join('')
      end
    end

    def initialize(mod, code)
      @mod = mod
      match = code.match(/^(#{IDENTIFIER_REGEX})(%|#|\$)?\(\s*(.*)\s*\)(:"(#{IDENTIFIER_REGEX})")/)
      _, @name, @return_type, @params, _, @symbol = match.to_a
      @params.strip!
      @params = @params.split(',').map { |s| Parameter.new(s) }
    rescue
      throw "error parsing: #{code}"
    end

    def signature
      rt = return_type != '%' && return_type

      if return_type.blank?
        [name, ' ', params.join(',')].join('')
      else
        [name, return_type, '( ', params ,' )'].join('')
      end
    end

    class View < Nokogiri::HTML::Document
      attr_accessor :command

      def cleanup!(command)
        @command = command

        name_header = xpath('//h1').first

        if name_header
          name_header.content = command.signature
        end
      end

      def has_description?
        xpath('//h2').find { |node| node.text == 'Description' }
      end

      def example_path
        return false if example_link.nil?

        example_path = File.join(command.mod.path, 'docs', example_href)
        example_path && File.exist?(example_path) && example_path
      end

      def example_link
        xpath('//a').find { |node| node.text == 'Example' }
      end

      def example_href
        example_link && example_link['href']
      end

      def example_link=(path)
        if path.present?
          if example_link.nil?
            doc = Nokogiri::XML("<h2><a href=\"#{example_path}\">Example</a></h2>")
            xpath('//body').children.last.after(doc.root)
          end

          example_source = File.open(path).read
          example_link.parent.after("\n<code><pre>\n#{example_source}</pre></code>")
        end
      end

      def parameters_header
        xpath('//h2').find { |node| node.content == 'Parameters' }
      end

      def has_parameters?
        !parameters_header.nil?
      end

      def parameters
        return [] unless has_parameters?
        list = parameters_header.xpath('following-sibling::ul').first
        return [] unless list
        list.xpath('li').map do |item|
          begin
            Parameter.new(item.content)
          rescue
            ident = item.content.match(/^#{IDENTIFIER_REGEX}/)
            Parameter.new("#{ident}%")
          end
        end
      end
    end

    def html_help
      markdown = File.read(help_exists? ? help_path : File.expand_path('../views/commands/not_found.md', __dir__))

      source = GitHub::Markup.render_s(GitHub::Markups::MARKUP_MARKDOWN, markdown)

      doc = View.parse(source)
      doc.cleanup! self
      doc
    end

    def help
      html_doc = html_help
      html_doc.example_link = example_path
      html_doc.xpath('//body').children
    end

    def help_exists?
      help_path
    end

    def help_path
      markdown_path = "#{mod.path}/docs/#{name}.md"
      File.exist?(markdown_path) ? markdown_path : nil
    end

    def example_path
      path = html_help.example_path || "#{mod.path}/docs/examples/#{name}.bb"
      File.exist?(path) ? path : nil
    end

    def example_exists?
      example_path.present?
    end

    def help_or_example_exists?
      help_exists? || example_exists?
    end
  end
end

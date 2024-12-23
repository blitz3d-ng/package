module Highlighter
  def run(html)
    formatter = Rouge::Formatters::HTML.new
    lexer = Highlighter::Lexer.new

    html.css('code pre').each do |block|
      block.children = formatter.format(lexer.lex(block.text))
    end
  end

  def css
    Highlighter::Theme.render(scope: 'code pre')
  end

  module_function :run, :css

  class Theme < Rouge::CSSTheme
    name 'blitzbasic'

    palette :bright_green   => '#a6e22e'
    palette :bright_pink    => '#f92672'
    palette :carmine        => '#960050'
    palette :dark_grey      => '#888888'
    palette :dark_red       => '#aa0000'
    palette :dimgreen       => '#324932'
    palette :dimred         => '#493131'
    palette :emperor        => '#555555'
    palette :grey           => '#999999'
    palette :light_grey     => '#aaaaaa'
    palette :soft_cyan      => '#66d9ef'
    palette :very_dark      => '#1e0010'
    palette :whitish        => '#eeeeee'
    palette :white          => '#ffffff'

    palette :yellow         => '#ffee00'
    palette :green          => '#00ff66'
    palette :light_blue     => '#aaffff'
    palette :dark_blue      => '#225588'
    palette :cyan           => '#33ffdd'

    style Comment,                 :fg => :yellow
    style Error,                            :fg => :carmine, :bg => :very_dark
    style Keyword,
          Keyword::Constant,
          Keyword::Declaration,
          Keyword::Pseudo,
          Keyword::Reserved,
          Keyword::Type,
          Name::Function,          :fg => :light_blue
    style Keyword::Namespace,
          Operator::Word,
          Operator,                         :fg => :white
    style Literal::Number::Float,
          Literal::Number::Hex,
          Literal::Number::Integer::Long,
          Literal::Number::Integer,
          Literal::Number::Oct,
          Literal::Number::Bin,
          Literal::Number,
          Literal::String::Escape,          :fg => :cyan
    style Literal::String::Affix,           :fg => :soft_cyan
    style Literal::String::Backtick,
          Literal::String::Char,
          Literal::String::Doc,
          Literal::String::Double,
          Literal::String::Heredoc,
          Literal::String::Interpol,
          Literal::String::Other,
          Literal::String::Regex,
          Literal::String::Single,
          Literal::String::Symbol,
          Literal::String,                  :fg => :green
    style Name::Attribute,                  :fg => :bright_green
    style Name::Class,
          Name::Decorator,
          Name::Exception,                  :fg => :bright_green
    style Name::Constant,                   :fg => :soft_cyan
    style Name::Builtin::Pseudo,
          Name::Entity,
          Name::Namespace,
          Name::Variable::Class,
          Name::Variable::Global,
          Name::Variable::Instance,
          Name::Variable,
          Text::Whitespace,                 :fg => :whitish
    style Name::Label,                      :fg => :whitish, :bold => true
    style Name::Tag,                        :fg => :bright_pink
    style Text,                             :fg => :whitish, :bg => :dark_blue
  end

  class Lexer < Rouge::RegexLexer
    title "BlitzBasic"
    desc "BlitzBasic syntax"
    tag 'blitzbasic'
    filenames '*,bb'

    def self.punctuation
      @punctuation ||= %w(
        [,;'~] SPC TAB
      )
    end

    def self.function
      @function ||= begin
        commands = Blitz3D::Module.all.map(&:commands).flatten.map(&:name).sort do |a, b|
          c, d = [a, b].sort
          if d.starts_with?(c)
            -(a <=> b)
          else
            a <=> b
          end
        end

        %w(
          Local Global Const Dim
        ) + commands
      end
    end

    def self.statement
      @statement ||= %w(
        Select EndSelect Case Default
        Data Read
        If Then Else EndIf
        While Wend
        For Next Step To Each
        Gosub Goto
        IF NEXT ORIGIN
        Function EndFunction Return
        Repeat Until
        Type EndType Field Delete Before After Insert
        Not Or And
      )
    end

    def self.operator
      @operator ||= %w(
        << <= <> < >= >>> >> > [-!$()*+/=?^|]
      )
    end

    def self.constant
      @constant ||= %w(
        False True
      )
    end

    state :expression do
      rule %r/#{Lexer.function.join('|')}/io, Name::Function  # function or pseudo-variable
      rule %r/#{Lexer.operator.join('|')}/io, Operator
      rule %r/#{Lexer.constant.join('|')}/io, Name::Constant
      rule %r/"[^"]*"/o, Literal::String
      rule %r/[a-z_][\w]*[$%#]?/io, Name::Variable
      rule %r/[\d.]+/io, Literal::Number
      rule %r/%[01]+/io, Literal::Number::Bin
      rule %r/\$[\h]+/io, Literal::Number::Hex
    end

    state :root do
      rule %r/(:+)( *)(\*)(.*)/ do
        groups Punctuation, Text, Keyword, Text # CLI command
      end
      rule %r/(\n+ *)(\*)(.*)/ do
        groups Text, Keyword, Text # CLI command
      end
      rule %r/(ELSE|REPEAT|THEN)( *)(\*)(.*)/ do
        groups Keyword, Text, Keyword, Text # CLI command
      end
      rule %r/[ \n]+/o, Text
      rule %r/:+/o, Punctuation
      rule %r/[\[]/o, Keyword, :assembly1
      rule %r/;.*/o, Comment
      rule %r/(?:#{Lexer.statement.join('|')})/io, Keyword
      mixin :expression
      rule %r/#{Lexer.punctuation.join('|')}/o, Punctuation
    end

    state :assembly1 do
      rule %r/ +/o, Text
      rule %r/]/o, Keyword, :pop!
      rule %r/[:\n]/o, Punctuation
      rule %r/\.[a-z_`][\w`]*%? */io, Name::Label
      rule %r/(?:REM|;)[^:\n]*/o, Comment
      rule %r/[^ :\n]+/o, Keyword, :assembly2
    end

    state :assembly2 do
      rule %r/ +/o, Text
      rule %r/[:\n]/o, Punctuation, :pop!
      rule %r/(?:REM|;)[^:\n]*/o, Comment, :pop!
      mixin :expression
      rule %r/[!#,@\[\]^{}]/, Punctuation
    end
  end
end

class Template
  def render(temp)
    ERB.new(temp).result(binding)
  rescue SyntaxError => e
    throw StandardError.new(e)
  end
end

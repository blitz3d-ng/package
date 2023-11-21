module Blitz3D
  module Help
    class Language
      class Page < Template
        def initialize(path)
          @path = path
        end

        def input_path
          @path
        end

        def layout_path
          File.expand_path('../../views/language/_layout.html.erb', __dir__)
        end

      end

      def generate(outdir)
        FileUtils.rm_r(outdir) if Dir.exist?(outdir)
        FileUtils.mkdir_p outdir

        indir = File.expand_path('../../views/language', __dir__)
        Dir["#{indir}/**"].each do |file|
          next if File.basename(file).start_with?('_')

          Page.new(file).generate("#{outdir}/#{File.basename(file)}")
          #FileUtils.cp file, outdir
        end
      end
    end
  end
end

module Blitz3D
  module Help
    class Tutorials
      class Page < Template
        def initialize(base_path, path)
          @base_path = base_path
          @path = path
        end

        def root_path
          File.expand_path('../../../../_release/help', __dir__)
        end

        def input_path
          @path
        end

        def layout_path
          File.expand_path("../../views/tutorials/#{@base_path}/_layout.html.erb", __dir__)
        end
      end

      def generate(basedir)
        FileUtils.rm_r(basedir) if Dir.exist?(basedir)

        Dir["#{File.expand_path('../../views/tutorials', __dir__)}/*"].each do |dir|
          outdir = "#{basedir}/#{File.basename(dir)}"
          FileUtils.mkdir_p outdir

          FileUtils.cp_r "#{dir}/assets", outdir
          FileUtils.cp_r "#{dir}/code", outdir

          Dir["#{dir}/*.html"].each do |file|
            Page.new(File.basename(dir), file).generate("#{outdir}/#{File.basename(file)}")
          end
        end
      end
    end
  end
end

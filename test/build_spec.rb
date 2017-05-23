
require 'open3'

lists = {
  games: '_release/Games/**/*.bb',
  commands: '_release/help/commands/**/*.bb',
  modules: 'src/**/*.bb',
  samples: '_release/samples/**/*.bb',
  tutorials: '_release/tutorials/**/*.bb'
}

exclusions = File.open('test/exclusions.txt').read.split("\n")

lists.each_pair do |tag, glob|
  files = Dir[glob].reject { |path| File.directory?(path) || exclusions.include?(path) }

  files.each do |file|
    describe file, tag => true do
      file = file.gsub(/\s+/, '\\ ').gsub('&', '\\&')

      it 'should build with the old compiler', legacy: true do
        stdout = `wine _release/bin/blitzcc.exe +q -c -d #{file}`
        status = $?.exitstatus

        expect(status).to be(0), stdout
      end

      it 'should build with the new compiler', ast: true do
        stdout = `_release/bin/blitzcc +q -j -d #{file}`
        status = $?.exitstatus

        expect(status).to be(0), stdout

        stdout = `_release/bin/blitzcc +q -j -d #{file} | bin/ast2c`
        status = $?.exitstatus

        expect(status).to be(0), stdout

        stdout = `_release/bin/blitzcc +q -j -d #{file} | bin/ast2c | clang++ -x c - \`bin/bb-config\``
        status = $?.exitstatus

        expect(status).to be(0), stdout
      end
    end
  end
end

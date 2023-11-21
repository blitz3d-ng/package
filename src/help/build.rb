#!/usr/bin/env ruby

## WARNING: This file is VERY ugly & quite the hackjob.

ENV['BUNDLE_GEMFILE'] = File.expand_path("./Gemfile", __dir__)
require 'bundler'
Bundler.require :default

require 'yaml'
require 'erb'
require 'json'
require 'fileutils'

module Blitz3D
  define_singleton_method(:path) { __FILE__ }
end

require_relative './lib/cli'

def run(mod, &block)
  Class.new { extend mod }.run(*block.try(:yield))
end

if ARGV.include?('--lint')
  module_names = [ARGV[ARGV.find_index('--lint') + 1]] || []
  run(Blitz3D::Tools::Lint) { [module_names] }
elsif ARGV.include?('--import')
  module_name = ARGV[ARGV.find_index('--import') + 1]
  if module_name.blank?
    puts "Invalid module name given for import.".red
    exit 1
  end
  run(Blitz3D::Tools::Import) { module_name }
else
  run Blitz3D::Tools::Help

  if ENV['WATCH'].to_s.size > 0
    puts 'Watching for changes...'.yellow
    root = Pathname.new(File.expand_path("../..", __dir__))
    Filewatcher.new([
      root.join('src/help/**/*'),
      root.join('src/modules/**/*.md'),
      root.join('src/runtime/**/*.md')
    ]).watch do
      run Blitz3D::Tools::Help
    rescue StandardError => e
      puts e
    end
  end
end

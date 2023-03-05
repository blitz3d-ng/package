#!/usr/bin/env ruby

## WARNING: This file is VERY ugly & quite the hackjob.

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

  if ARGV.include?('--watch')
    puts 'Watching for changes...'.yellow
    FileWatcher.new(['src/docs', 'src/runtime/**/*.md']).watch do
      begin
        run Blitz3D::Tools::Help
      rescue StandardError => e
        puts e
      end
    end
  end
end

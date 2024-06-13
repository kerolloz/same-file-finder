require 'digest'
require 'fileutils'

class HashStorage
  def initialize
    @storage = {}
  end

  def add_hash(hash, file_path)
    @storage[hash] ? @storage[hash] << file_path : @storage[hash] = [file_path]
  end

  def get_similar
    result = {}
    @storage.each do |key, value|
      result[key] = value if value.length > 1
    end
    result.values
  end
end

def get_file_hash(file_path)
  file_contents = File.read(file_path)
  Digest::MD5.hexdigest(file_contents)
end

def read_dir(dir)
  contents = Dir.glob("#{dir}/**/*")
  contents.select { |f| File.file?(f) }
end

if ARGV.length != 1
  puts "Please provide a valid directory path"
  exit(-1)
end

dir = ARGV[0]
is_valid_dir = dir && Dir.exist?(dir)
abort("Please provide a valid directory path") unless is_valid_dir

hash_storage = HashStorage.new
read_dir(dir).each do |file|
  hash_storage.add_hash(get_file_hash(file), file)
end

puts hash_storage.get_similar.inspect
exit(0)

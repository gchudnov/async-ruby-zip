async-ruby-zip
==============

Non-blocking zip reading and writing for Ruby


## Requirements.

  * OS X or Linux
  * MRI 1.9.2
  * libzip >=0.10.1

## Example.

```ruby
require 'rubygems'
require 'async_zip'
include AsyncZip


# Non-blocking zip-file creation:
AsyncZip.create('output.zip', ['./images/DSC00012.JPG', './images/DSC00018.JPG', './images/DSC00040.JPG']) do |archive_filename|
  puts 'called on completion'
end

# Non-blocking zip-file extraction:
AsyncZip.extract('output.zip', 'destination_folder') do |file_array|
  puts 'called on completion'
end


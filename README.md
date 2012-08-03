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
AsyncZip.create('./output.zip', ['./images/DSC00012.JPG', './images/DSC00018.JPG', './images/DSC00040.JPG']) do |archive|
  puts 'async, called after zip is done'
end

# Non-blocking zip-file extraction:
AsyncZip.extract('./output.zip', './extracted') do |file_array|
  puts 'called async when done with list of filenames'
end


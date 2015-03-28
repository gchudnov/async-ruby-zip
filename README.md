# async-ruby-zip

> Non-blocking zip reading and writing for Ruby


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
AsyncZip.create(files, './output.zip') do |task|
  puts task.inspect
end

# Non-blocking zip-file extraction:
AsyncZip.extract('./output.zip', './extracted') do |task|
  puts task.inspect
end
```

## Contact

[Grigoriy Chudnov] (mailto:g.chudnov@gmail.com)

## License

Distributed under the [The MIT License (MIT)](https://github.com/gchudnov/inkjet/blob/master/LICENSE).

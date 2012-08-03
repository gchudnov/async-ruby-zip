ENV['RC_ARCHS'] = '' if RUBY_PLATFORM =~ /darwin/

require "mkmf"

File::unlink("Makefile") if (File::exist?("Makefile"))

pkg_config("libzip")

have_header('zip.h')
create_makefile('async_zip_ext')


Gem::Specification.new do |s|
  s.name = 'posix-semaphore'
  s.version = '0.5'
  
  s.summary = 'Access to the POSIX semaphore APIs in Ruby'
  s.description = 'Cross-thread, cross-process synchronisation made tolerably easy'
  
  s.homepage = 'http://github.com/dpkendal/posix-semaphore'
  
  s.authors = ['David Kendal']
  
  s.extensions = ['ext/extconf.rb']
  s.require_paths = ['lib']
  
  s.files = %w{
    ext
    ext/extconf.rb
    ext/posix-semaphore.c
    lib
    lib/posix
    lib/posix/semaphore
    lib/posix/semaphore/mutex.rb
    lib/posix/semaphore.rb
    lib/posix-semaphore.rb
    posix-semaphore.gemspec
    README
  }
end

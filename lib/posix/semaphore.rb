
require 'posix_semaphore_ext'
require 'posix/semaphore/mutex'

module POSIX
  class Semaphore
    attr_reader :id, :name
    
    def initialize name, mode=File::CREAT, perms=0611, value=1
      @id = POSIX.sem_open name, mode, perms, value
      @name = name
    end
    
    def wait timeout=nil
      if timeout.nil?
        POSIX.sem_wait @id
        true
      else
        begin
          if timeout.is_a? Numeric
            timeout = Time.now + timeout
          elsif defined? DateTime and timeout.is_a? DateTime
            timeout = timeout.to_time
          end
          POSIX.sem_timedwait @id, timeout
        rescue Errno::ETIMEDOUT
          return false
        rescue 
        end
      end
    end
    def wait_nonblock
      POSIX.sem_trywait @id
    end
    
    def post
      POSIX.sem_post @id
    end
    
    def close
      POSIX.sem_close @id
    end
    def unlink
      self.class.unlink @name
    end
    
    def self.unlink name
      POSIX.sem_unlink name
    end
    
    def zero?
      begin
        wait_nonblock
      rescue Errno::EAGAIN
        return true
      end
      post
      return false
    end
    alias locked? zero?
    
    def inspect
      "#<#{self.class.name}:#@id name:#{@name.inspect}>"
    end
  end
end

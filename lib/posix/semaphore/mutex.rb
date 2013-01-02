
module POSIX
  class Semaphore
    # Mutex provides a simple wrapper to provide a POSIX-based version
    # of Ruby's built-in Mutex class.
    # 
    # All you need to do to switch from using Ruby's Mutex to using this
    # is change Mutex.new to POSIX::Semaphore::Mutex.new, providing
    # options that are the same as those to POSIX::Semaphore.new.
    class Mutex
      def initialize *args
        @semaphore = Semaphore.new *args
        @have_lock = false
      end
      
      def lock *a
        raise ThreadError, 'this mutex is already locked to us' if @have_lock
        @semaphore.wait *a
        @have_lock = true
      end
      def locked?
        @have_lock or @semaphore.locked?
      end
      
      def synchronize *a
        lock *a
        begin
          yield
        ensure
          unlock
        end
      end
      
      def try_lock
        raise ThreadError, 'this mutex is already locked to us' if @have_lock
        begin
          @semaphore.wait_nonblock
        rescue Errno::EAGAIN
          return false
        end
        return (@have_lock = true)
      end
      
      def unlock
        @semaphore.post
        @have_lock = false
      end
    end
  end
end


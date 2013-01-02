
#include "ruby.h"
#include <semaphore.h>
#include <time.h>
#include <errno.h>

VALUE mPOSIX = Qnil;

void Init_posix_semaphore_ext();
VALUE method_sem_open(int argc, VALUE* argv, VALUE self);
VALUE method_sem_wait(VALUE self, VALUE semid);
VALUE method_sem_trywait(VALUE self, VALUE semid);
VALUE method_sem_timedwait(VALUE self, VALUE semid, VALUE timeout);
VALUE method_sem_post(VALUE self, VALUE semid);
VALUE method_sem_close(VALUE self, VALUE semid);
// VALUE method_sem_getvalue(VALUE self, VALUE semid);
VALUE method_sem_unlink(VALUE self, VALUE semid);

void Init_posix_semaphore_ext() {
  mPOSIX = rb_define_module("POSIX");
  rb_define_singleton_method(mPOSIX, "sem_open", method_sem_open, -1);
  rb_define_singleton_method(mPOSIX, "sem_wait", method_sem_wait, 1);
  rb_define_singleton_method(mPOSIX, "sem_trywait", method_sem_trywait, 1);
  rb_define_singleton_method(mPOSIX, "sem_timedwait", method_sem_timedwait, 2);
  rb_define_singleton_method(mPOSIX, "sem_post", method_sem_post, 1);
  rb_define_singleton_method(mPOSIX, "sem_close", method_sem_close, 1);
  // rb_define_singleton_method(mPOSIX, "sem_getvalue", method_sem_getvalue, 1);
  rb_define_singleton_method(mPOSIX, "sem_unlink", method_sem_unlink, 1);
}

VALUE method_sem_open(int argc, VALUE *argv, VALUE self) {
  if (argc < 2 || argc > 4) {
    rb_raise(rb_eArgError, "wrong number of arguments (%d for 2..4)", argc);
  }
  VALUE name = argv[0];
  VALUE oflag = argv[1];
  Check_Type(name, T_STRING);
  Check_Type(oflag, T_FIXNUM);
  sem_t* semaphore;
  
  if (argc == 2) {
    semaphore = sem_open(StringValueCStr(name), FIX2INT(oflag));
  } else {
    VALUE mode = argv[2];
    VALUE value;
    if (argc == 4) {
      value = argv[3];
    } else {
      value = INT2FIX(1);
    }
    Check_Type(mode, T_FIXNUM);
    Check_Type(value, T_FIXNUM);
    semaphore = sem_open(StringValueCStr(name), FIX2INT(oflag), FIX2INT(mode), FIX2INT(value));
  }
  
  if (semaphore == SEM_FAILED) {
    rb_sys_fail(0);
  }
  
  return INT2FIX(semaphore);
}

VALUE method_sem_wait(VALUE self, VALUE semid) {
  Check_Type(semid, T_FIXNUM);
  
  int result = sem_wait((sem_t*) FIX2LONG(semid));
  if (result < 0) rb_sys_fail(0);
  
  return INT2FIX(result);
}

VALUE method_sem_trywait(VALUE self, VALUE semid) {
  Check_Type(semid, T_FIXNUM);
  
  int result = sem_trywait((sem_t*) FIX2LONG(semid));
  if (result < 0) rb_sys_fail(0);
  
  return INT2FIX(result);
}

VALUE method_sem_timedwait(VALUE self, VALUE semid, VALUE timeout) {
  Check_Type(semid, T_FIXNUM);
  
  struct timespec ts = rb_time_timespec(timeout);
  
  int result = sem_timedwait((sem_t*) FIX2LONG(semid), &ts);
  if (result < 0) rb_sys_fail(0);
  
  return INT2FIX(result);
}

VALUE method_sem_post(VALUE self, VALUE semid) {
  Check_Type(semid, T_FIXNUM);
 
  int result = sem_post((sem_t*) FIX2LONG(semid));
  if (result < 0) rb_sys_fail(0);
  
  return INT2FIX(result);
}

VALUE method_sem_close(VALUE self, VALUE semid) {
  Check_Type(semid, T_FIXNUM);
  
  int result = sem_close((sem_t*) FIX2LONG(semid));
  if (result < 0) rb_sys_fail(0);
  
  return INT2FIX(result);
}

// diked out, too buggy -- not supported on OS X, borks on FreeBSD. dunno about Linux
// VALUE method_sem_getvalue(VALUE self, VALUE semid) {
//   Check_Type(semid, T_FIXNUM);
//   int sval;
//   
//   int result = sem_getvalue((sem_t*) FIX2LONG(semid), &sval);
//   if (result < 0) rb_sys_fail(0);
//   
//   return INT2FIX(sval);
// }

VALUE method_sem_unlink(VALUE self, VALUE name) {
  Check_Type(name, T_STRING);
  
  int result = sem_unlink(StringValueCStr(name));
  if (result < 0) rb_sys_fail(0);
  
  return INT2FIX(result);
}

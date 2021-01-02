#ifndef SHARED_H
#define SHARED_H

#include <cstdlib>
#include <iostream>
#include <mutex>
#include <cstdio>
#include <cstddef>
#include <string.h>

namespace cs540{

std::mutex lock;

// base class for managing type erasure, this is the base class that stores the count and also has functions for decrementing and incrementing the count
  class Counter{
  public:
    int count;
    Counter(){
      this->count = 1;
    }

    virtual ~Counter() {};

    void inc(){
      std::lock_guard<std::mutex> lck (lock);
      this->count++;
    }
    //since dec is also used for deleting, I have a wrapper in the Shared ptr class that has the lock and check for deletion
    void dec(){
      this->count--;
    }
  };


// derived class for type erasure, stores the type of the pointer so it can be deleted properly
  template <typename V>
  class Counter_tmpl : public Counter{
  public:
    V * pointer;
    Counter_tmpl(V *ptr) : Counter(), pointer(ptr){}
    ~Counter_tmpl(){
      if(this->pointer){
        delete this->pointer;
        this->pointer = NULL;
      }
    }
  };

  template <typename T> class SharedPtr{
  public:
    T * pointer;
    Counter * count;

    SharedPtr(){
      this->pointer = NULL;
      this->count = NULL;
    }

    template <typename U> explicit SharedPtr(U *ptr){
      this->pointer = ptr;
      this->count = new Counter_tmpl<U>(ptr);
    }

//copy constructors
    SharedPtr(const SharedPtr &p){
      this->pointer = p.pointer;
      this->count = p.count;
      if(this->count){
        this->count->inc();
      }
    }

    template <typename U> SharedPtr(const SharedPtr<U> &p){
      this->pointer = p.pointer;
      this->count = p.count;
      if(this->count){
        this->count->inc();
      }
    }
//move constructors
    SharedPtr(SharedPtr &&p){
      this->pointer = std::move(p.pointer);
      this->count = p.count;
      p.pointer = NULL;
      p.count = NULL;
    }

    template <typename U> SharedPtr(SharedPtr<U> &&p){
      this->pointer = std::move(p.pointer);
      this->count = p->count;
      p.pointer = NULL;
      p.count = NULL;
    }

//calls the lock, decrements count, and then checks if count is zero.  Deletes cound and pointer and sets both to null
    void dec(){
      std::lock_guard<std::mutex> lck (lock);
      if(this->count){
        this->count->dec();
        if(this->count->count == 0){
          delete this->count;
          this->count = NULL;
          this->pointer = NULL;
        }
      }
    }

//copy assignment
    SharedPtr &operator=(const SharedPtr & p){
      if(*this == p){
        return *this;
      }
      if(this->count){
        this->dec();
      }
      this->pointer = p.pointer;
      this->count = p.count;
      if(this->count){
        this->count->inc();
      }
      return *this;
    }
//templated copy assignment
    template <typename U> SharedPtr<T> &operator=(const SharedPtr<U> & p){
      if(*this == p){
        return *this;
      }
      if(this->count){
        this->dec();
      }
      this->pointer = p.pointer;
      this->count = p.count;
      if(this->count){
        this->count->inc();
      }
      return *this;
    }

//move assignment
    SharedPtr &operator=(SharedPtr &&p){
      if(p){
        this->pointer = static_cast<T*>(p.pointer);
        this->count = p.count;
        }
      p.pointer = NULL;
      p.count = NULL;
      return *this;
    }
// templated move assignment
    template <typename U> SharedPtr &operator=(SharedPtr<U> &&p){
      if(p){
        this->pointer = static_cast<T *>(p->pointer);
        this->count = p->count;
        }
      p->pointer = NULL;
      p->count = NULL;
      p = NULL;
      return *this;
    }

    ~SharedPtr(){
      if(this->count != NULL){
        this->dec();
      }
    }

    void reset(){
      this->pointer = nullptr;
      if(this->count){
        this->dec();
      }
      this->count = nullptr;
    }

    template <typename U> void reset(U *p){
      if(this->count){
        this->dec();
      }
      this->pointer = p;
      this->count = new Counter_tmpl<U>(p);
    }

    T *get() const{
      return this->pointer;
    }

    T &operator*() const{
      return *(this->pointer);
    }

    T *operator->() const{
      return this->pointer;
    }

    explicit operator bool() const{
      if(this->pointer != NULL){
        return true;
      }
      return false;
    }

  };

  template <typename T1, typename T2>
  bool operator==(const SharedPtr<T1> &p1, const SharedPtr<T2> &p2){
    return (p1.get() == p2.get());
  }

  template <typename T>
  bool operator==(const SharedPtr<T> &p1, std::nullptr_t p2){
    return (p1 == p2);
  }

  template <typename T>
  bool operator==(std::nullptr_t p1, const SharedPtr<T> & p2){
    return (p1 == p2);
  }

  template <typename T1, typename T2>
  bool operator!=(const SharedPtr<T1>& p1, const SharedPtr<T2> & p2){
    return (p1.get() != p2.get());
  }

  template <typename T>
  bool operator!=(const SharedPtr<T> & p1, std::nullptr_t p2){
    return (p1 != p2);
  }

  template <typename T>
  bool operator!=(std::nullptr_t p1, const SharedPtr<T> &p2){
    return (p1 != p2);
  }

  template <typename T, typename U>
  SharedPtr<T> static_pointer_cast(const SharedPtr<U> &sp){
    SharedPtr<T> retval = SharedPtr<T>();
    retval.pointer = static_cast<T*>(sp.pointer);
    retval.count = sp.count;
    if(retval.count){
      retval.count->inc();
    }
    return retval;
  }

  template <typename T, typename U>
  SharedPtr<T> dynamic_pointer_cast(const SharedPtr<U> &sp){
    SharedPtr<T> retval = SharedPtr<T>();
    retval.pointer = dynamic_cast<T*>(sp.pointer);
    retval.count = sp.count;
    if(retval.count){
      retval.count->inc();
    }
    return retval;
  }

}
#endif

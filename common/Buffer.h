#ifndef __BUFFER_H
#define __BUFFER_H

#include <util.h>

namespace mgo {


template <class T>
class Buffer
{
public:
   Buffer (): buffer (nullptr), length(0)  {}

   explicit Buffer (size_t len): length (len), buffer(new T[len])  {}

   Buffer (const Buffer& rhs): length (rhs.length), buffer(new T[rhs.length])  
   {
      (*this) = rhs;
   }

   ~Buffer()
   {
      if (buffer)  delete[] buffer;
   }


   void  allocate (size_t len)
   {
      clear();
      length = len;
      buffer = new T[length];
   }

   void  clear()
   {
      if (buffer) 
      {
         delete[] buffer;
         buffer = nullptr;
         length = 0;
      }
   }


   T*  data()
   {
      return buffer;
   }

   const T*  data() const
   {
      return buffer;
   }

   operator T*()
   {
      return buffer;
   }

   operator const T*() const
   {
      return buffer
   }

   size_t size() const
   {
      return length;
   }

   bool empty() const
   {
      return length == 0;
   }


   Buffer&  operator= (const Buffer& rhs)
   {
      size_t len = std::min (length, rhs.length);
      assert (len);
      memcpy (buffer, rhs.buffer, len);
      return *this;
   }

protected:
   size_t length;
   T  *buffer;
};




template <>
class Buffer<FILE>: public Buffer<byte>
{
public:
   Buffer ()  {}

   Buffer (const TCHAR *filename)
   {
      read (filename);
   }


   bool  read (const TCHAR *filename)
   {
      file = fopen (filename, "rb");      
      if (file == nullptr)  return false;

      fseek (file , 0, SEEK_END);
      length = ftell (file);
      if (length == 0)  
      {
         fclose (file);
         return false;
      }

      rewind (file);
      allocate (length);
      bool read_len = fread (buffer, 1, length, file);
      fclose (file);
      return  (read_len == length);
   }

private:
   FILE *file;
};


typedef  Buffer<FILE>  FileBuffer;


} //namespace mgo
#endif


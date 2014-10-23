#ifndef __PATH__
#define __PATH__


#include <windows.h>
#include <Shellapi.h>

#include <iterator>
#include <stdlib.h>
#include <direct.h>
#include <vector>
#include "util.h"


namespace mgo {


#define DIRECTORY_DELIMITER   TEXT('\\')
#define EMPTY_STRING          TEXT("")
#define EXTENSION_ALL         TEXT("*")


 



class File: public WIN32_FIND_DATA
{
public:
   File()
   {
      dwFileAttributes = INVALID_FILE_ATTRIBUTES;
   }

   File (const TCHAR *fullname)
   {
      getAttributes (fullname);
   }

   bool  isValid() const      
   { 
      return  dwFileAttributes != INVALID_FILE_ATTRIBUTES; 
   }

   operator bool()
   {
      return isValid();
   }


   bool  isDirectory() const    
   { 
      return  (dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY; 
   }


   const TCHAR* Name() const
   {
      return  cFileName;
   }

   const TCHAR* Extension() const
   {
      const TCHAR  *ext = _tcsrchr (Name(), TEXT('.'));
      return  ext == NULL?  EMPTY_STRING:  ext+1;
   }

   friend class FileIterator;

protected:

   void getAttributes (const TCHAR *fullname)
   {
      if (FindFirstFile (fullname, this) == INVALID_HANDLE_VALUE)
      {
         dwFileAttributes = INVALID_FILE_ATTRIBUTES;
      }
   }
};


class FileIterator: public std::forward_iterator_tag
{
public:
   enum  SELECTION
   {
      SHOW_ALL_FILES,
      HIDE_DOT_FILES
   };



   FileIterator ()                   
   { 
      handle = INVALID_HANDLE_VALUE;
   }

   FileIterator (const TCHAR *directory, 
                 const TCHAR *extension = EXTENSION_ALL,
                 SELECTION    selection = HIDE_DOT_FILES)
   {
      init (directory, extension, selection);
   }


   ~FileIterator ()                  
   { 
      if (handle != INVALID_HANDLE_VALUE) 
         FindClose (handle);
   }


   void  init (const TCHAR *directory, 
               const TCHAR *extension = EXTENSION_ALL,
               SELECTION    selection = HIDE_DOT_FILES)
   {
      String  file_selection = directory;
      file_selection += DIRECTORY_DELIMITER;
      file_selection += cU("*.");
      file_selection += extension;

      handle = FindFirstFile (file_selection.c_str(), &file);
      if (handle == INVALID_HANDLE_VALUE)
      {
         file.dwFileAttributes = INVALID_FILE_ATTRIBUTES;
      }
      else
      {
         this->selection = selection;
         skipFiles();
      }
   }


   FileIterator& operator++ ()
   {
      if (handle == INVALID_HANDLE_VALUE)  return *this;

      if (FindNextFile (handle, &file))
      {
         skipFiles();
      }
      else // file not found
      {
         FindClose (handle);
         handle = INVALID_HANDLE_VALUE;
         file.dwFileAttributes = INVALID_FILE_ATTRIBUTES;
      }

      return *this;
   }


   File&  operator* ()    
   { 
      return file; 
   }

   File*  operator-> () 
   { 
      return &file; 
   }


   operator  bool()
   {
      return  handle != INVALID_HANDLE_VALUE;
   }


   friend 
   bool  operator== (const FileIterator  &lhs, const FileIterator &rhs)
   {
      return  lhs.handle == rhs.handle;
   }

   friend
   bool  operator!= (const FileIterator  &lhs, const FileIterator &rhs)
   {
      return !(lhs == rhs);
   }


private:
   void skipFiles()
   {
      if (selection == HIDE_DOT_FILES  &&  file.cFileName[0] == TEXT('.'))
      {
          operator++ ();
      }
   }

   HANDLE   handle;
   File    file;
   SELECTION  selection;
};



inline 
bool  isDirectory (const TCHAR* fullname)
{
   return  File(fullname).isDirectory();
}



class Path: public String
{
public:
   Path ()  {}

   Path (const TCHAR* name): String(name)  
   {
      removeBackslash();
   }

   Path (const String &name): String(name)  
   {
      removeBackslash();
   }

   Path&  operator= (const TCHAR* other)
   {
      (String&) (*this) = other;
      removeBackslash();
      return *this;
   }

   Path&  operator= (const String& other)
   {
      (String&) (*this) = other;
      removeBackslash();
      return *this;
   }


   Path&  operator+= (const String& other)
   {
      this->append (cU('\\') + other);
      return *this;
   }

   Path&  operator-- ()
   {
      *this = FolderName();
      return  *this;
   }

   Path  FileName() const
   {
      String::size_type  ind = find_last_of (cU('\\'));
      return  (ind == String::npos)?  *this: substr (ind+1);
   }

   Path  FolderName() const
   {
      String::size_type  ind = find_last_of (cU('\\'));
      return  (ind == String::npos)?  String(): substr (0, ind);
   }


   friend 
   Path  operator+ (Path  lhs, const String &rhs)
   {
      return  lhs += rhs;
   }


private:
   void  removeQuotes()
   {
      if (empty())  return;

      String::iterator last = end() - 1;
      if (*begin() == cU('"'))  erase (begin());
      if (*last == cU('"'))   erase (last);
   }

   void  removeBackslash()
   {
      String::iterator  last = end() - 1;
      if (*last == cU('/')  || *last == cU('\\'))  erase (last);
   }
};


inline
Path getCWD()
{
   TCHAR  name[MAX_PATH];
   _tgetcwd (name, MAX_PATH);
   return  Path(name);
}


inline
Path  getModulePath()
{
   TCHAR  name[MAX_PATH];
   GetModuleFileName (NULL, name, MAX_PATH);
   return  Path(name);
}



inline
bool  deleteFile (const String& file)
{
   size_t  len = file.size();
   TCHAR *buffer = new TCHAR[len + 2];
   memcpy (buffer, file.data(), len);
   buffer[len] = 0;
   buffer[++len] = 0;

   SHFILEOPSTRUCT FileOp;
   FileOp.hwnd   = nullptr;
   FileOp.wFunc  = FO_DELETE; 
   FileOp.pFrom  = buffer;
   FileOp.pTo    = nullptr;
   FileOp.fFlags = FOF_ALLOWUNDO | FOF_NOCONFIRMATION;
   FileOp.hNameMappings = nullptr;
   return  SHFileOperation(&FileOp);
}



} // namespace mgo
#endif  // __PATH_
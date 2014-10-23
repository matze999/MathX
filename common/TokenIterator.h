#include <string>

namespace mgo {

class TokenIterator
{
public:
   TokenIterator (const std::string &str, const char *del = ", "): string(str)
   {
      delimiter = del;
      beg = 0;
      end = string.find_first_of (delimiter, 0);
      new_token = true;
   }
   


   const std::string&  operator*() const 
   {
      return  getToken();
   }


   const std::string*  operator->() const
   {

      return  &getToken();
   }

   TokenIterator&  operator++()
   {
      if (end == std::string::npos)
         beg = end;
      else
      {
         beg = end+1;
         end = string.find_first_of (delimiter, beg);
      }

      new_token = true;
      return *this;
   }

   operator bool()
   {
      return  beg != std::string::npos;
   }

private:
   const std::string&  getToken() const 
   {
      if (new_token)
      {
         if (beg != std::string::npos)
            string.substr (token, beg, end - beg);

         new_token = false;
      }
      return token;
   }

   mutable std::string   token;
   mutable bool          new_token;

   const std::string    &string;
   const char           *delimiter;
   size_t                beg, end;
};



template <class Vector>
void split (const std::string &data, const char *delimiter, Vector &result)
{
   for (TokenIterator token(data); token; ++token)
       result.push_back (*token);
}



} // namespace mgo

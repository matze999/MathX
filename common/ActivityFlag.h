#ifndef __TOOLS_
#define __TOOLS_


namespace mgo {

class ActivityFlag
{
   bool is_active;
public:

   ActivityFlag (bool value = true)    { is_active = value; }

   bool  isActive() const              { return is_active; }

   void  activate()                    { is_active = true; }
   void  enable()                      { is_active = true; }

   void  deactivate()                  { is_active = false; }
   void  disable()                     { is_active = false; }

   void  toggle()                      { is_active = !is_active; }

   ActivityFlag& operator= (bool value) 
   {
      is_active = value;
      return *this;
   }

   operator bool() const               { return  is_active; }
};

} // namespace mgo
#endif 

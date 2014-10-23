#ifndef  __JSONITEM_H__
#define  __JSONITEM_H__

#include <Wrapper.h>
#include <map>


namespace json {


enum class ObjectType: char
{
   Null,
   Integer,
   Number,
   String,
   Bool,
   List,
   Object,
};


struct BaseItem;

typedef  std::vector <BaseItem>             BaseList;
typedef  std::map <std::string, BaseItem>   BaseObject;



struct Allocator
{
   std::string*   newString()
   {
      return  string_alloc.registerObject (new std::string);
   }

   BaseList*   newList()
   {
      return  list_alloc.registerObject (new BaseList);
   }

   BaseObject*   newObject()
   {
      return  object_alloc.registerObject (new BaseObject);
   }

private:
   mgo::Allocator <std::string>  string_alloc;
   mgo::Allocator <BaseList>     list_alloc;
   mgo::Allocator <BaseObject>   object_alloc;
};


typedef  mgo::Wrapper <Allocator, mgo::Assoc::WeakReference>  AllocatorRef;



struct BaseItem
{
   BaseItem(): type(ObjectType::Null)  {}

   union
   {
      int         int_value;
      bool        bool_value;
      double      number_value;
      std::string *string_value;
      BaseList    *list_value;
      BaseObject  *object_value;
   };

   ObjectType  type;
};



class Item
{
public:
   Item&  operator= (const Item& other)
   {
      *base = *other.base;
      return *this;
   }


   Allocator&  getAllocator() const
   {
      return allocator;
   }


   void  swap (Item&  other)
   {
      BaseItem*  item = base;
      base = other.base;
      other.base = item;
   }

   void  setNull()
   {
      base->type = ObjectType::Null;
   }

   bool  isNull() const
   {
      return  base->type == ObjectType::Null;
   }

   bool  hasType (ObjectType ty) const
   {
      return base->type == ty;
   }

   ObjectType  Type() const
   {
      return  base->type;
   }

// bool
   Item& operator= (bool val)
   {
      base->type = ObjectType::Bool;
      base->bool_value = val;
      return *this;
   }

   operator bool() const
   {
      assert (base->type == ObjectType::Bool);
      return  base->bool_value;
   }

// int
   Item& operator= (int val)
   {
      base->type = ObjectType::Integer;
      base->int_value = val;
      return *this;
   }

   operator int() const
   {
      assert (base->type == ObjectType::Integer);
      return  base->int_value;
   }


// double
   Item& operator= (double val)
   {
      base->type = ObjectType::Number;
      base->int_value = val;
      return *this;
   }

   operator double() const
   {
      assert (base->type == ObjectType::Number);
      return  base->int_value;
   }


// string
   Item& operator= (const std::string& val)
   {
      StringValue() = val;
      return *this;
   }

   operator const std::string&() const
   {
      return  StringValue();
   }


// object value

   Item  operator[] (const std::string& name)
   {
      return  Item (&ObjectValue()[name], allocator);
   }

   const Item  operator[] (const std::string& name) const
   {
      Item*  item = const_cast<Item*> (this);
      return  Item (&item->ObjectValue()[name], allocator);
   }


   bool contains (const std::string& name) const
   {
      return  ObjectValue().count (name);
   }


   bool contains (const std::string& name, ObjectType ty) const
   {
      return ObjectValue().count (name) && 
             (*base->object_value)[name].type == ty;
   }



// list value
   Item  operator[] (uint pos)
   {
      return  Item (&ListValue()[pos], allocator);
   }

   const Item  operator[] (uint pos) const
   {
      assert (base->type == ObjectType::List);
      return  Item (&(*base->list_value)[pos], allocator);
   }

   Item  newEntry()
   {
      ListValue().push_back (json::BaseItem());
      return Item (&ListValue().back(), allocator);
   }

   template<class T>
   void  push_back (const T& value)
   {
      newEntry() = value;
   }

   uint size() const
   {
      return   hasType (ObjectType::List)? ListValue().size(): ObjectValue().size();
   }

   bool empty() const
   {
      return  hasType (ObjectType::List)? ListValue().empty(): ObjectValue().empty();
   }

   Item (const Item& other): allocator (other.allocator)  
   {
      base = other.base;
   }

   Item (BaseItem* other, AllocatorRef alloc): 
      base (other),
      allocator (alloc)  {}


   operator BaseItem&()
   {
      return *base;
   }

   operator const BaseItem&() const
   {
      return *base;
   }

protected:

   Item (BaseItem* other): base (other)  {}



   BaseObject&  ObjectValue()
   {
      if (base->type != ObjectType::Object)
      {
         base->type = ObjectType::Object;
         base->object_value = allocator->newObject();
      }
      return *base->object_value;
   }

   const BaseObject&  ObjectValue() const
   {
      assert (base->type == ObjectType::Object);
      return *base->object_value;
   }


   BaseList&  ListValue()
   {
      if (base->type != ObjectType::List)
      {
         base->type = ObjectType::List;
         base->list_value = allocator->newList();
      }
      return *base->list_value;
   }

   const BaseList&  ListValue() const
   {
      assert (base->type == ObjectType::List);
      return *base->list_value;
   }


   std::string&  StringValue()
   {
      if (base->type != ObjectType::String)
      {
         base->type = ObjectType::String;
         base->string_value = allocator->newString();
      }
      return *base->string_value;
   }

   const std::string&  StringValue() const
   {
      assert (base->type == ObjectType::String);
      return *base->string_value;
   }

   mutable AllocatorRef  allocator;
   mutable BaseItem*  base;
};



class  ObjectRef: public Item
{
public:
   typedef  BaseObject::iterator  iterator;
   typedef  BaseObject::value_type  value_type;
   //class  iterator
   //{
   //   BaseIterator  base_iter;
   //   Allocator&  allocator;

   //   iterator (BaseIterator& iter, AllocatorRef& alloc)
   //      : base_iter(iter), allocator(alloc)  {}
   //public:

   //   Item  operator* ()
   //   {
   //      return  Item (&*base_iter, allocator);
   //   }
   //};

   ObjectRef (const Item& item): Item(item)  
   {
      ObjectValue();
   }

   iterator  begin()
   {
      return  ObjectValue().begin();
   }

   iterator  end()
   {
      return  ObjectValue().end();
   }

};



class  ListRef: public Item
{
public:
   ListRef (const Item& item): Item(item)  
   {
      ListValue();
   }
};




class Object: public Item
{
public:
   Object(): Item (new BaseItem) 
   {
      ObjectValue();
   }   
   
   Object (Allocator& alloc): Item (new BaseItem, alloc)
   {
      ObjectValue();
   }

   Object (const Object& other): 
      Item (new BaseItem (*other.base), other.allocator)  {}

   ~Object()
   {
      delete base;
   }


   Object& operator= (const Object& other)
   {
      *base = *other.base;
      return *this;
   }

   void  reassign()
   {
      base->object_value = allocator->newObject();
   }
};



class List: public Item
{
public:
   List(): Item (new BaseItem)
   {
      ListValue();
   }

   List (Allocator& alloc): Item (new BaseItem, alloc) 
   {
      ListValue();
   }

   List (const List& other): 
      Item (new BaseItem (*other.base), other.allocator)  {}


   ~List()
   {
      delete base;
   }

   List& operator= (const List& other)
   {
      *base = *other.base;
   }

   void  reassign()
   {
      base->list_value = allocator->newList();
   }
};


} // namespace json
#endif


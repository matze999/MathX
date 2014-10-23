#ifndef __TYPEINFO_
#define __TYPEINFO_


namespace mp {


enum 
{
   READ_ONLY,
   FULL_ACCESS
};


template <class baseT>
struct typeinfo
{
   typedef        baseT         value_type;
   typedef        baseT         reference;
   typedef        baseT   const_reference;

   enum { access_mode = READ_ONLY };
};



template <class baseT>
struct typeinfo <baseT&>: public typeinfo <baseT> 
{
   typedef        baseT&        reference;
   typedef  const baseT&  const_reference;

   enum { access_mode = FULL_ACCESS };
};



template <class baseT>
struct typeinfo <const baseT&>: public typeinfo <baseT>
{
   typedef  const baseT&        reference;
   typedef  const baseT&  const_reference;
};


} // namespace mp
#endif // __META_

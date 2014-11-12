


/* This approch make sense for scenarios where
  there are only a few of visitor classes but
  any number of element classes
*/

class Visitor
{
public:
   void  visit (Element& element)
   {
      element.accept (*this)
   }
};


// make more generally
template <class Visitor>
class make_visitor
{
public:
   void  visit (Element& element)
   {
      element.accept (static_cast <Visitor&> (*this));
   }
};


class Element
{
   virtual ~Element() {}

   virtual void accept (Visitor&) = 0;
   // In real implementation, you can choose an addressing name 
};

template <class Base>
class make_element: public Element
{
   virtual void accept (Visitor& visitor)
   {
      ElementVisitor <Base, Visitor> device;
      device.execute (*this, visitor);
   }
};


template <class Element, class Visitor>
class ElementVisitor
{
public:
   void execute()  {}
};


struct ElementA: public makeElement < ElementA >
{};
struct ElementB: public makeElement < ElementB >
{};





#endif 


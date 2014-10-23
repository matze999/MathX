#ifndef __OPTIONS_
#define __OPTIONS_

#include "listing.h"


enum option_type 
{
   OPT_NOARG,
   OPT_INTARG,
   OPT_STRARG
};


struct optionT
{
   optionT ()  {}
   optionT (char idx, option_type ty, char *desc, int val = 0)
   {
      id          = idx;
      type        = ty;
      description = desc;
      value       = val;
      content     = NULL;
   }


   bool isSet()
   {
      return  value != 0;
   }

   char         id;
   option_type  type;
   const char  *description;
   int          value;
   const char  *content;
};



template <int capacity = 20, int tag = 0>
class option_listT: public mgo::listingT <optionT, capacity, tag>
{
public:
   option_listT () {}

   option_listT (char opt_id, option_type type, char *desc, int val = 0)
      : mgo::listingT <optionT, capacity, tag> (optionT (opt_id, type, desc, val))  {}

   explicit
   option_listT (char opt_id);

   static
   void readInput (int &argc, char *argv[]);

   static
   void display();
};



//*******************************************************************************
template <int capacity, int tag>
option_listT <capacity, tag>::option_listT (char opt_id)
{
   while (id < length)
   {
      if (list[id].id == opt_id)  break;
      ++id;
   }
   if (id == length)  id = 0;
}


template <int capacity, int tag>
void option_listT <capacity, tag>::readInput (int &argc, char *argv[])
{
   int       inpos  = 1, 
             argpos = 1, 
             optpos = 0;         // position of the option in the input string
   char     *token = NULL;       // actual string in argv


   for (;;)
   {
      if (token == NULL || *token == 0)          /* read next program argument */
      {
         if (inpos == argc)  break;              /* all arguments readed */

         token = argv[inpos++];
         if (*token != '-')                      /* 'real' argument */
         {
            argv[argpos++] = token;
            token = NULL;
            continue;
         }
         ++token;
      }

      // input options block found => search for a match with program options
      option_listT  option (*token);
      if (option.isInitial())                /* option not found */                   
      {
         printf ("WARNING  %s: Unknow option '%c'.\n", argv[0], *token++);
         continue;
      }

      // program option (*token) found
      ++token;
       
      if (option->type == OPT_INTARG || option->type == OPT_STRARG)
      {
         if (*token)
            option->content = token; 
         else if (inpos < argc)
            option->content = argv[inpos++];
         else
         {
            printf ("ERROR  %s: Option '%c' need an argument.\n", argv[0], token[-1]);
            argc = argpos;
            return;
         }
         token = NULL;                   /* process with new argument */
      }

      option->value = (option->type == OPT_INTARG? atoi(option->content): ++optpos);
   } /* loop over all program arguments */

   argc = argpos;
}



template <int capacity, int tag>
void option_listT <capacity, tag>::display ()
{
   iterator  option = begin(),
               last = end();

   while (option != last)        /* loop over program options */
   {
      if (option->content)
         printf ("   -%c: %s [%d, %s]\n", 
                 option->id, option->description, option->value, option->content);
      else
         printf ("   -%c: %s [%d]\n", 
                 option->id, option->description, option->value);
      ++option;
   }
}

#endif // __OPTIONS_


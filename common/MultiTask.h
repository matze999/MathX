#ifndef __MULTI_TASK_
#define __MULTI_TASK_

#include <util.h>


namespace mgo {


// precondition: class Task contains method 
// int Task::execute()

// task should be created on the heap, i.e using new 

template <class Task>
class MultiTask: public Task
{
public:


   ~MultiTask()  
   {
      clear();
   }


   void clear()
   {
      with (tasks)  delete tasks[pos_];
      tasks.clear();
   }

   void  add (Task* task)
   {
      tasks.push_back (task);
   }
   

   int execute()
   {
      int rc = 0;
      with (tasks)  
         rc = max (tasks[pos_]->execute(), rc);
      return rc;
   }

private:
   std::vector <Task*>  tasks;
};


template <class Task>
class TaskQueue: public Task
{
public:
   ~TaskQueue()  
   {
      clear();
   }


   void clear()
   {
      with (tasks)  delete tasks[pos_];
      tasks.clear();
   }

   void  push_back (Task* task)
   {
      queue.push_back (task);
   }

   int execute()
   {
      if (queue.empty())  return 0;

      int rc = queue.front()->execute();
      if (rc == 0  &&  !queue.empty())
      {
         queue.pop_front();
         rc = 1;
      }
      return rc;
   }

private:
   deque <Task*>  queue;
};

} // namespace mgo
#endif

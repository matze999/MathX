#ifndef __GLPP_
#define __GLPP_


//#define GL_GLEXT_PROTOTYPES 1
//#define GL3_PROTOTYPES 1
//
//#include <GL3/GL3.h>

//#include <gl/gl.h>
//#include "glext.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <Buffer.h>

//#pragma comment(lib,"glew32.lib")


struct GlewInitiator
{
   GlewInitiator()
   {
      if (glewInit())
      {
         //printf("Initializing glew failed\n");
         exit(1);
      }

      if (!glewIsSupported("GL_VERSION_3_0"))
      {
        //printf("OpenGL 3.0 not supported\n");
        exit(1);
      }
   }
};


inline  GLubyte glVersion()
{
   return  glGetString (GL_VERSION)[0] - '0';
}

namespace _ {
   struct glDrawImp
   {
      glDrawImp (int mode)          { glBegin (mode); }

      ~glDrawImp()                  { glEnd(); }

      operator bool()               { return true; }
   };
}

#define glDraw(mode)  if (_::glDrawImp _= mode) 
//
//
//
//#define glItem(item, dim, type)  item ##dim ##type
//#define glItemDim(item, type)  item ##type
//
//#define glItemInt(item)  item##i
//
//
template <class T>
void glVertex (T x, T y)
{
   glVertex2f ((float) x, (float) y);
}

inline
void glVertex (int x, int y)
{
   glVertex2i (x, y);
}


template <class T>
void glVertex (T x, T y, T z)
{
   glVertex3f ((float) x, (float) y, (float) z);
}

inline
void glVertex (int x, int y, int z)
{
   glVertex3i (x, y, z);
}


template <class T>
void glQuad (T x0, T y0, T x1, T y1)
{
   glBegin (GL_QUADS);
      glVertex (x0, y0);
      glVertex (x0, y1);
      glVertex (x1, y1);
      glVertex (x1, y0);
   glEnd();
}

inline
void glColor (uint color)
{
   glColor4ubv ((GLubyte*) &color);
}


inline
void glColor (float red, float green, float blue, float alpha = 1.f)
{
   glColor4f (red, green, blue, alpha);
}


inline 
void  glTranslate (float x, float y, float z = 0)
{
   glTranslatef (x, y, z);
}


template <class T>
void  glTranslate (const T  &array)
{
   glTranslatef (array[0], array[1], array[2]);
}



inline 
void  glScale (float x, float y, float z = 1)
{
   glScalef (x, y, z);
}

inline 
void  glScale (float s)
{
   glScalef (s, s, s);
}

template <class T>
void  glScale (const T  &array)
{
   glScalef (array[0], array[1], array[2]);
}



template <GLenum  type = GL_ARRAY_BUFFER>
class glBuffer
{
public:
   explicit glBuffer()
   {
      glGenBuffers (1, &id);
   }

   void  bind()
   {
      glBindBuffer (type, id);
   }

   void  setData (uint len, void *data, GLenum hint = GL_STATIC_DRAW)
   {
      glBufferData (type, len, data, hint);
   }
private:
   GLuint  id;
};




class glVertexArray
{
public:
   explicit glVertexArray()
   {
      glGenVertexArrays (1, &id);
   }

   void  bind()
   {
      glBindVertexArray (id);
   }

private:
   GLuint  id;
};


//template <class T>
//struct glFeatureBlock
//{
//   glFeatureBlok (
//   operator bool()
//   {
//      return true;
//   }
//};



template <GLint size, GLenum type = GL_FLOAT>
class glVertexAttrib
{
public:
   glVertexAttrib()
   {
      id = 0;
   }
   glVertexAttrib (GLuint  index)
   {
      id = index;
   }


   void setIndex (GLuint  index)
   {
      id = index;
   }

   void enable()
   {
      glEnableVertexAttribArray (id);
   }

   void disable()
   {
      glDisableVertexAttribArray (id);
   }

   void setData (GLsizei stride, const GLvoid *pointer) 
   {
      glVertexAttribPointer (id, size, type, GL_FALSE, stride, pointer); 
   }

private:
   GLuint  id;
};



template <class T>
bool glBindElement (T &el)
{
   el.bind();
   return true;
}


#define glBind(el)   if (glBindElement (el))


template <GLenum  type>
class  glShader
{
public:
   glShader()
   {
      id = glCreateShader (type);
   }

   bool  readFile (const char *file_name)
   {
      FileBuffer file;
      if (file.read(file_name))
      {
         const GLchar*  data = (const GLchar*) file.data();
         glShaderSource (id, 1, &data, nullptr);
         return true;
      }
      return false;
   }

   void compile()
   {
      glCompileShader (id);
   }


   GLsizei getInfoLogLength()
   {
      GLsizei len = 0;
      glGetShaderiv (id, GL_INFO_LOG_LENGTH, &len);
      return len;
   }


   GLsizei getInfoLog (char* log, GLint size)
   {
      GLsizei len = 0;
      glGetShaderInfoLog (id, size, &len, log);
      return len;
   }


   friend class glProgram;
private:
   GLuint  id;
};



class glProgram
{
public:
   glProgram()
   {
      id = glCreateProgram();
   }

   void link()
   {
      glLinkProgram (id);
   }

   template <GLenum type>
   void  attach (glShader<type> &shader)
   {
      glAttachShader (id, shader.id);
   }

   int getAttribLocation (const char* attrib)
   {
      return glGetAttribLocation (id, attrib);
   }

   void use()
   {
      glUseProgram (id);
   }

   
private:
   GLuint id;
};



#ifdef _DEBUG	
#define GL(statement) \
   statement; \
   if (int error = glGetError())  _CrtDbgBreak(); 

#else // RELEASE
#define GL(statement)  statement;
#endif


#endif //__GLPP_
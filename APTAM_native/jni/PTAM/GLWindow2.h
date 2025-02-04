// -*- c++ -*-
// Copyright 2008 Isis Innovation Limited
// Modified by ICGJKU 2015

#ifndef __GL_WINDOW_2_H
#define __GL_WINDOW_2_H
//
//  A class which wraps a CVD::GLWindow and provides some basic
//  user-interface funtionality: A gvars-driven clickable menu, and a
//  caption line for text display. Also provides some handy GL helpers
//  and a wrapper for CVD's text display routines.

#ifdef __ANDROID__
#include <../../ndk-modules/cvd/installfiles/cvd/image.h>
#include <map>
#else
#include <../../ndk-modules/cvd/installfiles/cvd/glwindow.h>
#endif
#include <../../ndk-modules/TooN/include/TooN/TooN.h>

namespace APTAM {

class GLWindowMenu;

#ifdef __ANDROID__
class GLWindow2
#else
class GLWindow2 : public CVD::GLWindow, public CVD::GLWindow::EventHandler
#endif
{
public:
  GLWindow2(CVD::ImageRef irSize, std::string sTitle);
  
  // The preferred event handler..
  void HandlePendingEvents();
  
  // Menu interface:
  void AddMenu(std::string sName, std::string sTitle);
  void DrawMenus();
  
  // Some OpenGL helpers:
  void SetupViewport();
  void SetupVideoOrtho();
  void SetupUnitOrtho();
  void SetupWindowOrtho();
  void SetupVideoRasterPosAndZoom();

  // Text display functions:
  void PrintString(CVD::ImageRef irPos, std::string s);
  void DrawCaption(std::string s);
  
  // Map viewer mouse interface:
  std::pair<TooN::Vector<6>, TooN::Vector<6> > GetMousePoseUpdate();

#ifdef __ANDROID__
  CVD::ImageRef size() const;
    
    void on_mouse_down(int x, int y);
    void on_key_down(int keycode);

    void resize(int w, int h);

    static std::string getFDir();
#else
    static std::string getFDir(){ return "";}
#endif

protected:
  void GUICommandHandler(std::string sCommand, std::string sParams);
  static void GUICommandCallBack(void* ptr, std::string sCommand, std::string sParams);
  
  // User interface menus:
  std::vector<GLWindowMenu*> mvpGLWindowMenus;

  CVD::ImageRef mirVideoSize;   // The size of the source video material.

  // Event handling routines:
#ifdef __ANDROID__
  virtual void on_key_down(GLWindow2&, int key);
  virtual void on_mouse_move(GLWindow2& win, CVD::ImageRef where, int state);
  virtual void on_mouse_down(GLWindow2& win, CVD::ImageRef where, int state, int button);
  virtual void on_event(GLWindow2& win, int event);
#else
  virtual void on_key_down(GLWindow&, int key);
  virtual void on_mouse_move(GLWindow& win, CVD::ImageRef where, int state);
  virtual void on_mouse_down(GLWindow& win, CVD::ImageRef where, int state, int button);
  virtual void on_event(GLWindow& win, int event);
#endif
  CVD::ImageRef mirLastMousePos;

  // Storage for map viewer updates:
  TooN::Vector<6> mvMCPoseUpdate;
  TooN::Vector<6> mvLeftPoseUpdate;

#ifdef __ANDROID_API__
  CVD::ImageRef mirWindowSize; //TODO
  const void drawTextJava(std::string,CVD::ImageRef pos, int shaderid) const;
#endif
};

}
#endif

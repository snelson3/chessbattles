#include "Display.cxx"
#include "Board.C"
#include <iostream>
#include "Gamemaster.C"
#include "Piece.C"

/*=========================================================================

  Program:   Visualization Toolkit
  Module:    SpecularSpheres.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
//
// This examples demonstrates the effect of specular lighting.
//
#include "vtkInteractorStyle.h"
#include <vtkInteractorStyleTrackballCamera.h>
#include "vtkSmartPointer.h"
#include "vtkSphereSource.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkInteractorStyle.h"
#include "vtkObjectFactory.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkProperty.h"
#include "vtkCamera.h"
#include "vtkLight.h"
#include "vtkOpenGLPolyDataMapper.h"
#include "vtkJPEGReader.h"
#include "vtkImageData.h"
#include <vtkTextActor.h>

#include <vtkPolyData.h>
#include <vtkPointData.h>
#include <vtkPolyDataReader.h>
#include <vtkPoints.h>
#include <vtkUnsignedCharArray.h>
#include <vtkFloatArray.h>
#include <vtkDoubleArray.h>
#include <vtkCellArray.h>
#include <cmath>
#include <vector>

const double frames = 30;
const int sz_board = 20;
const double sz_square = sz_board/9;
const double sqDist = sz_square*0.71*2;

void glTranslate(float x, float y, float z)
{
  //because I keep mistyping it
  glTranslatef(x,y,z);
}

class Triangle
{
  public:
      double         X[3];
      double         Y[3];
      double         Z[3];
};





class vtk441Mapper : public vtkOpenGLPolyDataMapper
{
  protected:
   GLuint displayList;
   bool   initialized;

  public:
   vtk441Mapper()
   {
     initialized = false;
   }

   void
   RemoveVTKOpenGLStateSideEffects()
   {
     float Info[4] = { 0, 0, 0, 1 };
     glLightModelfv(GL_LIGHT_MODEL_AMBIENT, Info);
     float ambient[4] = { 1,1, 1, 1.0 };
     glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
     float diffuse[4] = { 1, 1, 1, 1.0 };
     glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
     float specular[4] = { 1, 1, 1, 1.0 };
     glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
   }


   void SetupLight(void)
   {
       glEnable(GL_LIGHTING);
       glEnable(GL_LIGHT0);
       GLfloat diffuse0[4] = { 0.6, 0.6, 0.6, 1 };
       GLfloat ambient0[4] = { 0.2, 0.2, 0.2, 1 };
       GLfloat specular0[4] = { 0.0, 0.0, 0.0, 1 };
       GLfloat pos0[4] = { 0, .707, 0.707, 0 };
       glLightfv(GL_LIGHT0, GL_POSITION, pos0);
       glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse0);
       glLightfv(GL_LIGHT0, GL_AMBIENT, ambient0);
       glLightfv(GL_LIGHT0, GL_SPECULAR, specular0);
       glEnable(GL_LIGHT1);
       GLfloat pos1[4] = { .707, -.707, 0 };
       glLightfv(GL_LIGHT1, GL_POSITION, pos1);
       glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse0);
       glLightfv(GL_LIGHT1, GL_AMBIENT, ambient0);
       glLightfv(GL_LIGHT1, GL_SPECULAR, specular0);
       glDisable(GL_LIGHT2);
       glDisable(GL_LIGHT3);
       glDisable(GL_LIGHT5);
       glDisable(GL_LIGHT6);
       glDisable(GL_LIGHT7);
   }
};

class vtk441MapperPart3 : public vtk441Mapper
{
 public:
   static vtk441MapperPart3 *New();

   GLuint displayList;
   int active[2];
   float sz_stand;//, sz_board, sz_square;
   float sz_pa_body,sz_pa_head,sz_pa_body_ht;
   float sz_bishop_base, sz_bishop_ht,sz_bishop_width;
   int bishop_layers;
   float sz_queen_base, sz_queen_ht, sz_queen_head, sz_queen_hat;
   float sz_king_base, sz_king_ht, sz_king_head, sz_king_cross;
   float sz_ro_base, sz_ro_ht, sz_ro_rampart;
   float sz_kn_base, sz_kn_ht, sz_kn_head, sz_kn_face;
   Board *board;

   vtk441MapperPart3()
   {
    active[0] = 9;
    active[1] = 9;
    // sz_board = 20;
    // //sz_square = sz_board/9;
    // sz_square = sz_board/9;
    sz_stand = sz_square-(0.3*sz_square);

    sz_pa_body = sz_stand-(0.35*sz_stand);
    sz_pa_body_ht = sz_pa_body*3.2;
    sz_pa_head = sz_pa_body+(0.250*sz_pa_body);

    sz_bishop_base = sz_stand-(0.30*sz_stand);
    sz_bishop_ht = sz_pa_body_ht*2;
    sz_bishop_width = sz_stand/2;//this is weird
    bishop_layers = 30;

    sz_queen_base = sz_stand-(0.23*sz_stand);
    sz_queen_ht = sz_bishop_ht;
    sz_queen_head = sz_queen_base+(0.17*sz_queen_base);
    sz_queen_hat = sz_queen_head-(0.2*sz_queen_head);

    sz_king_base = sz_stand-(0.18*sz_stand);
    sz_king_ht = sz_queen_ht;
    sz_king_head = sz_king_base+(0.25*sz_king_base);
    sz_king_cross = sz_king_head-(0.4*sz_king_head);

    sz_ro_base = sz_stand;
    sz_ro_ht = sz_bishop_ht/3+sz_bishop_ht/3;
    sz_ro_rampart = sz_ro_base/3;

    sz_kn_base = sz_pa_body - (0.10*sz_pa_body);
    sz_kn_ht = sz_pa_body_ht + (0.17*sz_pa_body_ht);
    sz_kn_head = sz_pa_head - (0.20*sz_pa_head);
    sz_kn_face = sz_kn_head - (0.30*sz_kn_head);
   }

   void setActive(int i, int j)
   {
    active[0] = i;
    active[1] = j;
   }

   void setBoard(Board *b)
   {
    board = b;
   }

   void DrawCube()
   {
       int nfacets = 4;
       glBegin(GL_TRIANGLES);
       for (int i = 0 ; i < nfacets ; i++)
       {
           double angle = 3.14159*2.0*i/nfacets;
           double nextAngle = (i == nfacets-1 ? 0 : 3.14159*2.0*(i+1)/nfacets);
           glNormal3f(0,0,1);
           glVertex3f(0, 0, 1);
           glVertex3f(cos(angle), sin(angle), 1);
           glVertex3f(cos(nextAngle), sin(nextAngle), 1);
           glNormal3f(0,0,-1);
           glVertex3f(0, 0, 0);
           glVertex3f(cos(angle), sin(angle), 0);
           glVertex3f(cos(nextAngle), sin(nextAngle), 0);
       }
       glEnd();
       glBegin(GL_QUADS);
       for (int i = 0 ; i < nfacets ; i++)
       {
           double angle = 3.14159*2.0*i/nfacets;
           double nextAngle = (i == nfacets-1 ? 0 : 3.14159*2.0*(i+1)/nfacets);
           glNormal3f(cos(angle), sin(angle), 0);
           glVertex3f(cos(angle), sin(angle), 1);
           glVertex3f(cos(angle), sin(angle), 0);
           glNormal3f(cos(nextAngle), sin(nextAngle), 0);
           glVertex3f(cos(nextAngle), sin(nextAngle), 0);
           glVertex3f(cos(nextAngle), sin(nextAngle), 1);
       }
       glEnd();
   }

   void DrawCylinder()
   {
       int nfacets = 30;
       glBegin(GL_TRIANGLES);
       for (int i = 0 ; i < nfacets ; i++)
       {
           double angle = 3.14159*2.0*i/nfacets;
           double nextAngle = (i == nfacets-1 ? 0 : 3.14159*2.0*(i+1)/nfacets);
           glNormal3f(0,0,1);
           glVertex3f(0, 0, 1);
           glVertex3f(cos(angle), sin(angle), 1);
           glVertex3f(cos(nextAngle), sin(nextAngle), 1);
           glNormal3f(0,0,-1);
           glVertex3f(0, 0, 0);
           glVertex3f(cos(angle), sin(angle), 0);
           glVertex3f(cos(nextAngle), sin(nextAngle), 0);
       }
       glEnd();
       glBegin(GL_QUADS);
       for (int i = 0 ; i < nfacets ; i++)
       {
           double angle = 3.14159*2.0*i/nfacets;
           double nextAngle = (i == nfacets-1 ? 0 : 3.14159*2.0*(i+1)/nfacets);
           glNormal3f(cos(angle), sin(angle), 0);
           glVertex3f(cos(angle), sin(angle), 1);
           glVertex3f(cos(angle), sin(angle), 0);
           glNormal3f(cos(nextAngle), sin(nextAngle), 0);
           glVertex3f(cos(nextAngle), sin(nextAngle), 0);
           glVertex3f(cos(nextAngle), sin(nextAngle), 1);
       }
       glEnd();
   }

   std::vector<Triangle> SplitTriangle(std::vector<Triangle> &list)
   {
       std::vector<Triangle> output(4*list.size());
       for (unsigned int i = 0 ; i < list.size() ; i++)
       {
           double mid1[3], mid2[3], mid3[3];
           mid1[0] = (list[i].X[0]+list[i].X[1])/2;
           mid1[1] = (list[i].Y[0]+list[i].Y[1])/2;
           mid1[2] = (list[i].Z[0]+list[i].Z[1])/2;
           mid2[0] = (list[i].X[1]+list[i].X[2])/2;
           mid2[1] = (list[i].Y[1]+list[i].Y[2])/2;
           mid2[2] = (list[i].Z[1]+list[i].Z[2])/2;
           mid3[0] = (list[i].X[0]+list[i].X[2])/2;
           mid3[1] = (list[i].Y[0]+list[i].Y[2])/2;
           mid3[2] = (list[i].Z[0]+list[i].Z[2])/2;
           output[4*i+0].X[0] = list[i].X[0];
           output[4*i+0].Y[0] = list[i].Y[0];
           output[4*i+0].Z[0] = list[i].Z[0];
           output[4*i+0].X[1] = mid1[0];
           output[4*i+0].Y[1] = mid1[1];
           output[4*i+0].Z[1] = mid1[2];
           output[4*i+0].X[2] = mid3[0];
           output[4*i+0].Y[2] = mid3[1];
           output[4*i+0].Z[2] = mid3[2];
           output[4*i+1].X[0] = list[i].X[1];
           output[4*i+1].Y[0] = list[i].Y[1];
           output[4*i+1].Z[0] = list[i].Z[1];
           output[4*i+1].X[1] = mid2[0];
           output[4*i+1].Y[1] = mid2[1];
           output[4*i+1].Z[1] = mid2[2];
           output[4*i+1].X[2] = mid1[0];
           output[4*i+1].Y[2] = mid1[1];
           output[4*i+1].Z[2] = mid1[2];
           output[4*i+2].X[0] = list[i].X[2];
           output[4*i+2].Y[0] = list[i].Y[2];
           output[4*i+2].Z[0] = list[i].Z[2];
           output[4*i+2].X[1] = mid3[0];
           output[4*i+2].Y[1] = mid3[1];
           output[4*i+2].Z[1] = mid3[2];
           output[4*i+2].X[2] = mid2[0];
           output[4*i+2].Y[2] = mid2[1];
           output[4*i+2].Z[2] = mid2[2];
           output[4*i+3].X[0] = mid1[0];
           output[4*i+3].Y[0] = mid1[1];
           output[4*i+3].Z[0] = mid1[2];
           output[4*i+3].X[1] = mid2[0];
           output[4*i+3].Y[1] = mid2[1];
           output[4*i+3].Z[1] = mid2[2];
           output[4*i+3].X[2] = mid3[0];
           output[4*i+3].Y[2] = mid3[1];
           output[4*i+3].Z[2] = mid3[2];
       }
       return output;
   }


   void DrawSphere()
   {
       int recursionLevel = 3;
       Triangle t;
       t.X[0] = 1;
       t.Y[0] = 0;
       t.Z[0] = 0;
       t.X[1] = 0;
       t.Y[1] = 1;
       t.Z[1] = 0;
       t.X[2] = 0;
       t.Y[2] = 0;
       t.Z[2] = 1;
       std::vector<Triangle> list;
       list.push_back(t);
       for (int r = 0 ; r < recursionLevel ; r++)
       {
           list = SplitTriangle(list);
       }

       // really draw `
       for (int octent = 0 ; octent < 8 ; octent++)
       {
           glPushMatrix();
           glRotatef(90*(octent%4), 1, 0, 0);
           if (octent >= 4)
               glRotatef(180, 0, 0, 1);
           glBegin(GL_TRIANGLES);
           for (unsigned int i = 0 ; i < list.size() ; i++)
           {
               for (int j = 0 ; j < 3 ; j++)
               {
                   double ptMag = sqrt(list[i].X[j]*list[i].X[j]+
                                       list[i].Y[j]*list[i].Y[j]+
                                       list[i].Z[j]*list[i].Z[j]);
                   glNormal3f(list[i].X[j]/ptMag, list[i].Y[j]/ptMag, list[i].Z[j]/ptMag);
                   glVertex3f(list[i].X[j]/ptMag, list[i].Y[j]/ptMag, list[i].Z[j]/ptMag);
               }
           }
           glEnd();
           glPopMatrix();
       }
   }

   void Brown(void) { glColor3ub(205, 133, 63); };
   void LightBrown(void) { glColor3ub(245, 222, 179); };
   void DarkBrown(void) { glColor3ub(162, 82, 45); };
   void Pink(void) { glColor3ub(250, 128, 114); };
   void White(void) { glColor3ub(255, 255, 255); };
   void Black(void) { glColor3ub(0, 0, 0); };
   void Grey(void) { glColor3ub(170,170,170); };
   void Red(void) { glColor3ub(255,0,0); };
   void Green(void) { glColor3ub(0,255,0);};
   void Blue(void) { glColor3ub(0,0,255);};

   void color(int co)
   {
   	if (co == 0) Grey();
   	else if (co == 7)
      Red();
    else LightBrown();
   }

   //draw black if 1 and white if 0
   void DrawSquare(int co)
   {
   	color(co);
    glPushMatrix();
     glScalef(sz_square,sz_square,sz_square/5);
     glRotatef(45,0,0,1);
     DrawCube();
    glPopMatrix();
   	//just make a cube of a fixed size
   };

   void DrawStand(void)
   {
   	//just a stand of fixed size for each piece to be on
    glPushMatrix();
     glScalef(sz_stand, sz_stand, sz_stand/3);
     DrawCylinder();
    glPopMatrix();
   }



   void DrawPawn(int co)
   {
    glPushMatrix();
   	 color(co);
     DrawStand();
     glPushMatrix();
      glScalef(sz_pa_body,sz_pa_body,sz_pa_body_ht);
      DrawCylinder();
     glPopMatrix();
     glPushMatrix();
      glTranslatef(0,0,sz_pa_body_ht-(0.5*sz_pa_head));
      glScalef(sz_pa_head,sz_pa_head,sz_pa_head);
      DrawSphere();
     glPopMatrix();
   	//draw a stand
   	//draw a small cylinder that sits on the stand
   	//draw draw a sphere that sits on top of the small cylinder
    glPopMatrix();
   };

   void DrawRook(int co)
   {
   	color(co);
    glPushMatrix();
    glPushMatrix();
     DrawStand();
     glScalef(sz_ro_base,sz_ro_base,sz_ro_ht);
     DrawCylinder();
     //Ahh I can't be all fancy like with a for loop, or I could but its a lot of math
    glPopMatrix();
    glTranslatef(0,0,sz_ro_ht);
    glPushMatrix();
     //glTranslatef(0,0,sz_ro_ht);
      glTranslatef(sz_ro_base-(sz_ro_base/4),0,0);
      glScalef(sz_ro_rampart,sz_ro_rampart,sz_ro_rampart*1.5);
      glRotatef(-45,0,0,1);
      DrawCube();
     glPopMatrix();

     glPushMatrix();
      glTranslate(-(sz_ro_base-(sz_ro_base/4)),0,0);
      glScalef(sz_ro_rampart,sz_ro_rampart,sz_ro_rampart*1.5);
      glRotatef(-45,0,0,1);
      DrawCube();
     glPopMatrix();

     glPushMatrix();
      glTranslate(0,sz_ro_base-(sz_ro_base/4),0);
      glScalef(sz_ro_rampart,sz_ro_rampart,sz_ro_rampart*1.5);
      glRotatef(-45,0,0,1);
      DrawCube();
     glPopMatrix();

     glPushMatrix();
      glTranslate(0,-(sz_ro_base-(sz_ro_base/4)),0);
      glScalef(sz_ro_rampart,sz_ro_rampart,sz_ro_rampart*1.5);
      glRotatef(-45,0,0,1);
      DrawCube();
     glPopMatrix();

     glPopMatrix();
   	//draw a stand
   	//draw a cylinder that sits on top of the stand
   	//make a bunch of cubes for the walls at the top of the rook
   };

   void DrawKnight(int co)
   {
   	color(co);
    glPushMatrix();
     DrawStand();
     glPushMatrix();
      glScalef(sz_kn_base,sz_kn_base,sz_kn_ht);
      DrawCylinder();
     glPopMatrix();

     glPushMatrix();
      glTranslatef(0,0,sz_kn_ht-(0.5*sz_kn_head));
      
      glPushMatrix();
       glScalef(sz_kn_head,sz_kn_head,sz_kn_head);
       DrawSphere();
      glPopMatrix();

      glPushMatrix();
       glScalef(sz_kn_face/2,sz_kn_face*2,sz_kn_face);
       glRotatef(-45,0,0,1);
       if (co == 0)
        glRotatef(-45,0,1,0);
      else
        glRotatef(45,0,1,0);
       DrawCube();
       //glRotatef(-90,0,1,0);
       //DrawCube();
      glPopMatrix();

     glPopMatrix();

    glPopMatrix();
   	//draw a stand
   	//draw a cylinder that sits on top of the stand
   	//draw a sphere on top of the cylinder for the head
   	//draw a horizontal cube 
   	//draw a vertical cube to complete the snout
   };

   void DrawBishop(int co)
   {
   	color(co);
    glPushMatrix();
     DrawStand();
     for (int i = 0; i < bishop_layers; ++i)
     {
      glPushMatrix();
       glTranslatef(0,0,(sz_bishop_ht/bishop_layers)*i);
       glScalef(sz_bishop_base-((sz_bishop_width/bishop_layers)*i),sz_bishop_base-((sz_bishop_width/bishop_layers)*i),sz_bishop_ht/bishop_layers);
       DrawCylinder();
      glPopMatrix();
     }
    glPopMatrix();
   	//draw the stand
   	//draw a small cylinder that sits on top of the stand
   	//make a cone to go up to the top of the head
   };

   void DrawQueen(int co)
   {
   	color(co);
    glPushMatrix();
     DrawStand();
     glPushMatrix();
      glScalef(sz_queen_base,sz_queen_base,sz_queen_ht);
      DrawCylinder();
     glPopMatrix();
     glPushMatrix();
      glTranslatef(0,0,sz_queen_ht-(0.5*sz_queen_head));
      glScalef(sz_queen_head,sz_queen_head,sz_queen_head);
      DrawSphere();
      glPushMatrix();
       glTranslatef(0,0,sz_queen_hat*1.25);
       glScalef(sz_queen_hat,sz_queen_hat,sz_queen_hat*0.5);
       DrawSphere();
      glPopMatrix();
     glPopMatrix();
    glPopMatrix();
   	//draw the stand
   	//draw a cylinder that sits on top of the stand
   	//draw a sphere that sits on top of the cylinder
   	//draw another sphere that sits on top of the other sphere
   };

   void DrawKing(int co)
   {
   	color(co);
    glPushMatrix();
     DrawStand();
     glPushMatrix();
      glScalef(sz_king_base,sz_king_base,sz_king_ht);
      DrawCylinder();
     glPopMatrix();
     glPushMatrix();
      glTranslatef(0,0,sz_king_ht-(0.5*sz_king_head));
      glScalef(sz_king_head,sz_king_head,sz_king_head);
      DrawSphere();
      glPushMatrix();
      glPushMatrix();
       glTranslatef(0,0,sz_king_cross);
       glScalef(sz_king_cross,sz_king_cross/4,sz_king_cross/4);
       glRotatef(45,0,0,1);
       DrawCube();
      glPopMatrix();
      glPushMatrix();
       glTranslatef(0,0,sz_king_cross-(sz_king_cross/2));
       glScalef(sz_king_cross/4,sz_king_cross/4,sz_king_cross);
       glRotatef(45,0,0,1);
       DrawCube();
      glPopMatrix();
      glPopMatrix();
     glPopMatrix();
    glPopMatrix();
   };

   virtual void RenderPiece(vtkRenderer *ren, vtkActor *act)
   {
    RemoveVTKOpenGLStateSideEffects();
    SetupLight();
    glEnable(GL_COLOR_MATERIAL);

    glMatrixMode(GL_MODELVIEW);

    glPushMatrix();
     DrawBoard();
    glPopMatrix();
   }

   void DrawPawns(int co)
   {
    glPushMatrix();
    for (int i = 0; i < 8; i++)
    {
      DrawPawn(co);
      glTranslate(sz_square*0.71*2,0,0);
    }
    glPopMatrix();
   }

   void DrawBackRow(int co)
   {
      //  glTranslate(0,sz_square*0.71*2,0);
    glPushMatrix();
    DrawRook(co);
    glTranslate(sz_square*0.71*2,0,0);
    DrawKnight(co);
    glTranslate(sz_square*0.71*2,0,0);
    DrawBishop(co);
    glTranslate(sz_square*0.71*2,0,0);
    DrawQueen(co);
    glTranslate(sz_square*0.71*2,0,0);
    DrawKing(co);
    glTranslate(sz_square*0.71*2,0,0);
    DrawBishop(co);
    glTranslatef(sz_square*0.71*2,0,0);
    DrawKnight(co);
    glTranslatef(sz_square*0.71*2,0,0);
    DrawRook(co);
    glPopMatrix();
   }

   void DrawPiece(const char *name, int pl)
   {
    if (strcmp(name,"Ki")==0)
      DrawKing(pl);
    else if (strcmp(name,"Bi")==0)
      DrawBishop(pl);
    else if (strcmp(name,"Ro")==0)
      DrawRook(pl);
    else if (strcmp(name,"Kn")==0)
      DrawKnight(pl);
    else if (strcmp(name,"Qu")==0)
      DrawQueen(pl);
    else if (strcmp(name,"Pa")==0)
      DrawPawn(pl);
    else ;
   }

    void DrawBoard()
   {
    //Draw a brown box then make 8x8 grid of alternating spaces on top of the box
    glPushMatrix();
    glPushMatrix();
    DarkBrown();
    glRotatef(45,0,0,1);
    glScalef(sz_board,sz_board,sz_board/4);
    DrawCube();
    glPopMatrix();
    glTranslatef(0,0,sz_board/4);

 glTranslatef( - ( (sz_square*0.71) + ( (sz_square*0.71*2) *3) ), - ( (sz_square*0.71) + ( (sz_square*0.71*2) *3) ) ,0);

    glPushMatrix();
    for (int i = 0; i < 8; i++)
    {
      glPushMatrix();
      for (int j = 0; j <8; j++)
      {
        if (!board->isThreat(i,j))
          DrawSquare((i+j)%2);
        else
          DrawSquare((7));
        glTranslate(sqDist,0,0);
      }
      glPopMatrix();
      glTranslate(0,sqDist,0);
    }
    glPopMatrix();


    glPushMatrix();
    //start drawing the pieces here
    //now I'm here at 1/1 or 0/0
    glTranslatef(0,0,sz_square/5);

    for (int i = 0; i < 8; i++)
      for (int j = 0; j < 8; j++)
      {
        glPushMatrix();
        int co;
        if ((i == board->active[0])&&(j==board->active[1]))
          co = 7;
        else
          co = board->board[i][j]->getPlayerNum();

        glTranslate(sqDist*j,0,0);

        if (board->yoffset[i][j])
        {
          glTranslate(0,board->yoffset[i][j],0);
        }

        if (board->xoffset[i][j])
        {
          glTranslate(board->xoffset[i][j],0,0);
        }

        glTranslate(0,(sqDist*i),0);
        DrawPiece(board->board[i][j]->getName(),co);
        glPopMatrix();
      }

    glPopMatrix();

  

    glPopMatrix();
   };

   void RenderPieces(vtkRenderer *ren, vtkActor *act)
   {   

    RemoveVTKOpenGLStateSideEffects();
    SetupLight();
    glEnable(GL_COLOR_MATERIAL);

    glMatrixMode(GL_MODELVIEW);

    glPushMatrix();
     DrawKnight(0);
     glTranslatef(0,-6,0);
     DrawRook(0);
     glTranslatef(0,-6,0);
     DrawKing(0);
     glTranslatef(0,-6,0);
     DrawQueen(0);
     glTranslatef(0,-6,0);
   	 DrawBishop(0);
     glTranslatef(0,-6,0);
     DrawPawn(0);
     glTranslatef(-6,0,0);
     DrawPawn(1);
     glTranslatef(0,6,0);
     DrawBishop(1);
     glTranslatef(0,6,0);
     DrawQueen(1);
     glTranslate(0,6,0);
     DrawKing(1);
     glTranslate(0,6,0);
     DrawRook(1);
     glTranslate(0,6,0);
     DrawKnight(1);
    glPopMatrix();
   }

};


class vtk441InteractorStyle : public vtkInteractorStyleTrackballCamera
{
        public:
                Board *b;
                Gamemaster *gm;
                double defpos[3];
                bool defset;
                vtkTextActor *act;
                int view;
                static vtk441InteractorStyle *New();

                vtk441InteractorStyle()
                {
                        shouldPick = true;
                        defset = false;
                        view = 1; //this means it is 'white'
                }

                void setActor(vtkTextActor *a){act = a;}
                void setBoard(Board *board) { b = board;}
                void setGM(Gamemaster *gmaster) { gm = gmaster;}

                void setText(const char * text)
                {
                  act->SetInput(text);
                }
                const char * getTurn()
                {
                  int i = gm->getTurn();
                  if (i == 0)
                    return "White to move";
                  else
                    return "Black to move";
                }

                void showCheckmate(int i)
                {
                  if (i == 0)
                    setText("Checkmate!!! Black Wins!!!");
                  else if (i == 1)
                    setText("Checkmate!!! White Wins!!!");
                }

                void showStalemate()
                {
                  setText("Stalemate!!! It's a tie!!!");
                }

                const char *getErr(int e)
                {
                  if (e == -1)
                    return "That's not your piece!";
                  else if (e == -2)
                    return "You must move a piece!";
                  else if (e == -3)
                    return "You can't take your own piece!";
                  else if (e == -4)
                    return "That piece can't move there!";
                  else        
                    return "That would put your king in check!";
                }

                void Animate(int i, int j, int m, int n, vtkRenderWindow *ren)
                {
                  //assuming only thing moving atm is on y axis increasing
                  //sqdist*i at first should be dist
                  if (j == n)
                  {
                    //moving vertically
                    int start = i;
                    int end = m;
                    if (end > start)
                    {
                      //moving up on the y axis
                      double distance = sqDist*abs(end-start);
                      double movedist = distance/frames;
                      b->yoffset[m][n] = -distance;

                      cerr<<"i "<<i<<" j "<<j<<" m "<<m<<" n "<<n<< "distance "<<distance;
                      for (int k = 0; k < frames*(end-start); k++)
                      {
                        std::cerr<<"\nAnimate frame "<<k<<" offset "<<distance/frames;
                        b->yoffset[m][n] += distance/(frames*(end-start));
                        ren->Render();
                      }
                    }
                    else if (end < start)
                    {
                      //moving down on the y axis
                      double distance = sqDist*abs(start-end);
                      double movedist = distance/frames;
                      b->yoffset[m][n] = distance;
                      for(int k = 0; k < frames*abs(start-end); k ++)
                      {
                        b->yoffset[m][n] -= distance/(frames*abs(start-end));
                        ren->Render();
                      }
                    }
                  }
                  else if (i == m)
                  {
                    //moving horizontally
                    int start = j;
                    int end = n;
                    if (end > start)
                    {
                      //moving right on the x axis
                      double distance = sqDist*abs(end-start);
                      double movedist = distance/frames;
                      b->xoffset[m][n] = -distance;
                      for (int k = 0; k < frames*abs(end-start); k++)
                      {
                        b->xoffset[m][n] += distance/(frames*abs(end-start));
                        ren->Render();
                      }
                    }
                    else if (end < start)
                    {
                      //moving left on the x axis
                      double distance = sqDist *abs(start - end);
                      double movedist = distance/frames;
                      b->xoffset[m][n] = distance;
                      for (int k = 0; k < frames*abs(start-end); k++)
                      {
                        b->xoffset[m][n] -= distance/(frames*abs(start-end));
                        ren->Render();
                      }
                    }
                  }
                  else if ((i>m)&&(j>n))
                  {
                    //traveling down left
                    int xstart = j;
                    int ystart = i;
                    int xend = n;
                    int yend = m;
                    double xdist = sqDist*abs(xend-xstart);
                    double ydist = sqDist*abs(yend-ystart);
                    int avg = (abs(xend-xstart)+abs(yend-ystart))/2;
                    b->xoffset[m][n] = xdist;
                    b->yoffset[m][n] = ydist;
                    for(int k = 0; k < frames*avg; k++)
                    {
                      b->xoffset[m][n]-=xdist/(frames*avg);
                      b->yoffset[m][n]-=ydist/(frames*avg);
                      ren->Render();
                    }
                  }
                  else if ((i>m)&&(j<n))
                  {
                    //traveling down right
                    int xstart = j;
                    int ystart = i;
                    int xend = n;
                    int yend = m;
                    double xdist = sqDist*abs(xend-xstart);
                    double ydist = sqDist*abs(yend-ystart);
                    int avg = (abs(xend-xstart)+abs(yend-ystart))/2;
                    b->xoffset[m][n] = -xdist;
                    b->yoffset[m][n] = ydist;
                    for(int k = 0; k < frames*avg; k++)
                    {
                      b->xoffset[m][n]+=xdist/(frames*avg);
                      b->yoffset[m][n]-=ydist/(frames*avg);
                      ren->Render();
                    }
                  }
                  else if ((i<m)&&(j>n))
                  {
                    //traveling up left
                    int xstart = j;
                    int ystart = i;
                    int xend = n;
                    int yend = m;
                    double xdist = sqDist*abs(xend-xstart);
                    double ydist = sqDist*abs(yend-ystart);
                    int avg = (abs(xend-xstart)+abs(yend-ystart))/2;
                    b->xoffset[m][n]=xdist;
                    b->yoffset[m][n]=-ydist;
                    for(int k = 0; k < frames*avg; k++)
                    {
                      b->xoffset[m][n]-=xdist/(frames*avg);
                      b->yoffset[m][n]+=ydist/(frames*avg);
                      ren->Render();
                    }

                  }
                  else if ((i<m)&&(j<n))
                  {
                    //traveling up right
                    int xstart = j;
                    int ystart = i;
                    int xend = n;
                    int yend = m;
                    double xdist = sqDist*abs(xend-xstart);
                    double ydist = sqDist*abs(yend-ystart);
                    int avg = (abs(xend-xstart)+abs(yend-ystart))/2;                    
                    b->xoffset[m][n]=-xdist;
                    b->yoffset[m][n]=-ydist;
                    for (int k = 0; k < frames*avg; k++)
                    {
                      b->xoffset[m][n]+= xdist/(frames*avg);
                      b->yoffset[m][n]+= ydist/(frames*avg);
                      ren->Render();
                    }                    
                  }

                  b->yoffset[m][n] = 0;
                  ren->Render();
                }

  
                void toMove(double *pos,int *move)
                {
                  //so you round
                  cerr<<"  so pos is " << pos[0] << " "<<pos[1]<<endl;


                  move[0] = round(pos[1]);
                  move[1] = round(pos[0]);
                  //you add 11.2 to get rid of the negatives
                  move[0]+=12;
                  move[1]+=12;
                  //you divide by 2.8 cause thats the size (cheater)
                  move[0]= move[0]/2.8;
                  move[1]=move[1]/2.8;
                  //you ceiling to get the int
                  move[0] = ceil(move[0]);
                  move[1] = ceil(move[1]);

                }

                virtual void OnChar()
                {
                        vtkRenderWindowInteractor *rwi = this->Interactor;
                        char pressedChar = rwi->GetKeyCode();
                        vtkRenderWindow *rw = rwi->GetRenderWindow();
                        int x = this->Interactor->GetEventPosition()[0];
                        int y = this->Interactor->GetEventPosition()[1];
                        vtkRenderer *ren = rwi->FindPokedRenderer(x, y);
                        double speed = 10;
                        double zoom = 5;
                        double pos[3];
                        double rot = 7;
                        double angle = 10;
                        pos[0] = ren->GetActiveCamera()->GetPosition()[0];
                        pos[1] = ren->GetActiveCamera()->GetPosition()[1];
                        pos[2] = ren->GetActiveCamera()->GetPosition()[2];
                        if (!defset)
                        {
                          defset = true;
                          defpos[0] = pos[0];
                          defpos[1] = pos[1];
                          defpos[2] = pos[2];
                        }
                        if (pressedChar == 'w')
                               //move camera forward
                          ren->GetActiveCamera()->SetPosition(pos[0],pos[1]+speed,pos[2]);
                        else if (pressedChar == 'a')
                              //move camera left
                          ren->GetActiveCamera()->SetPosition(pos[0]-speed,pos[1],pos[2]);
                        else if (pressedChar == 's')
                              //move camera down
                          ren->GetActiveCamera()->SetPosition(pos[0],pos[1]-speed,pos[2]);
                        else if (pressedChar == 'd')
                              //move camera down
                          ren->GetActiveCamera()->SetPosition(pos[0]+speed,pos[1],pos[2]);
                        else if (pressedChar == 'z')
                              //'zoom in'
                          ren->GetActiveCamera()->SetPosition(pos[0],pos[1],pos[2]-zoom);
                        else if (pressedChar == 'c')
                              //'zoom out'
                          ren->GetActiveCamera()->SetPosition(pos[0],pos[1],pos[2]+zoom);
                        else if (pressedChar == 'x')
                              //'reset to default'
                          ren->GetActiveCamera()->SetPosition(defpos[0],defpos[1],defpos[2]);
                        else if (pressedChar == 'p')
                          setText(getTurn());
                        else if (pressedChar == 'i')
                              //invert to see from other players pov
                          view=view*-1;
                          ren->GetActiveCamera()->SetViewUp(0, view, 0 );
                        rw->Render();
                        vtkInteractorStyleTrackballCamera::OnChar();
                };

                virtual void OnLeftButtonDown()
                {
                        if (!(gm->checkmate && gm->stalemate))
                        {
                        if (shouldPick)
                        {
                                vtkRenderWindowInteractor *rwi = this->Interactor;
                                vtkRenderWindow *rw = rwi->GetRenderWindow();
                                int *size = rw->GetSize();
                                int x = this->Interactor->GetEventPosition()[0];
                                int y = this->Interactor->GetEventPosition()[1];
                                vtkRenderer *ren = rwi->FindPokedRenderer(x, y);
                                double pos[3];
                                pos[0] = 2.0*((double)x/(double)size[0])-1;
                                pos[1] = 2.0*((double)y/(double)size[1])-1;
                                pos[2] = ren->GetZ(x, y);
                                cerr << "Picked on " << x << ", " << y << endl;
                                cerr << " = " << pos[0] << ", " << pos[1] << ", " << pos[2] << endl;
                                ren->ViewToWorld(pos[0], pos[1], pos[2]);
                                cerr << " converted to " << pos[0] << ", " << pos[1] << ", " << pos[2] << endl;
                                int move[2]; 
                                toMove(pos,move);
                                if ((move[0] < 0 || move[0] > 7 ) || (move[1] < 0 || move[1] > 7 ))
                                {
                                  cerr <<" Input out of range, invalid"<<endl;
                                  setText("Input out of range, invalid");
                                }
                                else
                                {
                                  cerr << " is square " << move[0] << ", " << move[1] << endl;
                                  b->displayBoard();
                                  cerr << "    is piece " << b->board[move[0]][move[1]]->getName() <<endl;

                                  char MSG[50];
                                  sprintf(MSG,"Selected: %s at %d,%d",b->board[move[0]][move[1]]->getName(),move[0],move[1]);
                                  act->SetInput(MSG);

                                  if ( (!gm->s1) && (b->board[move[0]][move[1]]->getPlayerNum() == gm->t) )
                                  {
                                    //gm->sq1 = move;
                                    gm->sq1[0] = move[0];
                                    gm->sq1[1] = move[1];
                                    gm->s1 = true;
                                    b->setActive(move[0],move[1]);
                                    b->setThreats(b->board[move[0]][move[1]]->getThreats(move[0],move[1],b->board));
                                  }
                                  else if (!gm->s2)
                                  {
                                    cerr << "moving " << b->board[gm->sq1[0]][gm->sq1[1]]->getName() << 
                                                " to " << b->board[move[0]][move[1]]->getName();
                                    //gm->sq2 = move;
                                    gm->sq2[0] = move[0];
                                    gm->sq2[1] = move[1];
                                    gm->s2 = true;
                                    int e = gm->makeMove();
                                    if (e <= -1)
                                    {
                                      cerr<<"bad move, try again "<<gm->getPlayer()<<endl;; 
                                      setText(getErr(e));
                                    }
                                    else
                                    {
                                      Animate(gm->sq1[0],gm->sq1[1],gm->sq2[0],gm->sq2[1],rw);
                                      gm->changeTurn(); //have the player turns be in a length 2 array so you can just -1
                                      gm->isCheckmate();
                                      setText(getTurn());
                                    }
                                    gm->s1 = false;
                                    gm->s2 = false;
                                    b->setActive(9,9);
                                    b->clearThreats();
                                  }

                                  cerr<<"your move "<<gm->getPlayer()<<endl;
                                  
                                  if (gm->checkmate)
                                  {
                                    shouldPick = false;
                                    showCheckmate(gm->getTurn());
                                  }
                                  else if (gm->stalemate)
                                  {
                                    shouldPick = false;
                                    showStalemate();
                                  }
                                 // rw->Render();
                                 // shouldPick = false;
                                }
                        }
                      }
                      else
                      {
                        cerr<<"checkmate or stalemate doofus. GAMEOVER"<<endl;
                        shouldPick = false;
                      }
                        vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
                      
                };

        private:
                bool shouldPick;
};
vtkStandardNewMacro(vtk441InteractorStyle);



vtkStandardNewMacro(vtk441MapperPart3);


class Display_gui : public Display
{
	public:
		Board *b;
    Gamemaster *gm;
    vtkSmartPointer<vtkSphereSource> sphere;
    vtkSmartPointer<vtk441MapperPart3> win3Mapper;
    vtkSmartPointer<vtkActor> win3Actor;
    vtkSmartPointer<vtkRenderer> ren3;
    vtkSmartPointer<vtkRenderWindow> renWin;
    vtkSmartPointer<vtkRenderWindowInteractor> iren;
    vtkSmartPointer<vtkLight> light;
    vtk441InteractorStyle *style;
    //(vtkInteractorStyle *)iren;
		Display_gui(Gamemaster *gamemaster) : Display() 
		{
			b = &gamemaster->board;
      gm = gamemaster;
// Dummy input so VTK pipeline mojo is happy.
  //
  sphere = vtkSmartPointer<vtkSphereSource>::New();
  sphere->SetThetaResolution(100);
  sphere->SetPhiResolution(50);

  win3Mapper = vtkSmartPointer<vtk441MapperPart3>::New();
  win3Mapper->SetInputConnection(sphere->GetOutputPort());
  win3Mapper->setBoard(b);

  win3Actor = vtkSmartPointer<vtkActor>::New();
  win3Actor->SetMapper(win3Mapper);

  ren3 = vtkSmartPointer<vtkRenderer>::New();

  renWin = vtkSmartPointer<vtkRenderWindow>::New();
  renWin->AddRenderer(ren3);
  ren3->SetViewport(0, 0, 1, 1);

  iren = vtkSmartPointer<vtkRenderWindowInteractor>::New();
  iren->SetRenderWindow(renWin);

  style = vtk441InteractorStyle::New();
  style->setGM(gm);
  style->setBoard(b);
  iren->SetInteractorStyle(style);

  // Add the actors to the renderer, set the background and size.
  //
  bool doWindow3 = true;
  if (doWindow3)
      ren3->AddActor(win3Actor);
  ren3->SetBackground(0.3, 0.3, 0.3);
  renWin->SetSize(500, 500);


  vtkTextActor *tx1 = vtkTextActor::New();
  tx1->SetInput("White to move");
  ren3->AddActor(tx1);
  style->setActor(tx1);

  // Set up the lighting.
  //
  light = vtkSmartPointer<vtkLight>::New();
  light->SetFocalPoint(1.875,0.6125,0);
  light->SetPosition(0.875,1.6125,1);
  ren3->AddLight(light);

  ren3->GetActiveCamera()->SetFocalPoint(-10,0,-5);
  ren3->GetActiveCamera()->SetPosition(35,-10,70);
  ren3->GetActiveCamera()->SetViewUp(0,1,0);
  ren3->GetActiveCamera()->SetClippingRange(20, 120);
  ren3->GetActiveCamera()->SetDistance(70);

  // This starts the event loop and invokes an initial render.
  //
  ((vtkInteractorStyle *)iren->GetInteractorStyle())->SetAutoAdjustCameraClippingRange(0);
  //iren->GetInteractorStyle()->SetAutoAdjustCameraClippingRange(0);
  cerr<<"initializing"<<endl;
  iren->Initialize();
  iren->Start();
		}
		virtual void update()
    {
      //iren->Initialize();
      //iren->Start();
      //renWin->Render();
      std::cerr<<"BEFORE iren->start()";
      //iren->Start();
      renWin->Render();
      std::cerr<<"AFTER iren->start()";
    }

    int parseNum(char a)
    {
      int i;
      if (a == 'a') i = 0;
      else if (a == 'b') i = 1;
      else if (a == 'c') i = 2;
      else if (a == 'd') i = 3;
      else if (a == 'e') i = 4;
      else if (a == 'f') i = 5;
      else if (a == 'g') i = 6;
      else if (a == 'h') i = 7;
      else i = -1;

      return i;
    }

    
		virtual void getMove(int *i,int *j,int *m,int *n)
    {
      std::cerr<<"reinitializing "<<iren->GetEnabled()<<" "<<iren->GetInitialized();
     // iren->ReInitialize();      
      //iren->Start();
      //iren->Initialize();
      
      char x, s; int y, t;
      std::cout<<endl<<"Starting X ";
      std::cin>>x;
      std::cout<<"   Starting Y ";
      std::cin>>y;
      std::cout<<endl<<"Ending X ";
      std::cin>>s;
      std::cout<<" Ending Y ";
      std::cin>>t;

      *j = parseNum(x);
      *i = y-1;
      *n = parseNum(s);
      *m = t-1;
    }
};


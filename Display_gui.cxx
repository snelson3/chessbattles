#include "Display.cxx"
#include "Board.C"
#include <iostream>


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



class vtk441InteractorStyle : public vtkInteractorStyleTrackballCamera
{
        public:
                static vtk441InteractorStyle *New();

                vtk441InteractorStyle()
                {
                        shouldPick = false;
                }

                void toMove(double *pos,int *move)
                {


                  //so you round
                  cerr<<"  so pos is " << pos[0] << pos[1]<<endl;
                  move[0] = round(pos[0]);
                  move[1] = round(pos[1]);
                  //you add 12 to get rid of the negatives
                  move[0]+=12;
                  move[1]+=12;
                  //you divide by 3 cause thats the size (cheater)
                  move[0]= move[0]/3;
                  move[1]=move[1]/3;
                  //you ceiling to get the int
                  move[0] = ceil(move[0]);
                  move[1] = ceil(move[1]);
                }

                virtual void OnChar()
                {
                        vtkRenderWindowInteractor *rwi = this->Interactor;
                        char pressedChar = rwi->GetKeyCode();
                        if (pressedChar == 'p')
                        {
                                cerr << "Should pick!" << endl;
                                shouldPick = true;
                        }
                        vtkInteractorStyleTrackballCamera::OnChar();
                };

                virtual void OnLeftButtonDown()
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
                                cerr << " is square " << move[0] << ", " << move[1] << endl;
                                if ((move[0] < 0 || move[0] > 7 ) || (move[1] < 0 || move[1] > 7 ))
                                  cerr <<" Input out of range, invalid"<<endl;
                                else
                                  shouldPick = false;
                        }
                        vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
                };

        private:
                bool shouldPick;
};
vtkStandardNewMacro(vtk441InteractorStyle);



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
   float sz_stand, sz_board, sz_square;
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
    sz_board = 20;
    //sz_square = sz_board/9;
    sz_square = sz_board/9;
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
       glTranslatef(0,0,sz_queen_hat*0.25);
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
    glScalef(sz_board,sz_board,sz_board/2);
    DrawCube();
    glPopMatrix();
    glTranslatef(0,0,sz_board/2);

    glPushMatrix();
     glTranslatef(-sz_square*0.71,sz_square*0.71,0);

     for (int i = 0; i < 4; i++)
     {
      glPushMatrix();
       for (int j = 0; j < 4; j++)
       {
        glTranslatef(sz_square*0.71*2,0,0);
        DrawSquare((i+j+1) % 2);
       }
       glPopMatrix();
       glTranslatef(0,sz_square*0.71*2,0);
     }
    glPopMatrix();

    glPushMatrix();
     glTranslatef(-sz_square*0.71,-(sz_square*0.71),0);
     for (int i = 0; i < 4; i++)
     {
      glPushMatrix();
      for (int j = 0; j < 4; j++)
      {
        glTranslatef(sz_square*0.71*2,0,0);
        DrawSquare((i+j) % 2);
      }
      glPopMatrix();
      glTranslate(0,-(sz_square*0.71*2),0);
     }
    glPopMatrix();

    glPushMatrix();
     glTranslatef((sz_square*0.71),sz_square*0.71,0);
     for (int i = 0; i < 4; i++)
     {
      glPushMatrix();
      for (int j = 0; j < 4; j++)
      {
        glTranslatef(-(sz_square*0.71)*2,0,0);
        DrawSquare((i+j) % 2);
      }
      glPopMatrix();
      glTranslate(0,sz_square*0.71*2,0);
     }
    glPopMatrix();

    glPushMatrix();
     glTranslatef((sz_square*0.71),-(sz_square*0.71),0);
     for (int i = 0; i < 4; i++)
     {
      glPushMatrix();
      for (int j = 0; j < 4; j++)
      {
        glTranslate(-(sz_square*0.71)*2,0,0);
        DrawSquare((i+j+1) % 2);
      }
      glPopMatrix();
      glTranslate(0,-(sz_square*0.71*2),0);
     }
    glPopMatrix();

    glPushMatrix();
    //start drawing the pieces here
    glTranslatef( - ( (sz_square*0.71) + ( (sz_square*0.71*2) *3) ), - ( (sz_square*0.71) + ( (sz_square*0.71*2) *3) ) ,0);
    
    //now I'm here at 1/1 or 0/0
    for (int i = 0; i < 8; i++)
      {
        glPushMatrix();
        for (int j = 0; j < 8; j++)
        {
          DrawPiece(board->board[i][j]->getName(), board->board[i][j]->getPlayerNum());
          glTranslate(sz_square*0.71*2,0,0);
        }
        glPopMatrix();
        glTranslate(0,sz_square*0.71*2,0);
      }




    // glPushMatrix();
    // DrawBackRow(0);
    // glPopMatrix();
    // glTranslate(0,sz_square*0.71*2,0);
    // glPushMatrix();
    // DrawPawns(0);
    // glPopMatrix();
    // for (int i = 0; i < 5; i++)
    //   glTranslate(0,sz_square*0.71*2,0);
    // glPushMatrix();
    // DrawPawns(1);
    // glPopMatrix();
    // glTranslate(0,sz_square*0.71*2,0);
    // DrawBackRow(1);
    // glPopMatrix();


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

vtkStandardNewMacro(vtk441MapperPart3);


class Display_gui : public Display
{
	public:
		Board *b;
    vtkSmartPointer<vtkSphereSource> sphere;
    vtkSmartPointer<vtk441MapperPart3> win3Mapper;
    vtkSmartPointer<vtkActor> win3Actor;
    vtkSmartPointer<vtkRenderer> ren3;
    vtkSmartPointer<vtkRenderWindow> renWin;
    vtkSmartPointer<vtkRenderWindowInteractor> iren;
    vtkSmartPointer<vtkLight> light;
    vtk441InteractorStyle *style;
    //(vtkInteractorStyle *)iren;
		Display_gui(Board *board) : Display() 
		{
			b = board;
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
  iren->SetInteractorStyle(style);

  // Add the actors to the renderer, set the background and size.
  //
  bool doWindow3 = true;
  if (doWindow3)
      ren3->AddActor(win3Actor);
  ren3->SetBackground(0.3, 0.3, 0.3);
  renWin->SetSize(500, 500);

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
  iren->Initialize();
  //iren->Start();
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


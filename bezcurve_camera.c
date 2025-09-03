/*
 * Copyright (c) 1993-1997, Silicon Graphics, Inc.
 * ALL RIGHTS RESERVED 
 * Permission to use, copy, modify, and distribute this software for 
 * any purpose and without fee is hereby granted, provided that the above
 * copyright notice appear in all copies and that both the copyright notice
 * and this permission notice appear in supporting documentation, and that 
 * the name of Silicon Graphics, Inc. not be used in advertising
 * or publicity pertaining to distribution of the software without specific,
 * written prior permission. 
 *
 * THE MATERIAL EMBODIED ON THIS SOFTWARE IS PROVIDED TO YOU "AS-IS"
 * AND WITHOUT WARRANTY OF ANY KIND, EXPRESS, IMPLIED OR OTHERWISE,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY OR
 * FITNESS FOR A PARTICULAR PURPOSE.  IN NO EVENT SHALL SILICON
 * GRAPHICS, INC.  BE LIABLE TO YOU OR ANYONE ELSE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY
 * KIND, OR ANY DAMAGES WHATSOEVER, INCLUDING WITHOUT LIMITATION,
 * LOSS OF PROFIT, LOSS OF USE, SAVINGS OR REVENUE, OR THE CLAIMS OF
 * THIRD PARTIES, WHETHER OR NOT SILICON GRAPHICS, INC.  HAS BEEN
 * ADVISED OF THE POSSIBILITY OF SUCH LOSS, HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE
 * POSSESSION, USE OR PERFORMANCE OF THIS SOFTWARE.
 * 
 * US Government Users Restricted Rights 
 * Use, duplication, or disclosure by the Government is subject to
 * restrictions set forth in FAR 52.227.19(c)(2) or subparagraph
 * (c)(1)(ii) of the Rights in Technical Data and Computer Software
 * clause at DFARS 252.227-7013 and/or in similar or successor
 * clauses in the FAR or the DOD or NASA FAR Supplement.
 * Unpublished-- rights reserved under the copyright laws of the
 * United States.  Contractor/manufacturer is Silicon Graphics,
 * Inc., 2011 N.  Shoreline Blvd., Mountain View, CA 94039-7311.
 *
 * OpenGL(R) is a registered trademark of Silicon Graphics, Inc.
 */

/*  bezcurve.c			
 *  This program uses evaluators to draw a Bezier curve.
 */
#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>

// Pontos de controle para o caminho da câmera
GLfloat cameraPath[4][3] = {
    {0.0, 6.0, 10.0},  // Ponto inicial (P1)
    {-15.0, 0.0, 0.0}, // Ponto de controle (P2)
    {0.0, -6.0, -10.0},// Ponto de controle (P3)
    {15.0, 0.0, 0.0}  // Ponto final (P4)
};

// Variável global para controlar o tempo/parâmetro da curva (t)
GLfloat t = 0.0;

void getPointOnBezier(float u, GLfloat *pos)
{
    // Polinômios de Bernstein
    float b0 = pow(1 - u, 3);
    float b1 = 3 * u * pow(1 - u, 2);
    float b2 = 3 * pow(u, 2) * (1 - u);
    float b3 = pow(u, 3);

    // Calcula as coordenadas x, y, z do ponto
    // pos[0] = X, pos[1] = Y, pos[2] = Z
    pos[0] = b0 * cameraPath[0][0] + b1 * cameraPath[1][0] + b2 * cameraPath[2][0] + b3 * cameraPath[3][0];
    pos[1] = b0 * cameraPath[0][1] + b1 * cameraPath[1][1] + b2 * cameraPath[2][1] + b3 * cameraPath[3][1];
    pos[2] = b0 * cameraPath[0][2] + b1 * cameraPath[1][2] + b2 * cameraPath[2][2] + b3 * cameraPath[3][2];
}

void init(void)
{
   glClearColor(0.0, 0.0, 0.0, 0.0);
   // Habilita o teste de profundidade para renderização 3D correta
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);
   glEnable(GL_COLOR_MATERIAL);
}

void drawScene(void) {
   // Chaleira vermelha no centro.
   glColor3f(1.0, 0.0, 0.0);
   glutSolidTeapot(1.0);

   // Cubo verde
   glPushMatrix();
   glTranslatef(3.0, 0.0, 0.0);
   glColor3f(0.0, 1.0, 0.0);
   glutSolidCube(1.0);
   glPopMatrix();

   // Esfera azul
   glPushMatrix();
   glTranslatef(-3.0, 0.0, 0.0);
   glColor3f(0.0, 0.0, 1.0);
   glutSolidSphere(0.7, 20, 20);
   glPopMatrix();
}

void display(void){
   GLfloat cameraPos[3];

   // Limpa os buffers
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   // Calcula a posição atual da câmera na curva de Bézier
   getPointOnBezier(t, cameraPos);

   // Configura a matriz de visualização
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

   // Posiciona a câmera usando gluLookAt
   // Câmera está em 'cameraPos', olhando para a origem (0,0,0)
   // O vetor "up" é (0,1,0)
   gluLookAt(cameraPos[0], cameraPos[1], cameraPos[2],
             0.0, 0.0, 0.0,
             0.0, 1.0, 0.0);

   // Desenha os objetos na cena
   drawScene();

   // Troca os buffers para exibir a cena (essencial para animação)
   glutSwapBuffers();
}

void reshape(int w, int h)
{
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   // Usa projeção de perspectiva em vez de ortográfica
   gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 1.0, 30.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

void update(void)
{
    // Incrementa o parâmetro 't'
    t += 0.0002;

    // Se a animação chegou ao fim da curva, reinicia
    if (t > 1.0) {
        t = 0.0;
    }

    // Pede para a GLUT redesenhar a tela
    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
   switch (key) {
      case 27:
         exit(0);
         break;
   }
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   // Modo de display com buffer duplo para animação suave e buffer de profundidade
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize (800, 600);
   glutInitWindowPosition (100, 100);
   glutCreateWindow ("Animacao de Camera com Bezier");
   init();
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   // Registra a função de 'update' para ser chamada quando o programa estiver ocioso
   glutIdleFunc(update);
   glutMainLoop();
   return 0;
}
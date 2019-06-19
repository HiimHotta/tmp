/****************************************************************
    Nome: Daniel Yoshio Hotta
    NUSP: 9922700
    Ao preencher esse cabeçalho com o meu nome e o meu número USP,
    declaro que todas as partes originais desse exercício programa (EP)
    foram desenvolvidas e implementadas por mim e que portanto não 
    constituem desonestidade acadêmica ou plágio.
    Declaro também que sou responsável por todas as cópias desse
    programa e que não distribui ou facilitei a sua distribuição.
    Estou ciente que os casos de plágio e desonestidade acadêmica
    serão tratados segundo os critérios divulgados na página da 
    disciplina.
    Entendo que EPs sem assinatura devem receber nota zero e, ainda
    assim, poderão ser punidos por desonestidade acadêmica.
    Descrição de ajuda ou indicação de fonte:
    Se for o caso, descreva a seguir 'bugs' e limitações do seu programa:
****************************************************************/

/******************************************************************************
 *  Compilation:  javac-algs4 Board.java
 *  Execution:    java Board 
 *
 *  Por convencao, 0 é o quadrado vazio. Esse programa eh mais "parrudo" que o 
 * o Solver ou era a essa a ideia, uma vez que não faz sentido ficar acessando 
 * o caching por lá e tentar resolver tudo nessa classe.
 *
 *  % java Board
 *
 *  Teste fechado.
 *
 ******************************************************************************/
import java.awt.Color;
import java.lang.IllegalArgumentException;
import java.lang.Math;

import edu.princeton.cs.algs4.Picture;

public class SeamCarver {

   int w, h;
   Picture picture;

   // create a seam carver object based on the given picture
   public SeamCarver(Picture picture) {
      this.picture = picture;
      h = picture.height ();
      w = picture.width ();
   }

   // current picture
   public Picture picture() {
      return picture;
   }

   // width of current picture
   public int width() {
      return w;
   }

   // height of current picture
   public int height() {
      return h;
   }

   // energy of pixel at column x and row y
   public double energy(int x, int y) {
      if (x < 0 || x > w)
         throw new IllegalArgumentException ("energy (): X-coordinate outside its prescribed range.\n");

      if (y < 0 || y > h)
         throw new IllegalArgumentException ("energy (): Y-coordinate outside its prescribed range.\n");

      Color n = getColor (x, y, 'n');  //north direction
      Color s = getColor (x, y, 's');  //south direction
      Color e  = getColor (x, y, 'e'); //east  direction
      Color w  = getColor (x, y, 'w'); //west  direction

      return Math.sqrt (
         (e.getRed () - w.getRed ()) ^ 2 + (e.getGreen () - w.getGreen ()) ^ 2 + (e.getBlue () + w.getBlue ()) ^ 2
       + (n.getRed () - s.getRed ()) ^ 2 + (n.getGreen () - s.getGreen ()) ^ 2 + (n.getBlue () + s.getBlue ()) ^ 2);
   }

   private Color getColor (int x, int y, char c) {
      if (c == 'n' && y - 1 < 0) 
         return picture.get (x, h - 1);

      else if (c == 'n')
         return picture.get (x, y - 1);

      else if (c == 's' && y + 1 >= h)
         return picture.get (x, 0);

      else if (c == 's')
         return picture.get (x, y + 1);

      else if (c == 'e' && x + 1 >= w)
         return picture.get (0, y);

      else if (c == 'e')
         return picture.get (x + 1, y);

      else if (c == 'w' && x - 1 < 0)
         return picture.get (w - 1, y);

      else if (c == 'w')
         return picture.get (x - 1, y);

      return picture.get (x, y);
   }

   // sequence of indices for horizontal seam
   public int[] findHorizontalSeam () {
      int aux[] = findH (0, 0, new int[w + 1]);
      for (int i = 1; i < h; i++) {
         int[] tmp = findH (i, 0, new int[w + 1]);

         if (tmp[w] < aux[w])
            aux = tmp;
      }

      return aux;
   }

   private int[] findH (int m, int n, int[] path) {
      if (n >= w)
         return path;

      path[n] = m;
      path[w] += energy (m, n);

      if (m == 0) {
         int[] m0 = findH (0, n, path);
         int[] m1 = findH (1, n, path);

         return (m0[w] > m1[w]) ? m1 : m0;
      }

      else if (m == w - 1) {
         int[] aux1 = findH (m - 1, n, path);
         int[] aux2 = findH (m    , n, path);

         return (aux1[w] > aux2[w]) ? aux2 : aux1;
      }

      int[] aux1 = findH (m - 1, n, path);
      int[] aux2 = findH (m    , n, path);
      int[] aux3 = findH (m + 1, n, path);

      if (aux1[w] < aux2[w] && aux1[w] < aux3[w]){
         return aux1;
      }

      return (aux2[w] < aux3[w]) ? aux2 : aux3;
   }

   // sequence of indices for vertical seam
   public int[] findVerticalSeam() {
      int aux[] = findV (0, 0, new int[h+ 1]);
      for (int i = 1; i < w; i++) {
         int[] tmp = findV (i, 0, new int[h + 1]);

         if (tmp[h] < aux[h])
            aux = tmp;
      }

      return aux;
   }

   private int[] findV (int m, int n, int[] path) {
      if (n >= h)
         return path;

      path[n] = m;
      path[h] += energy (m, n);

      if (m == 0) {
         int[] m0 = findV (0, n, path);
         int[] m1 = findV (1, n, path);
         return (m0[h] > m1[h]) ? m1 : m0;
      }

      else if (m == h - 1) {
         int[] aux1 = findV (m - 1, n, path);
         int[] aux2 = findV (m    , n, path);
         return (aux1[h] > aux2[h]) ? aux2 : aux1;
      }

      int[] aux1 = findV (m - 1, n, path);
      int[] aux2 = findV (m    , n, path);
      int[] aux3 = findV (m + 1, n, path);

      if (aux1[h] < aux2[h] && aux1[h] < aux3[h])
         return aux1;

      return (aux2[h] < aux3[h]) ? aux2 : aux3;
   }


   // remove horizontal seam from current picture
   public void removeHorizontalSeam(int[] seam) {
      if (seam == null)
         throw new IllegalArgumentException ("removeHorizontalSeam (): int[] seam is NULL.");
   }

   // remove vertical seam from current picture
   public void removeVerticalSeam(int[] seam) {
      if (seam == null)
         throw new IllegalArgumentException ("removeHorizontalSeam (): int[] seam is NULL.");
   }

   //  unit testing (required)
   public static void main(String[] args) {

   }

}
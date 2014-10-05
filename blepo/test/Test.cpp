/* 
 * Copyright (c) 2004,2005 Stan Birchfield.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "stdafx.h"
#include "../src/blepo.h"
//#include "AlignmentFns.h"

using namespace blepo;

void Error()
{
  printf("FAILED ================================\n");
  assert(0);
  printf("Press return\n");
  getchar();
  exit(1);
}

void Verify(bool b)
{
  if (!b)  Error();
}

int main(int argc, char* argv[])
{
  
  {
    printf("Testing basic image stuff...\n");
    // ImgGray
    Verify(ImgGray::NBITS_PER_PIXEL == 8);
    Verify(ImgGray::NCHANNELS == 1);
    Verify(ImgGray::MIN_VAL == 0);
    Verify(ImgGray::MAX_VAL == 255);
    Verify(8 * sizeof(ImgGray::Pixel) == ImgGray::NBITS_PER_PIXEL);
    // ImgBinary
    Verify(ImgBinary::NBITS_PER_PIXEL == 1);
    Verify(ImgBinary::NCHANNELS == 1);
    Verify(ImgBinary::MIN_VAL == 0);
    Verify(ImgBinary::MAX_VAL == 1);
    // ImgBgr
    Verify(ImgBgr::NBITS_PER_PIXEL == 24);
    Verify(ImgBgr::NCHANNELS == 3);
    Verify(ImgBgr::MIN_VAL == Bgr(0,0,0));
    Verify(ImgBgr::MAX_VAL == Bgr(0xFF, 0xFF, 0xFF));
    Verify(8 * sizeof(ImgBgr::Pixel) == ImgBgr::NBITS_PER_PIXEL);
    // ImgInt
    Verify(ImgInt::NCHANNELS == 1);
    Verify(8 * sizeof(ImgInt::Pixel) == ImgInt::NBITS_PER_PIXEL);
    // ImgFloat
    Verify(ImgFloat::NCHANNELS == 1);
    Verify(8 * sizeof(ImgFloat::Pixel) == ImgFloat::NBITS_PER_PIXEL);
  }

  {
    printf("Testing Min and Max...\n");
    Verify(blepo_ex::Min(3,5) == 3);
    Verify(blepo_ex::Max(3,5) == 5);
  }

  {
    printf("Testing pixel get/set...\n");
    int width = blepo_ex::GetRand(20, 100);
    int height = blepo_ex::GetRand(20, 100);
    int x = blepo_ex::GetRand(width);
    int y = blepo_ex::GetRand(height);
    {
      ImgGray img(height, width);
      ImgGray::Pixel pix = blepo_ex::GetRand(ImgGray::MIN_VAL, ImgGray::MAX_VAL+1);
      img(y, x) = pix;
      Verify(img(y, x) == pix);
    }
    {
      ImgInt img(height, width);
      ImgInt::Pixel pix = blepo_ex::GetRand(-1000, 1000);  // range can't be greater than RAND_MAX
      img(y, x) = pix;
      Verify(img(y, x) == pix);
    }
    {
      ImgFloat img(height, width);
      int n1 = blepo_ex::GetRand(-1000, 1000);  // range can't be greater than RAND_MAX
      int n2 = blepo_ex::GetRand(-1000, 1000);  // range can't be greater than RAND_MAX
      ImgFloat::Pixel pix = static_cast<float>(n1) / static_cast<float>(n2);
      img(y, x) = pix;
      Verify(img(y, x) == pix);
    }
    {
      ImgBgr img(height, width);
      int b = blepo_ex::GetRand(0, 256);
      int g = blepo_ex::GetRand(0, 256);
      int r = blepo_ex::GetRand(0, 256);
      ImgBgr::Pixel pix(b,g,r);
      img(y, x) = pix;
      Verify(img(y, x) == pix);
    }
  }

  {
    printf("Testing ComputeConnectedComponents4...\n");
    ImgGray img(4, 5);
    ImgGray::Pixel a=80, b=32, c=131;
    img(0,0) = a;  img(1,0) = a;  img(2,0) = a;  img(3,0) = b;
    img(0,1) = b;  img(1,1) = b;  img(2,1) = a;  img(3,1) = b;
    img(0,2) = b;  img(1,2) = a;  img(2,2) = a;  img(3,2) = b;
    img(0,3) = b;  img(1,3) = c;  img(2,3) = c;  img(3,3) = b;
    img(0,4) = b;  img(1,4) = b;  img(2,4) = b;  img(3,4) = b;
    ImgInt cc;
    ConnectedComponents4(img, &cc);
    Verify(cc(0,0)==0);  Verify(cc(1,0)==0);  Verify(cc(2,0)==0);  Verify(cc(3,0)==1);
    Verify(cc(0,1)==1);  Verify(cc(1,1)==1);  Verify(cc(2,1)==0);  Verify(cc(3,1)==1);
    Verify(cc(0,2)==1);  Verify(cc(1,2)==0);  Verify(cc(2,2)==0);  Verify(cc(3,2)==1);
    Verify(cc(0,3)==1);  Verify(cc(1,3)==2);  Verify(cc(2,3)==2);  Verify(cc(3,3)==1);
    Verify(cc(0,4)==1);  Verify(cc(1,4)==1);  Verify(cc(2,4)==1);  Verify(cc(3,4)==1);
  }
  {
    printf("Testing ComputeConnectedComponents4 with properties...\n");
    ImgGray img(4, 5);
    ImgGray::Pixel a=80, b=32, c=131;
    img(0,0) = a;  img(1,0) = a;  img(2,0) = a;  img(3,0) = b;
    img(0,1) = b;  img(1,1) = b;  img(2,1) = a;  img(3,1) = b;
    img(0,2) = b;  img(1,2) = a;  img(2,2) = a;  img(3,2) = b;
    img(0,3) = b;  img(1,3) = c;  img(2,3) = c;  img(3,3) = b;
    img(0,4) = b;  img(1,4) = b;  img(2,4) = b;  img(3,4) = b;
    ImgInt cc;
    std::vector<ConnectedComponentProperties<ImgGray::Pixel> > props;
    ConnectedComponents4(img, &cc, &props);
    Verify(cc(0,0)==0);  Verify(cc(1,0)==0);  Verify(cc(2,0)==0);  Verify(cc(3,0)==1);
    Verify(cc(0,1)==1);  Verify(cc(1,1)==1);  Verify(cc(2,1)==0);  Verify(cc(3,1)==1);
    Verify(cc(0,2)==1);  Verify(cc(1,2)==0);  Verify(cc(2,2)==0);  Verify(cc(3,2)==1);
    Verify(cc(0,3)==1);  Verify(cc(1,3)==2);  Verify(cc(2,3)==2);  Verify(cc(3,3)==1);
    Verify(cc(0,4)==1);  Verify(cc(1,4)==1);  Verify(cc(2,4)==1);  Verify(cc(3,4)==1);
    Verify(props.size() == 3);
    Verify(props[0].npixels == 6);
    Verify((props[0].bounding_rect == Rect(0, 0, 3, 3)) != 0);
    Verify(props[0].value == a);
    Verify(props[1].npixels == 12);
    Verify((props[1].bounding_rect == Rect(0, 0, 4, 5)) != 0);
    Verify(props[1].value == b);
    Verify(props[2].npixels == 2);
    Verify((props[2].bounding_rect == Rect(1, 3, 3, 4)) != 0);
    Verify(props[2].value == c);
  }

  {
    printf("Testing ImgBinary ...\n");
    int width = blepo_ex::GetRand(1, 50);
    int height = blepo_ex::GetRand(1, 50);
    ImgBinary img(width, height);
    const ImgBinary& cimg = img;
    ImgBinary::Iterator it1 = img.Begin();
    ImgBinary::ConstIterator it2 = cimg.Begin();
    for (int y=0 ; y<height ; y++)
    {
      for (int x=0 ; x<width ; x++)
      {
        bool val = 0; //blepo_ex::GetRand(2) != 0;  
        img(x,y) = val;
        Verify(img(x,y) == val);
        Verify(*it1 == val);
        Verify(*it2 == val);
        val = blepo_ex::GetRand(2) != 0;  
        *it1 = val;
        Verify(img(x,y) == val);
        Verify(*it1 == val);
        Verify(*it2 == val);
        it1++;
        it2++;
      }
    }
  }

  {
    printf("Testing ImgBinary ptr ...\n");
    // create identical binary and gray images
    int width = blepo_ex::GetRand(1, 50);
    int height = blepo_ex::GetRand(1, 50);
    ImgBinary img(width, height);
    ImgGray gray(width, height);
    ImgBinary::Iterator it1 = img.Begin();
    ImgGray::Iterator it2 = gray.Begin();
    bool val;
    bool ff = it1 != img.End();
    for ( ; it1 != img.End() ; it1++, it2++)
    {
      val = blepo_ex::GetRand(2) != 0;  
      *it1 = val;
      *it2 = val;
      Verify(*it1 == (*it2 != 0));
    }
  }
  {
    printf("Testing ImgBinary ptr jumping ...\n");
    // create identical binary and gray images
    int width = blepo_ex::GetRand(1, 50);
    int height = blepo_ex::GetRand(1, 50);
    ImgBinary img(width, height);
    ImgGray gray(width, height);
    ImgBinary::Iterator it1 = img.Begin();
    ImgGray::Iterator it2 = gray.Begin();
    while (it2 != gray.End())
    {
      bool val = blepo_ex::GetRand(2) != 0;  
      *it1++ = val;
      *it2++ = val;
    }
    // jump pointers around at random, setting the same bit in each image
    it1 = img.Begin();
    it2 = gray.Begin();
    int n = width*height;
    int i;
    // first use +=
    for (i=0 ; i<100 ; i++)
    {
      int offset = blepo_ex::GetRand(0, (n+1)/2);
      if (it2-(n/2) >= gray.Begin())  offset = -offset;
      it1 += offset;
      it2 += offset;
      bool val = blepo_ex::GetRand(2) != 0;  
      *it1 = val;
      *it2 = val;
    }
    // then use -=
    for (i=0 ; i<100 ; i++)
    {
      int offset = blepo_ex::GetRand(0, (n+1)/2);
      if (it2-(n/2) <= gray.Begin())  offset = -offset;
      it1 -= offset;
      it2 -= offset;
      bool val = blepo_ex::GetRand(2) != 0;  
      *it1 = val;
      *it2 = val;
    }
    // first use +
    for (i=0 ; i<100 ; i++)
    {
      int offset = blepo_ex::GetRand(0, (n+1)/2);
      if (it2-(n/2) >= gray.Begin())  offset = -offset;
      it1 = it1 + offset;
      it2 = it2 + offset;
      bool val = blepo_ex::GetRand(2) != 0;  
      *it1 = val;
      *it2 = val;
    }
    // then use -
    for (i=0 ; i<100 ; i++)
    {
      int offset = blepo_ex::GetRand(0, (n+1)/2);
      if (it2-(n/2) <= gray.Begin())  offset = -offset;
      it1 = it1 - offset;
      it2 = it2 - offset;
      bool val = blepo_ex::GetRand(2) != 0;  
      *it1 = val;
      *it2 = val;
    }
    // now check to make sure they're the same
    it1 = img.Begin();
    it2 = gray.Begin();
    while (it2 != gray.End())
    {
      Verify(*it1 == (*it2!=0));
      it1++;
      it2++;
    }

  }

  {
//    printf("Testing ImgIplImage...\n");
//    int width = blepo_ex::GetRand(1,50);
//    int height = blepo_ex::GetRand(1,50);
//    ImgBinary img(width, height), img2;
//    for (int y=0 ; y<height ; y++)
//    {
//      for (int x=0 ; x<width ; x++)
//      {
//        bool val = blepo_ex::GetRand(2) != 0;  
//        img(x,y) = val;
//      }
//    }
//    ImgIplImage ipl(img);
//    ipl.CastToBinary(&img2);
//    Verify(IsIdentical(img, img2));
  }

  {
    printf("Testing And/Or/Xor/AbsDiff...\n");
    int width = blepo_ex::GetRand(1,50);
    int height = blepo_ex::GetRand(1,50);
    ImgGray img1(width, height), img2(width, height), imgand, imgor, imgxor, imgabsdiff;
    for (ImgGray::Iterator p1 = img1.Begin() ; p1 != img1.End() ; p1++)  *p1 = blepo_ex::GetRand(256);
    for (ImgGray::Iterator p2 = img2.Begin() ; p2 != img2.End() ; p2++)  *p2 = blepo_ex::GetRand(256);
//    TRACE("hii %d %d\n", width, height);
    And(img1, img2, &imgand);
    Or(img1, img2, &imgor);
    Xor(img1, img2, &imgxor);
    AbsDiff(img1, img2, &imgabsdiff);
    for (int y=0 ; y<height ; y++)
    {
      for (int x=0 ; x<width ; x++)
      {
        unsigned char val_and = imgand(x,y);
        unsigned char val_or = imgor(x,y);
        unsigned char val_xor = imgxor(x,y);
        unsigned char val_absdiff = imgabsdiff(x,y);
        Verify(val_and == (img1(x,y) & img2(x,y)));
        Verify(val_or == (img1(x,y) | img2(x,y)));
        Verify(val_xor == (img1(x,y) ^ img2(x,y)));
        int diff = img1(x,y) - img2(x,y);
        Verify(val_absdiff == (diff > 0 ? diff : -diff));
      }
    }
  }
  
  {
    printf("Testing FloodFill...\n");
    //ImgGray
    {
      ImgGray img(4,4),img1;
      ImgGray::Pixel bb = 255;
      ImgGray::Pixel cc = 0;
      ImgGray::Pixel rr = 125;
      img(0,0)= cc; img(1,0)= bb; img(2,0)= bb; img(3,0)= bb;
      img(0,1)= bb; img(1,1)= cc; img(2,1)= cc; img(3,1)= bb;
      img(0,2)= bb; img(1,2)= cc; img(2,2)= cc; img(3,2)= bb;
      img(0,3)= bb; img(1,3)= bb; img(2,3)= bb; img(3,3)= cc;
      img1 = img;
      FloodFill4(img,1,1,rr,&img1);
      Verify(img1(0,0) == cc); Verify(img1(1,0) == bb); Verify(img1(2,0) == bb); Verify(img1(3,0) == bb);
      Verify(img1(0,1) == bb); Verify(img1(1,1) == rr); Verify(img1(2,1) == rr); Verify(img1(3,1) == bb);
      Verify(img1(0,2) == bb); Verify(img1(1,2) == rr); Verify(img1(2,2) == rr); Verify(img1(3,2) == bb);
      Verify(img1(0,3) == bb); Verify(img1(1,3) == bb); Verify(img1(2,3) == bb); Verify(img1(3,3) == cc);
      img1 = img;
      FloodFill8(img,0,0,rr,&img1);
//      Figure fig;
//      fig.Draw(img1);
//      fig.GrabMouseClick();
      Verify(img1(0,0) == rr); Verify(img1(1,0) == bb); Verify(img1(2,0) == bb); Verify(img1(3,0) == bb);
      Verify(img1(0,1) == bb); Verify(img1(1,1) == rr); Verify(img1(2,1) == rr); Verify(img1(3,1) == bb);
      Verify(img1(0,2) == bb); Verify(img1(1,2) == rr); Verify(img1(2,2) == rr); Verify(img1(3,2) == bb);
      Verify(img1(0,3) == bb); Verify(img1(1,3) == bb); Verify(img1(2,3) == bb); Verify(img1(3,3) == rr);
    }
    //ImgBinary
    {
      ImgBinary img(4,4),img1;
      ImgBinary::Pixel bb = 1;
      ImgBinary::Pixel cc = 0;
      ImgBinary::Pixel rr = 1;
      img(0,0)= cc; img(1,0)= bb; img(2,0)= bb; img(3,0)= bb;
      img(0,1)= bb; img(1,1)= cc; img(2,1)= cc; img(3,1)= bb;
      img(0,2)= bb; img(1,2)= cc; img(2,2)= cc; img(3,2)= bb;
      img(0,3)= bb; img(1,3)= bb; img(2,3)= bb; img(3,3)= cc;
      img1 = img;
      FloodFill4(img,1,1,rr,&img1);
      Verify(img1(0,0) == cc); Verify(img1(1,0) == bb); Verify(img1(2,0) == bb); Verify(img1(3,0) == bb);
      Verify(img1(0,1) == bb); Verify(img1(1,1) == rr); Verify(img1(2,1) == rr); Verify(img1(3,1) == bb);
      Verify(img1(0,2) == bb); Verify(img1(1,2) == rr); Verify(img1(2,2) == rr); Verify(img1(3,2) == bb);
      Verify(img1(0,3) == bb); Verify(img1(1,3) == bb); Verify(img1(2,3) == bb); Verify(img1(3,3) == cc);
      img1 = img;
      FloodFill8(img,0,0,rr,&img1);
      Verify(img1(0,0) == rr); Verify(img1(1,0) == bb); Verify(img1(2,0) == bb); Verify(img1(3,0) == bb);
      Verify(img1(0,1) == bb); Verify(img1(1,1) == rr); Verify(img1(2,1) == rr); Verify(img1(3,1) == bb);
      Verify(img1(0,2) == bb); Verify(img1(1,2) == rr); Verify(img1(2,2) == rr); Verify(img1(3,2) == bb);
      Verify(img1(0,3) == bb); Verify(img1(1,3) == bb); Verify(img1(2,3) == bb); Verify(img1(3,3) == rr);
    }
    //ImgBgr
    {
      ImgBgr img(4,4),img1;
      ImgBgr::Pixel bb(255,255,255);
      ImgBgr::Pixel cc(0,0,0);
      ImgBgr::Pixel rr(125,125,125);
      img(0,0)= cc; img(1,0)= bb; img(2,0)= bb; img(3,0)= bb;
      img(0,1)= bb; img(1,1)= cc; img(2,1)= cc; img(3,1)= bb;
      img(0,2)= bb; img(1,2)= cc; img(2,2)= cc; img(3,2)= bb;
      img(0,3)= bb; img(1,3)= bb; img(2,3)= bb; img(3,3)= cc;
      img1 = img;
      FloodFill4(img,1,1,rr,&img1);
      Verify(img1(0,0) == cc); Verify(img1(1,0) == bb); Verify(img1(2,0) == bb); Verify(img1(3,0) == bb);
      Verify(img1(0,1) == bb); Verify(img1(1,1) == rr); Verify(img1(2,1) == rr); Verify(img1(3,1) == bb);
      Verify(img1(0,2) == bb); Verify(img1(1,2) == rr); Verify(img1(2,2) == rr); Verify(img1(3,2) == bb);
      Verify(img1(0,3) == bb); Verify(img1(1,3) == bb); Verify(img1(2,3) == bb); Verify(img1(3,3) == cc);
//      FloodFill8(img,rr,0,0,&img1);
      img1 = img;
      FloodFill8(img,0,0,rr,&img1);
      Verify(img1(0,0) == rr); Verify(img1(1,0) == bb); Verify(img1(2,0) == bb); Verify(img1(3,0) == bb);
      Verify(img1(0,1) == bb); Verify(img1(1,1) == rr); Verify(img1(2,1) == rr); Verify(img1(3,1) == bb);
      Verify(img1(0,2) == bb); Verify(img1(1,2) == rr); Verify(img1(2,2) == rr); Verify(img1(3,2) == bb);
      Verify(img1(0,3) == bb); Verify(img1(1,3) == bb); Verify(img1(2,3) == bb); Verify(img1(3,3) == rr);
    }

  }

  {
    printf("Testing Erode...\n");
    //ImgGray
    {
      int i,j,k,m;
      bool flag;
      int width = 21,height = 21;
      ImgGray img(width,height),img1;
      ImgGray::Pixel min_val;
      for(i=0;i<height;i++)
      {
        for(j=0;j<width;j++)
        {
          ImgGray::Pixel pix = blepo_ex::GetRand(0,256);
          pix>128?img(j,i) = 255:img(j,i) = 0;
        }
      }
      Erode3x3(img,&img1);
      for(i=1;i<height-1;i++)
      {
        for(j=1;j<width-4;j++)
        {
          min_val = img(j,i);
          for(k=-1;k<=1;k++)
          {
            for(m=-1;m<=1;m++)
            {
              if (img(j+k,i+m) < min_val) min_val = img(j+k,i+m);
              if (min_val == 0) 
              {
                flag = TRUE;
                break;
              }
            }
            if(flag == TRUE) 
            {
              flag = FALSE;
              break;
            }
          }
          Verify(img1(j,i)==min_val);
        }
      }
    }
  }
  
  {
    printf("Testing Dilate...\n");
    //ImgGray
    {
      int i,j,k,m;
      bool flag;
      int width = 21,height = 21;
      ImgGray img(width,height),img1;
      ImgGray::Pixel max_val;
      for(i=0;i<height;i++)
      {
        for(j=0;j<width;j++)
        {
          ImgGray::Pixel pix = blepo_ex::GetRand(0,256);
          pix>128?img(j,i) = 255:img(j,i) = 0;
        }
      }
      Dilate3x3(img,&img1);
      for(i=1;i<height-1;i++)
      {
        for(j=1;j<width-4;j++)
        {
          max_val = img(j,i);
          for(k=-1;k<=1;k++)
          {
            for(m=-1;m<=1;m++)
            {
              if (img(j+k,i+m) > max_val) max_val = img(j+k,i+m);
              if (max_val == 255) 
              {
                flag = TRUE;
                break;
              }
            }
            if(flag == TRUE) 
            {
              flag = FALSE;
              break;
            }
          }
          Verify(img1(j,i)==max_val);
        }
      }
    }
  }

  {
    printf("Testing Matrix...\n");
    MatDbl a(3,3);
    MatDbl::Iterator t = a.Begin();
    *t++ =  0.5;  *t++ =  0.8;  *t++ =  1.4;
    *t++ = 17.2;  *t++ = -0.5;  *t++ = 18.1;
    *t++ =  4.1;  *t++ = 21.3;  *t++ = -31.2;
    {
      printf("Testing SVD (using ground truth)...\n");
      MatDbl u, s, v;
      Svd(a, &u, &s, &v);	
      /*  % Copy and paste this Matlab code to compute results below.
          % Signs of corresponding columns of u and v may have to be changed 
          % manually due to the inherent sign ambiguity of the SVD.
          a = [0.5 0.8 1.4 ; 17.2 -0.5 18.1 ; 4.1 21.3 -31.2];
          [u,s,v] = svd(a)
      */
      double uu[] = { -0.0218,  -0.0515,   0.9984,
                      -0.4300,  -0.9011,  -0.0558,
                       0.9026,  -0.4305,  -0.0025 };
      double ss[] = { 41.0494,  19.6060,   1.0182 };
      double vv[] = { -0.0903,  -0.8819,  -0.4628,
                       0.4731,  -0.4469,   0.7592,
                      -0.8763,  -0.1504,   0.4576 };
      double eps = 0.0001;  // the accuracy is actually much better than this, but I have
                            // only included four decimal digits in the ground truth uu,ss,vv.
      int i;
      t = u.Begin();  for (i=0 ; i<9 ; i++)  Verify(blepo_ex::Similar(*t++, uu[i], eps));
      t = s.Begin();  for (i=0 ; i<3 ; i++)  Verify(blepo_ex::Similar(*t++, ss[i], eps));
      t = v.Begin();  for (i=0 ; i<9 ; i++)  Verify(blepo_ex::Similar(*t++, vv[i], eps));

      printf("Testing SVD (by multiplying U, S, and V)...\n");
      eps = 1e-13;
      Verify(Similar(a, u * Diag(s) * Transpose(v), eps));
    }
    {
      printf("Testing LU (using ground truth)...\n");
      MatDbl l, u, p;
      Lu(a, &l, &u, &p);
      double ll[] = { 1.0000,        0,        0,
                      0.2384,   1.0000,        0,
                      0.0291,   0.0380,   1.0000 };
      double uu[] = {17.2000,  -0.5000,  18.1000,
                           0,  21.4192, -35.5145,
                           0,        0,   2.2244 };
      double pp[] = {      0,    1,    0,
                           0,    0,    1,
                           1,    0,    0 };
      double eps = 1e-4;
      int i;
      t = l.Begin();  for (i=0 ; i<9 ; i++)  Verify(blepo_ex::Similar(*t++, ll[i], eps));
      t = u.Begin();  for (i=0 ; i<9 ; i++)  Verify(blepo_ex::Similar(*t++, uu[i], eps));
      t = p.Begin();  for (i=0 ; i<9 ; i++)  Verify(blepo_ex::Similar(*t++, pp[i], eps));
      
      printf("Testing LU (by multiplying L and U)...\n");
      eps = 1e-13;
      Verify(Similar(p * a, l * u, eps));
    }

    {
      printf("Testing matrix inverse (using ground truth)...\n");
      MatDbl ainv;
      Inverse(a, &ainv);
      double aa[] = {  -0.4514,   0.0668,   0.0185,
                        0.7454,  -0.0260,   0.0183,
                        0.4496,  -0.0090,  -0.0171 };
      double eps = 1e-4;
      int i;
      t = ainv.Begin();  for (i=0 ; i<9 ; i++)  Verify(blepo_ex::Similar(*t++, aa[i], eps));
      printf("Testing matrix inverse (by multiplying A and A^{-1})...\n");
      Verify(Similar(Eye(3), a * ainv, eps));
    }

    {
      printf("Testing matrix determinant...\n");
      double eps = 1e-4;
      Verify(blepo_ex::Similar(Determinant(a), 819.4890, eps));
    }

    {
      printf("Testing eigen (symmetric)...\n");
      MatDbl b = a + Transpose(a);  // make symmetric matrix
      MatDbl eigenvalues, eigenvectors;
      EigenSymm(b, &eigenvalues);
      EigenSymm(b, &eigenvalues, &eigenvectors);
      double eval[] = { 30.1377,  -10.7788,   -81.7589 };
      double evec[] = { 0.5388,   0.8416,   0.0373,
                        0.7631,  -0.4688,  -0.4449,
                        0.3569,  -0.2681,   0.8948 };
      double eps = 1e-4;
      int i;
      t = eigenvalues.Begin();  for (i=0 ; i<3 ; i++)  Verify(blepo_ex::Similar(*t++, eval[i], eps));
      t = eigenvectors.Begin();  for (i=0 ; i<9 ; i++)  Verify(blepo_ex::Similar(*t++, evec[i], eps));
    }

    {
      printf("Testing QR factorization (using ground truth)...\n");
      MatDbl q, r;
      Qr(a, &q, &r);
      double qq[] = {  -0.0283,   0.0323,  -0.9991,
                       -0.9724,  -0.2326,   0.0200,
                       -0.2318,   0.9720,   0.0380 };
      double rr[] = { -17.6890,  -4.4734, -10.4076,
                             0,  20.8463, -34.4928,
                             0,        0,  -2.2223 };
      double eps = 1e-4;
      int i;
      t = q.Begin();  for (i=0 ; i<9 ; i++)  Verify(blepo_ex::Similar(*t++, qq[i], eps));
      t = r.Begin();  for (i=0 ; i<9 ; i++)  Verify(blepo_ex::Similar(*t++, rr[i], eps));
      printf("Testing QR factorization (by multiplying Q and R)...\n");
      eps = 1e-13;
      Verify(Similar(a, q * r, eps));
    }

    {
      /*  b = [17.4 -5.3 31.7]';
          x = a \ b
      */
      MatDbl b(3), x, residue;
      t = b.Begin();  *t++ = 17.4;  *t++ = -5.3;  *t++ = 31.7;
      double xx[] = { -7.6217, 13.6894, 7.3281 };
      double eps = 1e-4, eps2 = 1e-12;
      int i;

      printf("Testing solve linear equation (LU square) (using ground truth)...\n");
      SolveLinearLuSquare(a, b, &x);
      t = x.Begin();  for (i=0 ; i<3 ; i++)  Verify(blepo_ex::Similar(*t++, xx[i], eps));
      printf("Testing solve linear equation (LU square) (multiplying a * x)...\n");
      Verify(Similar(a * x, b, eps2));
      
      printf("Testing solve linear equation (QR square) (using ground truth)...\n");
      x.Reset();
      SolveLinearQrSquare(a, b, &x);
      t = x.Begin();  for (i=0 ; i<3 ; i++)  Verify(blepo_ex::Similar(*t++, xx[i], eps));
      printf("Testing solve linear equation (QR square) (multiplying a * x)...\n");
      Verify(Similar(a * x, b, eps2));
      
      printf("Testing solve linear equation (QR) (using ground truth)...\n");
      x.Reset();
      SolveLinearQr(a, b, &x, &residue);
      t = x.Begin();  for (i=0 ; i<3 ; i++)  Verify(blepo_ex::Similar(*t++, xx[i], eps));
      printf("Testing solve linear equation (QR) (multiplying a * x)...\n");
      Verify(Similar(a * x, b, eps2));

      printf("Testing solve linear equation (SVD) (using ground truth)...\n");
      x.Reset();
      SolveLinearSvd(a, b, &x);
      t = x.Begin();  for (i=0 ; i<3 ; i++)  Verify(blepo_ex::Similar(*t++, xx[i], eps));
      printf("Testing solve linear equation (SVD) (multiplying a * x)...\n");
      Verify(Similar(a * x, b, eps2));
    }

    {
      printf("Testing solve linear equation overdetermined (QR) and (SVD)...\n");
      MatDbl a(14,16), b(16), x1, x2, res;
      Rand(14, 16, &a);
      Rand(1, 16, &b);
      SolveLinearQr(a, b, &x1, &res);
      SolveLinearSvd(a, b, &x2);
      double eps2 = 1e-12;
      Verify(Similar(x1, x2, eps2));
      Verify(Similar(b - a * x1, res, eps2));
    }

    {
      printf("Testing solve linear equation general (square)...\n");
      int m = blepo_ex::GetRand(4, 200);  
      int n = blepo_ex::GetRand(3, m);  
      MatDbl a, b, x1, x2, x3, res;
      Rand(n, n, &a);
      Rand(1, n, &b);
      SolveLinear(a, b, &x1);
      double eps2 = 1e-12;
      Verify(Similar(a * x1, b, eps2));

      printf("Testing solve linear equation general (overdetermined)...\n");
      Rand(n, m, &a);
      Rand(1, m, &b);
      SolveLinear(a, b, &x1);
      SolveLinearQr(a, b, &x2, &res);
      SolveLinearSvd(a, b, &x3);
      Verify(Similar(x1, x2, eps2));
      Verify(Similar(x2, x3, eps2));
    }
  }

  
  printf("SUCCESS!!!\n");
  printf("Press return\n");
  getchar();
	return 0;
}



//  {
//    int arr[10];
//    int* p = arr;
//    const int* p2 = arr;
////    *p2 = 5; // error
//    p2 += 10;  // okay
//    const int* const p3 = arr;
//    p3 += 10;
//  }
//  {
//    ImgGray foo(160,120);
//    ImgGray::Iterator ptr = foo.Begin();
//    float* a = reinterpret_cast<float*>(ptr);
//  }
//  {
//    ImgBinary goo(160,120);
//    ImgBinary::Iterator ptr; 
//    ptr = goo.Begin();
//    ImgBinary::Iterator ptr2 = goo.Begin();
////    float* a = ptr;
//    unsigned char* p = goo.Begin();
//    float* pa = reinterpret_cast<float*>(p);
//    unsigned gg = 5;
//    float* pb = pa + gg; //(-5);
//    unsigned int ff = pa - pb;
//    for (int aa=0; aa>-30 ; aa--)
//    {
//      int bb = aa & 0x07;
//      TRACE("%d %d\n", aa, bb);
//    }
//    int a = 5;
//    int b = (a += 6);
//    (a+=7) = 0;
//    int c = -5;
//    int e = c / 8;
//    int f = c % 8;
//    c <<= 2;
//    int d = c & 0x07;
//  }
//}

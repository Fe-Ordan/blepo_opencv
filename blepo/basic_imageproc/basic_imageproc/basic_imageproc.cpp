// Goutham Pacha Ravi
// This is my first set of experiments with MFC and Blepo/OpenCV
// basic_images.cpp : Defines the entry point for the console application.
// Program does the following tasks:
// 1. It reads and displays 2 (1st and 3rd Argument) images provided as command line arguments
//		example: ./basic filename1 filename2 filename3
// 2. It creates, displays and saves (filename: 2nd argument) a synthetic grayscale image (A simple image with a black background and white square foreground)
//		whose dimensions match that of image loaded from 1st argument with each pixel in a 100x100 square region centered in the image set to the value 255
//		and all other pixels set to 0.  If filename1 is smaller than 100x100, the application sets all pixels in the synthetic image to 255.
// 3. Note that functions Draw and Reset are Blepo Wrappers to OpenCV functions cv::resize and cv::imshow
// 4. Function Save is a Blepo Wrapper to OpenCV function cv::imwrite

#include <afxwin.h>  // necessary for MFC to work properly
#include "basic_imageproc.h"
#include "../../blepo/src/blepo.h"
#include <cstring>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace blepo;

int main(int argc, const char* argv[], const char* envp[])
{
    
	
	if(argc!=4)
	{
		printf("This program needs three command line parameters, please re-run and try again!");
		exit(1);
	}
	
	// Initialize MFC and return if failure
    HMODULE hModule = ::GetModuleHandle(NULL);
    if (hModule == NULL || !AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
    {
        printf("Fatal Error: MFC initialization failed (hModule = %x)\n", hModule);
        return 1;
    }
	try{
	//Loading the first image (argv[1]) from blepo/images directory


	int x,y; //Loop Variables
	int ht,wt; //Height and Width when needed
	int ht2,wt2; //Height and Width when needed
	ImgBgr img1; //BGR Image from filename1 path
	ImgGray img2, img3; //Grayscale Synthetic Image that I'm creating and Grayscale image to load image from filename3 path
	CString filename1=argv[1],filename2=argv[2],filename3=argv[3]; //Reading cmdline args into variables
	Figure fig1,fig2,fig3,fig4; //Figure variables for drawing images into windows
	
	filename1="../../images/"+filename1;
	filename2="../../images/"+filename2;
	filename3="../../images/"+filename3;
	//filename1="../../images/hydrant.pgm";
	
	//Loading Image from Filename1 into fig1 and drawing it onto the screen
	Load(filename1, &img1);
	fig1.Draw(img1);
	ht=img1.Height();
	wt=img1.Width();
	

	//Making and displaying the synthetic image
	img2.Reset(wt,ht);
	Set(&img2,0); //Setting all pixels to black
    
	//If image is of proportions lesser than 100X100, painting the whole image white
	//Please note that the bounds here do not check for a larger area than a 100X100 image, like 400X50 because the question is ambigious. 
	if(wt<100 || ht<100)	{
		Set(&img2,255);
	}
	//If image is bigger than 100X100, painting a 100X100 area at the center white
	else {
		
		wt2=wt/2 - 50,ht2=ht/2 - 50;
		Rect r(wt2,ht2,wt2+100,ht2+100);
		Set(&img2, r, 255);
	}

	//Displaying Image2 with fig2 
	fig2.Draw(img2);
	//Saving Image2 - synthetic image with the filename2 provided as cmdline arg
	Save(img2,filename2,"jpg");

	
	
	//Loading and displaying image at filename3 path as a grayscale image
	Load(filename3, &img3);
	fig3.Draw(img3);


	//Masking BGR image with the image at filename3 path and displaying it
	//Throwing an error if two images don't match by size
	if(img3.Height()!=img1.Height()||img3.Width()!=img1.Width())
	{
		cout<<endl<<endl<<filename1<<" cannot be masked with "<<filename3<<" because their sizes aren't the same! Rerun with different image to retry"<<endl;
	}
	else
	{
		for(y=0; y<img1.Height();y++)
		{
			for(x=0; x<img1.Width();x++)
			{
				Bgr& pix1= img1(x,y); 
				unsigned char pix2=img3(x,y);
				if(pix2==0){pix1.b=0;pix1.g=0;pix1.r=0;} //Masking ~ Setting a pixel to black (0,0,0)
			}

		}
		
		fig4.Draw(img1);
	}

	

	}catch(const Exception& e)
	{
		e.Display();
	}
	
    EventLoop();	//The MFC Event Loop

    return 0;
}

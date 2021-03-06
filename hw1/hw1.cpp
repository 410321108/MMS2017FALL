#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <cv.h>
#include <highgui.h>
#include <math.h>


int main()
{
	IplImage *input, *output, *outputGray;

	char InputFileName[] = "C:\\Users\\Default\\Desktop\\hw1\\hancat.jpg";
	input = cvLoadImage(InputFileName, CV_LOAD_IMAGE_COLOR);

	if (!input)
	{
		printf("Can't open the image.\n");
		system("pause");
	}
	output = cvCreateImage(cvSize(input->width, input->height), 8, 3);
	outputGray = cvCreateImage(cvSize(input->width, input->height), 8, 3);

	unsigned short *GrayArr = (unsigned short *)calloc(input->height*input->width, sizeof(unsigned short));
	unsigned short *BlackWhiteArr = (unsigned short *)calloc(input->height*input->width, sizeof(unsigned short));

	int width = input->width;
	int height = input->height;

	for (int i = 0; i<input->height; i++) {
		for (int j = 0; j<input->width; j++) {
			CvScalar x = cvGet2D(input, i, j);

			int y = 0.299*x.val[2] + 0.587*x.val[1] + 0.114*x.val[0];
			GrayArr[i*input->width + j] = y;

			cvSet2D(outputGray, i, j, CV_RGB(GrayArr[i*input->width + j], GrayArr[i*input->width + j], GrayArr[i*input->width + j]));
		}
	}
	int table[8][8] = { 
		{	0	,128	,32		,160	,8		,136	,40		,168	},
		{	192	,64		,224	,96		,200	,72		,232	,104	},
		{	48	,176	,16		,144	,56		,184	,24		,152	},
		{	240	,112	,208	,80		,248	,120	,216	,88		},
		{	12	,140	,44		,172	,4		,132	,36		,164	},
		{	204	,76		,236	,108	,196	,68		,228	,100	},
		{	60	,188	,28		,156	,52		,180	,20		,148	},
		{	252	,124	,220	,92		,244	,116	,212	,84		}
	};

	int skipX = 8, skipY = 8, gray = 0;
	for (int r = 0; r<height - skipY; r += skipY)
	{
		for (int c = 0; c<width - skipX; c += skipX)
		{
			for (int row = 0; row<skipY; ++row)
			{
				for (int col = 0; col<skipX; ++col)
				{
					CvScalar xa = cvGet2D(input, r + row, c + col);
					int gray = 0.299*xa.val[2] + 0.587*xa.val[1] + 0.114*xa.val[0];//gray
					if (gray >= table[row][col]) {
						BlackWhiteArr[r *input->width + c] = 256;
					}
					else {
						BlackWhiteArr[r *input->width + c] = 0;
					}
					cvSet2D(output, (r + row), (c + col), CV_RGB(BlackWhiteArr[r*input->width + c], BlackWhiteArr[r*input->width + c], BlackWhiteArr[r*input->width + c]));
				}
			}
		}
	}

	cvNamedWindow("First", 0);
	cvNamedWindow("Gray", 0);
	cvNamedWindow("BlackWhite", 0);

	cvMoveWindow("First",0,0 );
	cvMoveWindow("Gray",0,0 );
	cvMoveWindow("BlackWhite",0,0 );

	cvResizeWindow("First", 480, 360);
	cvResizeWindow("Gray", 480, 360);
	cvResizeWindow("BlackWhite", 480, 360);

	cvShowImage("First", input);
	cvShowImage("Gray", outputGray);
	cvShowImage("BlackWhite", output);


	cvWaitKey(0);
	free(GrayArr);

	cvReleaseImage(&input);
	cvReleaseImage(&output);

	cvDestroyWindow("First");
	cvDestroyWindow("Gray");
	cvDestroyWindow("BlackWhite");

	return 0;
}
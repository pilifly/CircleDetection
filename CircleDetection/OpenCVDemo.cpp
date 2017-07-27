#include "stdafx.h"
#include "OpenCVDemo.h"


COpenCVDemo::COpenCVDemo()
{
	m_minDist = 50;
	m_minRad = 2;
	m_maxRad = 4;
	m_minCutoff = 6;
	m_maxCutoff = 12;
	m_contourThresh = 100;
	m_winName = std::string("View");
	m_matMasterIamge = Mat(600, 800, CV_8UC3, Scalar(255, 255, 255));
	m_matMasterIamge.copyTo(m_matProcessedImage);
}


COpenCVDemo::~COpenCVDemo()
{
	cv::destroyAllWindows();
}

void COpenCVDemo::ReadFileAndDetect()
{

	CFileDialog dlg(TRUE);
	dlg.m_ofn.lpstrFilter = "Image Files (*.png)\0*.png\0Jpg Files (*.jpg)\0*.jpg\0All Files (*.*)\0*.*\0\0";
	dlg.m_ofn.lpstrTitle = "Open Image File";

	if (dlg.DoModal() == IDOK)
	{
		CString strStlFileName = dlg.GetPathName(); // return
		if (!strStlFileName.IsEmpty())
		{
			Mat matImg;

			matImg = cv::imread(std::string(CT2A(strStlFileName)));
			//cv::imshow(m_winName, matImg);
			matImg.copyTo(m_matOutImage);
			SetData(matImg);
			DetectCircles(m_matProcessedImage);
			//SetData(matImg);
		}
	}

}

void COpenCVDemo::DetectCircles(Mat src)
{
	CWaitCursor waitCursor;
	Mat src_gray;

	/// Read the image
	//src = imread(argv[1], 1);

	if (!src.data)
	{
		return;
	}

	/// Convert it to gray
	//cvtColor(src, src_gray, CV_BGR2GRAY);
	src.copyTo(src_gray);
	/// Reduce the noise so we avoid false circle detection
	//src.copyTo(src_gray);
	GaussianBlur(src_gray, src_gray, Size(9, 9), 2, 2);

	vector<Vec3f> circles;

	/// Apply the Hough Transform to find the circles
	//HoughCircles(src_gray, circles, CV_HOUGH_GRADIENT, 1, src_gray.rows / 8, 200, 100, 0, 0);
	HoughCircles(src_gray, circles, CV_HOUGH_GRADIENT, 1, src_gray.rows / (double)m_minDist, m_maxCutoff, m_minCutoff, m_minRad, m_maxRad);


	/// Draw the circles detected
	for (size_t i = 0; i < circles.size(); i++)
	{
		Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
		int radius = cvRound(circles[i][2]);
		// circle center
		circle(m_matOutImage, center, 3, Scalar(0, 255, 0), -1, 8, 0);
		// circle outline
		circle(m_matOutImage, center, radius, Scalar(0, 0, 255), 3, 8, 0);
	}

	/// Show your results
	//namedWindow("Hough Circle Transform Demo", CV_WINDOW_AUTOSIZE);
	//namedWindow(m_winName, CV_WINDOW_AUTOSIZE);
	cv::imshow(m_winName, m_matOutImage);
	waitKey(5);

}

void COpenCVDemo::SetData(Mat matImg)
{
	
	matImg.copyTo(m_matOutImage);
	matImg.copyTo(m_matMasterIamge);
	cvtColor(matImg, m_matProcessedImage, CV_BGR2GRAY);
	//m_matMasterIamge.copyTo(m_matProcessedImage);
	imshow(m_winName, m_matProcessedImage);
	waitKey(5);
}
void COpenCVDemo::DetectCircles()
{
	DetectCircles(m_matProcessedImage);
	
}
void COpenCVDemo::ApplyLaplacian()
{
	Mat matOut, dst;
	GaussianBlur(m_matProcessedImage, matOut, Size(3, 3), 0, 0, BORDER_DEFAULT);
	Laplacian(matOut, dst, CV_16S, 3, 1, 0, BORDER_DEFAULT);
	//convertScaleAbs(dst, abs_dst);
	cv::normalize(dst, matOut, 0, 255, NORM_MINMAX, CV_8UC1);
	matOut.copyTo(m_matProcessedImage);
	cvtColor(m_matProcessedImage, m_matMasterIamge, CV_GRAY2BGR);
	
	imshow(m_winName, m_matMasterIamge);
	waitKey(5);

}

void COpenCVDemo::DetectContours(double thresh, int cntrFilter, double cntrRad)
{
	Mat canny_output;
	RNG rng(12345);
	m_matMasterIamge.copyTo(m_matOutImage);

	/// Detect edges using canny
	Canny(m_matProcessedImage, canny_output, thresh, thresh * 2, 3);
	/// Find contours
	findContours(canny_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	//findContours(canny_output, contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

	/// Draw contours
	//Mat drawing = Mat::zeros(m_matProcessedImage.size(), CV_8UC3);
	
	for(int  i =0; i < contours.size(); i++)
		for (int j = 1; j < contours[i].size(); j++)
		{
			//circle(matEdge, contours[i].at(j), 1, Scalar(160), 1, 8);
			line(m_matOutImage, contours[i].at(j), contours[i].at(j - 1), Scalar(0,255,0), 1, 8);
			//if(contours[i].size() < cntrFilter)
			//	circle(m_matOutImage, contours[i].at(contours[i].size()/2), cntrRad, Scalar(0,0,255), 2, 8);
			
		}

	imshow(m_winName, m_matOutImage);
	waitKey(5);
}
void COpenCVDemo::UpdateCirclesDetection(int minDist, int minRad, int maxRad, int minCutoff, int maxCutoff)
{
	m_minDist = minDist;
	m_minRad = minRad;
	m_maxRad = maxRad;
	m_minCutoff = minCutoff;
	m_maxCutoff = maxCutoff;
	m_matMasterIamge.copyTo(m_matOutImage);
	DetectCircles(m_matProcessedImage);

}



#pragma once
#include "CVHeader.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

class COpenCVDemo
{
public:
	COpenCVDemo();
	~COpenCVDemo();

	void ReadFileAndDetect();
	void DetectCircles(Mat src);
	void SetData(Mat matImg);
	void DetectCircles();
	void ApplyLaplacian();
	void DetectContours(double thresh, int cntrFilter, double cntrRad);
	void UpdateCirclesDetection(int minDist, int minRad, int maxRad, int minCutoff, int maxCutoff);

protected:

	std::string m_winName;
	Mat m_matMasterIamge;
	Mat m_matProcessedImage;
	Mat m_matOutImage;
	int m_minDist;
	int m_minRad;
	int m_maxRad;
	int m_minCutoff;
	int m_maxCutoff;
	int m_contourThresh;

	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;


};


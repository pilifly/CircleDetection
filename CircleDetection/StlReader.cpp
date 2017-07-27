#include "stdafx.h"
#include "StlReader.h"


CStlReader::CStlReader()
{
}


CStlReader::~CStlReader()
{
}

void CStlReader::ReadStlFile()
{
	CFileDialog dlg(TRUE);
	dlg.m_ofn.lpstrFilter = "STL Files (*.stl)\0*.stl\0Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0\0";
	dlg.m_ofn.lpstrTitle = "Open STL File";

	if (dlg.DoModal() == IDOK)
	{
		CString strStlFileName = dlg.GetPathName(); // return
		if (!strStlFileName.IsEmpty())
		{
			ReadBinaryStl(strStlFileName);
		}
	}

}

bool CStlReader::ReadBinaryStl(CString fileName)
{
	CWaitCursor waitCursor;

	CStdioFile fileIn;
	CFileException excptError;
	primTriangle primRead;
	if (fileIn.Open(fileName, CFile::modeRead | CFile::typeBinary, &excptError))
	{
		long faceCount = 0;
		m_faceBuffer.clear();
		fileIn.Read(&m_headerBuff, sizeof(m_headerBuff));
		fileIn.Read(&faceCount, sizeof(long));
		stlFace faceRead;
		primTriangle primBuffer;
		if (0 != faceCount)
		{

			//For unknown reason primTriangle is 52-byte instead of 50-byte. Force file reading in 50-byte chunks
			for (int i = 0; i < faceCount; i++)
			{
				fileIn.Read(&faceRead, 50 * sizeof(char));

				for (int j = 0; j < 3; j++)
				{
					for (int m = 0; m < 3; m++)
					{
						primBuffer.vertxVect[j].v[m] = faceRead.vertxVect[j].v[m];
					}
				}

				primBuffer.faceID = i;
				for (int m = 0; m < 3; m++)
				{
					primBuffer.vertxVect[m].id = -1;
					primBuffer.normVect.v[m] = faceRead.normVect.v[m];
				}

				primBuffer.attribByteCount = 0;
				for (int m = 0; m < 3; m++)
					for (int j = 0; j < 3; j++)
						primBuffer.colorVect[m].v[j] = 0.8f;
				m_faceBuffer.push_back(primBuffer);
			}

		}

		fileIn.Close();
		//Scale it ot 0-1 cube;


		m_xMin = m_xMax = m_faceBuffer[0].vertxVect[0].v[0];
		m_yMin = m_yMax = m_faceBuffer[0].vertxVect[0].v[1];
		m_zMin = m_zMax = m_faceBuffer[0].vertxVect[0].v[2];
		GetMinMax(&m_faceBuffer, &m_xMin, &m_xMax, &m_yMin, &m_yMax, &m_zMin, &m_zMax);

		float maxVal = m_xMax - m_xMin;
		if ((m_yMax - m_yMin) > maxVal)
			maxVal = m_yMax - m_yMin;
		if ((m_zMax - m_zMin) > maxVal)
			maxVal = m_zMax - m_zMin;

		float fScale = 1.0f;
		if (0 != maxVal)
			fScale = 1.0 / maxVal;


		for (long i = 0; i < m_faceBuffer.size(); i++)
		{
			for (int j = 0; j < 3; j++)
			{
				m_faceBuffer[i].vertxVect[j].v[0] = fScale*(m_faceBuffer[i].vertxVect[j].v[0] - m_xMin);
				m_faceBuffer[i].vertxVect[j].v[1] = fScale*(m_faceBuffer[i].vertxVect[j].v[1] - m_yMin);
				m_faceBuffer[i].vertxVect[j].v[2] = fScale*(m_faceBuffer[i].vertxVect[j].v[2] - m_zMin);
			}

		}

		StlToRegularGrid(&m_faceBuffer, 0);
		//FindMajorAxis();
		//Mat matData = StlToRegularGrid(m_primBuffer, m_primCount, m_majorAxis);
		//StlFromMat(StlToRegularGrid(&m_faceBuffer, m_majorAxis));
		return true;
	}

	return false;
}

void CStlReader::GetMinMax(std::vector<primTriangle>* pfaceBuffer, float* xMin, float* xMax, float* yMin, float* yMax, float* zMin, float* zMax)
{
	if (pfaceBuffer->size() < 1)
		return;

	*xMin = *xMax = pfaceBuffer->at(0).vertxVect[0].v[0];
	*yMin = *yMax = pfaceBuffer->at(0).vertxVect[0].v[1];
	*zMin = *zMax = pfaceBuffer->at(0).vertxVect[0].v[2];

	for (long i = 0; i < pfaceBuffer->size(); i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (pfaceBuffer->at(i).vertxVect[j].v[0] < *xMin)
				*xMin = pfaceBuffer->at(i).vertxVect[j].v[0];
			if (pfaceBuffer->at(i).vertxVect[j].v[0] > *xMax)
				*xMax = pfaceBuffer->at(i).vertxVect[j].v[0];
			if (pfaceBuffer->at(i).vertxVect[j].v[1] < *yMin)
				*yMin = pfaceBuffer->at(i).vertxVect[j].v[1];
			if (pfaceBuffer->at(i).vertxVect[j].v[1] > *yMax)
				*yMax = pfaceBuffer->at(i).vertxVect[j].v[1];
			if (pfaceBuffer->at(i).vertxVect[j].v[2] < *zMin)
				*zMin = pfaceBuffer->at(i).vertxVect[j].v[2];
			if (pfaceBuffer->at(i).vertxVect[j].v[2] > *zMax)
				*zMax = pfaceBuffer->at(i).vertxVect[j].v[2];
		}
	}
}

Mat CStlReader::StlToRegularGrid(std::vector<primTriangle>* pfaceBuffer, int mainAxis)
{
	float x, y, z;
	float xMin, xMax, yMin, yMax, zMin, zMax;
	xMin = xMax = yMin = yMax = zMin = zMax = 0.0f;
	GetMinMax(pfaceBuffer, &xMin, &xMax, &yMin, &yMax, &zMin, &zMax);
	int cx = 1200;
	int cy = 800;

	float fXY;
	if (cx > cy)
		fXY = float(cx - 12);
	else
		fXY = float(cy - 12);
	Mat matData = Mat(cy, cx, CV_32F, Scalar(-0.5));
	Mat matCount = Mat(cy, cx, CV_32F, Scalar(0));


	for (long i = 0; i < pfaceBuffer->size(); i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (0 == mainAxis)
			{
				x = (pfaceBuffer->at(i).vertxVect[j].v[1] - yMin) / (yMax - yMin);
				y = (pfaceBuffer->at(i).vertxVect[j].v[2] - zMin) / (zMax - zMin);
				z = (pfaceBuffer->at(i).vertxVect[j].v[0] - xMin) / (xMax - xMin);
			}
			if (1 == mainAxis)
			{
				x = (pfaceBuffer->at(i).vertxVect[j].v[0] - xMin) / (xMax - xMin);
				y = (pfaceBuffer->at(i).vertxVect[j].v[2] - zMin) / (zMax - zMin);
				z = (pfaceBuffer->at(i).vertxVect[j].v[1] - yMin) / (yMax - yMin);
			}
			if (2 == mainAxis)
			{
				x = (pfaceBuffer->at(i).vertxVect[j].v[0] - xMin) / (xMax - xMin);
				y = (pfaceBuffer->at(i).vertxVect[j].v[1] - yMin) / (yMax - yMin);
				z = (pfaceBuffer->at(i).vertxVect[j].v[2] - zMin) / (zMax - zMin);
			}
			x = fXY*x;
			y = fXY*y;
			if (matData.at<float>(y, x) < z)
				matData.at<float>(y, x) = z;
			//matData.at<float>(y, x) = matData.at<float>(y, x) + z;
			matCount.at<float>(y, x) = matCount.at<float>(y, x) + 1.0f;
		}
	}

	Mat matTemp;

	cv::normalize(matData, matTemp, 0, 255, NORM_MINMAX, CV_8UC1);
	cv::imshow(std::string("Gray"), matTemp);
	//cv::applyColorMap(matTemp, m_matColorMap, COLORMAP_JET);
	//cv::imshow(std::string("Mat"), m_matColorMap);
	cv::waitKey(5);

	return matData;
}




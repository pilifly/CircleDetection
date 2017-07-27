#pragma once
#include "CVHeader.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

class CStlReader
{
	struct vect3f { float v[3]; };
	struct vertx { float v[3]; int id; };
	struct primTriangle {

		vect3f normVect;
		vertx vertxVect[3];
		vect3f colorVect[3];
		UINT8 attribByteCount;
		int faceID;
	};

	struct stlFace
	{
		vect3f normVect;
		vect3f vertxVect[3];
		UINT8 attribByteCount;
	};

public:
	CStlReader();
	~CStlReader();

	void ReadStlFile();
	bool ReadBinaryStl(CString fileName);

protected:
	void GetMinMax(std::vector<primTriangle>* pfaceBuffer, float* xMin, float* xMax, float* yMin, float* yMax, float* zMin, float* zMax);
	Mat StlToRegularGrid(std::vector<primTriangle>* pfaceBuffer, int mainAxis);

	char m_headerBuff[80];
	std::vector<primTriangle> m_faceBuffer;
	float m_xMin, m_xMax, m_yMin, m_yMax, m_zMin, m_zMax;

};


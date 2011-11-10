//BEGIN_INCLUDE(all)
#include <jni.h>
#include <errno.h>
#include <dlfcn.h>
#include <stdio.h>
#include <string.h>
#include <android/log.h>
#include <math.h>

#include <EGL/egl.h>
#include <GLES/gl.h>

#include <android/sensor.h>
#include <android/log.h>
#include <android_native_app_glue.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))

/////////////////////////////////////////////////////////////////
class JoystickHelper
{
	float m_fMidX, m_fMidY;
	float m_fRadius;

	float m_fX, m_fY;
	int m_nID;

public:

	JoystickHelper(float fMidX, float fMidY, float fRadius)
	{
		m_fMidX = fMidX;
		m_fMidY = fMidY;
		m_fRadius = fRadius;
		m_nID = -1;
		m_fX = fMidX;
		m_fY = fMidY;
	}

	bool OnTouchBegin(int nID, float fX, float fY)
	{
		if (m_nID == -1)
		{
			if (fabs(m_fMidX-fX) < m_fRadius*1.5f &&
				fabs(m_fMidY-fY) < m_fRadius*1.5f)
			{
				m_nID = nID;
				m_fX = fX;
				m_fY = fY;
				return true;
			}
		}
		return false;
	}

	bool OnTouchMove(int nID, float fX, float fY)
	{
		if (m_nID != nID)
			return false;
		m_fX = fX;
		m_fY = fY;
		return true;
	}

	bool OnTouchEnd(int nID, float fX, float fY)
	{
		if (m_nID != nID)
			return false;
		m_nID = -1;
		m_fX = m_fMidX;
		m_fY = m_fMidY;
		return true;
	}

	bool OnTouchCancel(int nID, float fX, float fY)
	{
		if (m_nID != nID)
			return false;
		m_nID = -1;
		m_fX = m_fMidX;
		m_fY = m_fMidY;
		return true;
	}

	void GetJoystickValue(float& fX, float& fY)
	{
		fX = (m_fX-m_fMidX)/m_fRadius*1.25f;
		fY = -(m_fY-m_fMidY)/m_fRadius*1.25f;
		float fLen = sqrt(fX*fX+fY*fY);
		if (fLen > 1.0f)
		{
			fX /= fLen;
			fY /= fLen;
		}
	}
};
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
JoystickHelper* g_joys[2] = {NULL, NULL};
///////////////////////////////////////////////////////////////// 

/////////////////////////////////////////////////////////////////
extern "C" void InitJoysticks()
{
	g_joys[0] = new JoystickHelper(966/5,360/2,130);
	g_joys[1] = new JoystickHelper(966-966/5,360/2,130);
}
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
extern "C" void TermJoysticks()
{
	delete g_joys[0];
	g_joys[0] = NULL;
	delete g_joys[1];
	g_joys[1] = NULL;
}
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
extern "C" int JoystickOnTouchBegin(int nID, float fX, float fY)
{
	if (g_joys[0] == NULL || g_joys[1] == NULL)
		return 0;
	
	if (!g_joys[0]->OnTouchBegin(nID, fX, fY))
	{
		if (!g_joys[1]->OnTouchBegin(nID, fX, fY))
			return 0;
	}
	
	return 1;
}
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
extern "C" int JoystickOnTouchMove(int nID, float fX, float fY)
{
	if (g_joys[0] == NULL || g_joys[1] == NULL)
		return 0;
	
	if (!g_joys[0]->OnTouchMove(nID, fX, fY))
	{
		if (!g_joys[1]->OnTouchMove(nID, fX, fY))
			return 0;
	}
	
	return 1;
}
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
extern "C" int JoystickOnTouchEnd(int nID, float fX, float fY)
{
	if (g_joys[0] == NULL || g_joys[1] == NULL)
		return 0;
	
	if (!g_joys[0]->OnTouchEnd(nID, fX, fY))
	{
		if (!g_joys[1]->OnTouchEnd(nID, fX, fY))
			return 0;
	}
	
	return 1;
}
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
extern "C" int JoystickOnTouchCancel(int nID, float fX, float fY)
{
	if (g_joys[0] == NULL || g_joys[1] == NULL)
		return 0;
	
	if (!g_joys[0]->OnTouchCancel(nID, fX, fY))
	{
		if (!g_joys[1]->OnTouchCancel(nID, fX, fY))
			return 0;
	}
	
	return 1;
}
/////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////
extern "C" int JoystickGetValue(int nJoystick, float *fX, float *fY)
{
	if (fX == NULL || fY == NULL)
		return 0;
	if (nJoystick < 0 || nJoystick > 1)
		return 0;
	g_joys[nJoystick]->GetJoystickValue(*fX,*fY);
	return 1;
}
/////////////////////////////////////////////////////////////////

//END_INCLUDE(all)

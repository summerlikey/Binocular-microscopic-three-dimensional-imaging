#ifndef APICONTROL_H
#define APICONTROL_H

#include <string>

#include <VimbaCPP/Include/VimbaCPP.h>
class ApiControl
{
public:
    ApiControl();
    ~ApiControl();
    int GetWidth() const;
    int GetHeight() const;
    VmbPixelFormatType  GetPixelFormat() const;
private:
  // The current pixel format
  VmbInt64_t                  m_nPixelFormat;
  // The current width
  VmbInt64_t                  m_nWidth;
  // The current height
  VmbInt64_t                  m_nHeight;
};

#endif // APICONTROL_H

#include "apicontrol.h"
#include "VimbaCPP/Include/VimbaCPP.h"
using namespace AVT::VmbAPI;
ApiControl::ApiControl()
{

}
int ApiControl::GetWidth() const
{
    return static_cast<int>(m_nWidth);
}
int ApiControl::GetHeight() const
{
    return static_cast<int>(m_nWidth);
}
VmbPixelFormatType ApiControl::GetPixelFormat() const
{
    return static_cast<VmbPixelFormatType>(m_nPixelFormat);
}

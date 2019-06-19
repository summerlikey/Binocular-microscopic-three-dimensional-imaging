/*=============================================================================
  Copyright (C) 2014 - 2016 Allied Vision Technologies.  All Rights Reserved.

  Redistribution of this file, in original or modified form, without
  prior written consent of Allied Vision Technologies is prohibited.

-------------------------------------------------------------------------------

  File:        ErrorCodeToMessage.h

  Description: Convert the error codes to a self-explanatory message.

-------------------------------------------------------------------------------

  THIS SOFTWARE IS PROVIDED BY THE AUTHOR "AS IS" AND ANY EXPRESS OR IMPLIED
  WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF TITLE,
  NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A PARTICULAR  PURPOSE ARE
  DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
  TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=============================================================================*/

#ifndef ERROR_CODE_TO_MESSAGE_H_
#define ERROR_CODE_TO_MESSAGE_H_

#include <string>

#include "VimbaCPP/Include/VimbaCPP.h"

//
// Translates Vimba error codes to readable error messages
//
// Parameters:
//  [in]    eError      The error code to be converted to string
//
// Returns:
//  A descriptive string representation of the error code
//
inline std::string ErrorCodeToMessage( VmbError_t eError )
{
    std::string res;
    switch( eError )
    {
    case VmbErrorSuccess:
        res="success";
        return res;
    case VmbErrorInternalFault:
        res= "Unexpected fault in VmbApi or driver.";
        return res;
    case VmbErrorApiNotStarted:
        res="API not started.";
        return res;
    case VmbErrorNotFound:
        res="Not found.";
        return res;
    case VmbErrorBadHandle:
        res="Device not open";
        return res;
    case VmbErrorDeviceNotOpen:
        res="Device not open";
        return res;
    case VmbErrorInvalidAccess:
        res="Invalid access";
        return res;
    case VmbErrorBadParameter:
        res="Bad parameter";
        return res;
    case VmbErrorStructSize:
        res="Wrong DLL version";
        return res;
    case VmbErrorMoreData:
        res="More data returned than memory provided.";
        return res;
    case VmbErrorWrongType:
        res="Wrong type.";
        return res;
    case VmbErrorInvalidValue:
        res="Invalid value.";
        return res;
    case VmbErrorTimeout:
        res="Timeout.";
        return res;
    case VmbErrorOther:
        res="TL error.";
        return res;
    case VmbErrorResources:
        res="Resource not available.";
        return res;
    case VmbErrorInvalidCall:
        res="Invalid call.";
        return res;
    case VmbErrorNoTL:
        res="TL not loaded.";
        return res;
    case VmbErrorNotImplemented:
        res="Not implemented.";
        return res;
    case VmbErrorNotSupported:
        res="Not supported.";
        return res;
    default:
        res="Unknown";
        return res;
    }
}

#endif

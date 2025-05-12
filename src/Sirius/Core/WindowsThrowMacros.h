//
// Created by Leon on 02/02/2025.
//

#ifndef WINDOWSTHROWMACROS_H
#define WINDOWSTHROWMACROS_H
#define CHWND_EXCEPT( hr ) GfxDevice::HrException( __LINE__,__FILE__,hr )
#define CHWND_LAST_EXCEPT() GfxDevice::HrException( __LINE__,__FILE__,GetLastError() )
#define CHWND_NOGFX_EXCEPT() GfxDevice::NoGfxHrException( __LINE__,__FILE__ )
#endif //WINDOWSTHROWMACROS_H

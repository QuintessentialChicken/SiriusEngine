//
// Created by Leon on 02/02/2025.
//

#ifndef WINDOWSTHROWMACROS_H
#define WINDOWSTHROWMACROS_H
#define CHWND_EXCEPT( hr ) Window::HrException( __LINE__,__FILE__,hr )
#define CHWND_LAST_EXCEPT() Window::HrException( __LINE__,__FILE__,GetLastError() )
#define CHWND_NOGFX_EXCEPT() Window::NoGfxHrException( __LINE__,__FILE__ )
#endif //WINDOWSTHROWMACROS_H

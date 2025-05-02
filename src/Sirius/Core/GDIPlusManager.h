//
// Created by Lms on 05/02/2025.
//

#ifndef GDIPLUSMANAGER_H
#define GDIPLUSMANAGER_H
#include <intsafe.h>


class GDIPlusManager {
public:
    GDIPlusManager();
    ~GDIPlusManager();
private:
    static ULONG_PTR token;
    static int refCount;
};



#endif //GDIPLUSMANAGER_H

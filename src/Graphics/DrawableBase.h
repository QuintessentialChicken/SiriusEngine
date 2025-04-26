// //
// // Created by Leon on 02/02/2025.
// //
//
// #ifndef DRAWABLEBASE_H
// #define DRAWABLEBASE_H
//
// #include "Drawable.h"
// #include "IndexBuffer.h"
//
// /*
//  *  Helper class to store staticBinds per Drawable (Cube, Sphere, etc.) instead of having one for all Drawables
//  *  This works because each Drawable inherits from DrawableBase using themselves as the template type.
//  *  The compiler then creates DrawableBase<Cube>::staticBinds, DrawableBase<Sphere>::staticBinds, etc.
//  */
// template<class T>
// class DrawableBase : public Drawable
// {
// protected:
//     static bool IsStaticInitialized() noexcept
//     {
//         return !staticBinds.empty();
//     }
//     static void AddStaticBind( std::unique_ptr<Bindable> bind ) noexcept
//     {
//         assert( "*Must* use AddStaticIndexBuffer to bind index buffer" && typeid(*bind) != typeid(IndexBuffer) );
//         staticBinds.push_back( std::move( bind ) );
//     }
//     void AddStaticIndexBuffer( std::unique_ptr<IndexBuffer> ibuf ) noexcept
//     {
//         assert( "Attempting to add index buffer a second time" && indexBuffer == nullptr );
//         indexBuffer = ibuf.get();
//         staticBinds.push_back( std::move( ibuf ) );
//     }
//     void SetIndexFromStatic() noexcept
//     {
//         assert( "Attempting to add index buffer a second time" && indexBuffer == nullptr );
//         for( const auto& b : staticBinds )
//         {
//             if( const auto p = dynamic_cast<IndexBuffer*>(b.get()) )
//             {
//                 indexBuffer = p;
//                 return;
//             }
//         }
//         assert( "Failed to find index buffer in static binds" && indexBuffer != nullptr );
//     }
// private:
//     [[nodiscard]] const std::vector<std::unique_ptr<Bindable>>& GetStaticBinds() const noexcept override
//     {
//         return staticBinds;
//     }
//     static std::vector<std::unique_ptr<Bindable>> staticBinds;
// };
//
// template<class T>
// std::vector<std::unique_ptr<Bindable>> DrawableBase<T>::staticBinds;
//
// #endif //DRAWABLEBASE_H

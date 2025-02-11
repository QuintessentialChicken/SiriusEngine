//
// Created by Leon on 02/02/2025.
//

#include "Drawable.h"

#include "Bindable.h"
#include "IndexBuffer.h"

void Drawable::Draw(Graphics& gfx) const noexcept {
    for( auto& bind : binds )
    {
        bind->Bind( gfx );
    }
    for( auto& bind : GetStaticBinds() )
    {
        bind->Bind( gfx );
    }
    gfx.DrawIndexed( indexBuffer->GetCount() );
}
void Drawable::AddBind(std::unique_ptr<Bindable> bind) noexcept {
    assert( "*Must* use AddIndexBuffer to bind index buffer" && typeid(*bind) != typeid(IndexBuffer) );
    binds.push_back( std::move( bind ) );
}

void Drawable::AddIndexBuffer(std::unique_ptr<IndexBuffer> ibuf) noexcept {
    assert( "Attempting to add index buffer a second time" && indexBuffer == nullptr );
    indexBuffer = ibuf.get();
    binds.push_back( std::move( ibuf ) );
}
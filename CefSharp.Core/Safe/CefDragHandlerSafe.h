

#pragma once

#include "include/cef_drag_handler.h"

template<typename TImplementation=CefDragHandler>
class CefDragHandlerSafeType : public CefDragHandler {
public:
    CefDragHandlerSafeType(const CefRefPtr<CefDragHandler> wrapped) {
        this->wrapped = wrapped.get();
        this->wrapped->AddRef();
        this->appDomainId = System::AppDomain::CurrentDomain->Id;
    }

    ~CefDragHandlerSafeType() {
        this->wrapped->Release();
    }

    TImplementation* operator ->() const {
        return static_cast<TImplementation*>(wrapped);
    }

    virtual void AddRef() const OVERRIDE { 
        return refct.AddRef(); 
    }

    virtual bool Release() const OVERRIDE {
        bool retval = refct.Release();
        if (retval)
            if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
                msclr::call_in_appdomain(appDomainId, &_Release, this);
            } else {
                _Release(this);
            }
        return retval;
    }

    virtual bool HasOneRef() const OVERRIDE {
        return refct.HasOneRef(); 
    }

    virtual bool OnDragEnter (CefRefPtr<CefBrowser> browser, CefRefPtr<CefDragData> dragData, DragOperationsMask mask) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
            return  msclr::call_in_appdomain(appDomainId, &_OnDragEnter, wrapped, browser, dragData, mask);
        } else {
            return _OnDragEnter(wrapped, browser, dragData, mask);
        }
    }

    virtual void OnDraggableRegionsChanged (CefRefPtr<CefBrowser> browser, const int & regions) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
             msclr::call_in_appdomain(appDomainId, &_OnDraggableRegionsChanged, wrapped, browser, regions);
        } else {
            _OnDraggableRegionsChanged(wrapped, browser, regions);
        }
    }

private:
    CefRefCount refct;
    CefDragHandler* wrapped;
    int appDomainId;

    static void _Release(const CefDragHandlerSafeType* _this) {
        delete _this;
    }


    static bool _OnDragEnter (CefDragHandler* wrapped, CefRefPtr<CefBrowser> browser, CefRefPtr<CefDragData> dragData, DragOperationsMask mask) {
        return  wrapped->OnDragEnter(browser, dragData, mask);
    }


    static void _OnDraggableRegionsChanged (CefDragHandler* wrapped, CefRefPtr<CefBrowser> browser, int  regions) {
         wrapped->OnDraggableRegionsChanged(browser, regions);
    }

};

typedef CefDragHandlerSafeType<> CefDragHandlerSafe;


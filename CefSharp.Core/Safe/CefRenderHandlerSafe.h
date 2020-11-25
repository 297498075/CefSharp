

#pragma once

#include "include/cef_render_handler.h"

template<typename TImplementation=CefRenderHandler>
class CefRenderHandlerSafeType : public CefRenderHandler {
public:
    CefRenderHandlerSafeType(const CefRefPtr<CefRenderHandler> wrapped) {
        this->wrapped = wrapped.get();
        this->wrapped->AddRef();
        this->appDomainId = System::AppDomain::CurrentDomain->Id;
    }

    ~CefRenderHandlerSafeType() {
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

    virtual bool GetRootScreenRect (CefRefPtr<CefBrowser> browser, CefRect & rect) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
            return  msclr::call_in_appdomain(appDomainId, &_GetRootScreenRect, wrapped, browser, &rect);
        } else {
            return _GetRootScreenRect(wrapped, browser, &rect);
        }
    }

    virtual bool GetViewRect (CefRefPtr<CefBrowser> browser, CefRect & rect) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
            return  msclr::call_in_appdomain(appDomainId, &_GetViewRect, wrapped, browser, &rect);
        } else {
            return _GetViewRect(wrapped, browser, &rect);
        }
    }

    virtual bool GetScreenPoint (CefRefPtr<CefBrowser> browser, int viewX, int viewY, int & screenX, int & screenY) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
            return  msclr::call_in_appdomain(appDomainId, &_GetScreenPoint, wrapped, browser, viewX, viewY, &screenX, &screenY);
        } else {
            return _GetScreenPoint(wrapped, browser, viewX, viewY, &screenX, &screenY);
        }
    }

    virtual bool GetScreenInfo (CefRefPtr<CefBrowser> browser, CefScreenInfo & screen_info) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
            return  msclr::call_in_appdomain(appDomainId, &_GetScreenInfo, wrapped, browser, &screen_info);
        } else {
            return _GetScreenInfo(wrapped, browser, &screen_info);
        }
    }

    virtual void OnPopupShow (CefRefPtr<CefBrowser> browser, bool show) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
             msclr::call_in_appdomain(appDomainId, &_OnPopupShow, wrapped, browser, show);
        } else {
            _OnPopupShow(wrapped, browser, show);
        }
    }

    virtual void OnPopupSize (CefRefPtr<CefBrowser> browser, const CefRect & rect) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
             msclr::call_in_appdomain(appDomainId, &_OnPopupSize, wrapped, browser, rect);
        } else {
            _OnPopupSize(wrapped, browser, rect);
        }
    }

    virtual void OnPaint (CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList & dirtyRects, const void * buffer, int width, int height) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
             msclr::call_in_appdomain(appDomainId, &_OnPaint, wrapped, browser, type, dirtyRects, buffer, width, height);
        } else {
            _OnPaint(wrapped, browser, type, dirtyRects, buffer, width, height);
        }
    }

    virtual void OnCursorChange (CefRefPtr<CefBrowser> browser, HCURSOR cursor, CursorType type, const CefCursorInfo & custom_cursor_info) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
             msclr::call_in_appdomain(appDomainId, &_OnCursorChange, wrapped, browser, cursor, type, custom_cursor_info);
        } else {
            _OnCursorChange(wrapped, browser, cursor, type, custom_cursor_info);
        }
    }

    virtual bool StartDragging (CefRefPtr<CefBrowser> browser, CefRefPtr<CefDragData> drag_data, DragOperationsMask allowed_ops, int x, int y) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
            return  msclr::call_in_appdomain(appDomainId, &_StartDragging, wrapped, browser, drag_data, allowed_ops, x, y);
        } else {
            return _StartDragging(wrapped, browser, drag_data, allowed_ops, x, y);
        }
    }

    virtual void UpdateDragCursor (CefRefPtr<CefBrowser> browser, DragOperation operation) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
             msclr::call_in_appdomain(appDomainId, &_UpdateDragCursor, wrapped, browser, operation);
        } else {
            _UpdateDragCursor(wrapped, browser, operation);
        }
    }

    virtual void OnScrollOffsetChanged (CefRefPtr<CefBrowser> browser, double x, double y) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
             msclr::call_in_appdomain(appDomainId, &_OnScrollOffsetChanged, wrapped, browser, x, y);
        } else {
            _OnScrollOffsetChanged(wrapped, browser, x, y);
        }
    }

private:
    CefRefCount refct;
    CefRenderHandler* wrapped;
    int appDomainId;

    static void _Release(const CefRenderHandlerSafeType* _this) {
        delete _this;
    }


    static bool _GetRootScreenRect (CefRenderHandler* wrapped, CefRefPtr<CefBrowser> browser, CefRect * rect) {
        return  wrapped->GetRootScreenRect(browser, *rect);
    }


    static bool _GetViewRect (CefRenderHandler* wrapped, CefRefPtr<CefBrowser> browser, CefRect * rect) {
        return  wrapped->GetViewRect(browser, *rect);
    }


    static bool _GetScreenPoint (CefRenderHandler* wrapped, CefRefPtr<CefBrowser> browser, int viewX, int viewY, int * screenX, int * screenY) {
        return  wrapped->GetScreenPoint(browser, viewX, viewY, *screenX, *screenY);
    }


    static bool _GetScreenInfo (CefRenderHandler* wrapped, CefRefPtr<CefBrowser> browser, CefScreenInfo * screen_info) {
        return  wrapped->GetScreenInfo(browser, *screen_info);
    }


    static void _OnPopupShow (CefRenderHandler* wrapped, CefRefPtr<CefBrowser> browser, bool show) {
         wrapped->OnPopupShow(browser, show);
    }


    static void _OnPopupSize (CefRenderHandler* wrapped, CefRefPtr<CefBrowser> browser, CefRect  rect) {
         wrapped->OnPopupSize(browser, rect);
    }


    static void _OnPaint (CefRenderHandler* wrapped, CefRefPtr<CefBrowser> browser, PaintElementType type, RectList  dirtyRects, const void * buffer, int width, int height) {
         wrapped->OnPaint(browser, type, dirtyRects, buffer, width, height);
    }


    static void _OnCursorChange (CefRenderHandler* wrapped, CefRefPtr<CefBrowser> browser, HCURSOR cursor, CursorType type, CefCursorInfo  custom_cursor_info) {
         wrapped->OnCursorChange(browser, cursor, type, custom_cursor_info);
    }


    static bool _StartDragging (CefRenderHandler* wrapped, CefRefPtr<CefBrowser> browser, CefRefPtr<CefDragData> drag_data, DragOperationsMask allowed_ops, int x, int y) {
        return  wrapped->StartDragging(browser, drag_data, allowed_ops, x, y);
    }


    static void _UpdateDragCursor (CefRenderHandler* wrapped, CefRefPtr<CefBrowser> browser, DragOperation operation) {
         wrapped->UpdateDragCursor(browser, operation);
    }


    static void _OnScrollOffsetChanged (CefRenderHandler* wrapped, CefRefPtr<CefBrowser> browser, double x, double y) {
         wrapped->OnScrollOffsetChanged(browser, x, y);
    }

};

typedef CefRenderHandlerSafeType<> CefRenderHandlerSafe;


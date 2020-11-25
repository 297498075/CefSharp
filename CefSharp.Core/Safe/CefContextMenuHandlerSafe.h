

#pragma once

#include "include/cef_context_menu_handler.h"

template<typename TImplementation=CefContextMenuHandler>
class CefContextMenuHandlerSafeType : public CefContextMenuHandler {
public:
    CefContextMenuHandlerSafeType(const CefRefPtr<CefContextMenuHandler> wrapped) {
        this->wrapped = wrapped.get();
        this->wrapped->AddRef();
        this->appDomainId = System::AppDomain::CurrentDomain->Id;
    }

    ~CefContextMenuHandlerSafeType() {
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

    virtual void OnBeforeContextMenu (CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefContextMenuParams> params, CefRefPtr<CefMenuModel> model) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
             msclr::call_in_appdomain(appDomainId, &_OnBeforeContextMenu, wrapped, browser, frame, params, model);
        } else {
            _OnBeforeContextMenu(wrapped, browser, frame, params, model);
        }
    }

    virtual bool RunContextMenu (CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefContextMenuParams> params, CefRefPtr<CefMenuModel> model, CefRefPtr<CefRunContextMenuCallback> callback) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
            return  msclr::call_in_appdomain(appDomainId, &_RunContextMenu, wrapped, browser, frame, params, model, callback);
        } else {
            return _RunContextMenu(wrapped, browser, frame, params, model, callback);
        }
    }

    virtual bool OnContextMenuCommand (CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefContextMenuParams> params, int command_id, EventFlags event_flags) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
            return  msclr::call_in_appdomain(appDomainId, &_OnContextMenuCommand, wrapped, browser, frame, params, command_id, event_flags);
        } else {
            return _OnContextMenuCommand(wrapped, browser, frame, params, command_id, event_flags);
        }
    }

    virtual void OnContextMenuDismissed (CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
             msclr::call_in_appdomain(appDomainId, &_OnContextMenuDismissed, wrapped, browser, frame);
        } else {
            _OnContextMenuDismissed(wrapped, browser, frame);
        }
    }

private:
    CefRefCount refct;
    CefContextMenuHandler* wrapped;
    int appDomainId;

    static void _Release(const CefContextMenuHandlerSafeType* _this) {
        delete _this;
    }


    static void _OnBeforeContextMenu (CefContextMenuHandler* wrapped, CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefContextMenuParams> params, CefRefPtr<CefMenuModel> model) {
         wrapped->OnBeforeContextMenu(browser, frame, params, model);
    }


    static bool _RunContextMenu (CefContextMenuHandler* wrapped, CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefContextMenuParams> params, CefRefPtr<CefMenuModel> model, CefRefPtr<CefRunContextMenuCallback> callback) {
        return  wrapped->RunContextMenu(browser, frame, params, model, callback);
    }


    static bool _OnContextMenuCommand (CefContextMenuHandler* wrapped, CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefContextMenuParams> params, int command_id, EventFlags event_flags) {
        return  wrapped->OnContextMenuCommand(browser, frame, params, command_id, event_flags);
    }


    static void _OnContextMenuDismissed (CefContextMenuHandler* wrapped, CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame) {
         wrapped->OnContextMenuDismissed(browser, frame);
    }

};

typedef CefContextMenuHandlerSafeType<> CefContextMenuHandlerSafe;


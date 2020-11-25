

#pragma once

#include "include/cef_focus_handler.h"

template<typename TImplementation=CefFocusHandler>
class CefFocusHandlerSafeType : public CefFocusHandler {
public:
    CefFocusHandlerSafeType(const CefRefPtr<CefFocusHandler> wrapped) {
        this->wrapped = wrapped.get();
        this->wrapped->AddRef();
        this->appDomainId = System::AppDomain::CurrentDomain->Id;
    }

    ~CefFocusHandlerSafeType() {
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

    virtual void OnTakeFocus (CefRefPtr<CefBrowser> browser, bool next) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
             msclr::call_in_appdomain(appDomainId, &_OnTakeFocus, wrapped, browser, next);
        } else {
            _OnTakeFocus(wrapped, browser, next);
        }
    }

    virtual bool OnSetFocus (CefRefPtr<CefBrowser> browser, FocusSource source) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
            return  msclr::call_in_appdomain(appDomainId, &_OnSetFocus, wrapped, browser, source);
        } else {
            return _OnSetFocus(wrapped, browser, source);
        }
    }

    virtual void OnGotFocus (CefRefPtr<CefBrowser> browser) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
             msclr::call_in_appdomain(appDomainId, &_OnGotFocus, wrapped, browser);
        } else {
            _OnGotFocus(wrapped, browser);
        }
    }

private:
    CefRefCount refct;
    CefFocusHandler* wrapped;
    int appDomainId;

    static void _Release(const CefFocusHandlerSafeType* _this) {
        delete _this;
    }


    static void _OnTakeFocus (CefFocusHandler* wrapped, CefRefPtr<CefBrowser> browser, bool next) {
         wrapped->OnTakeFocus(browser, next);
    }


    static bool _OnSetFocus (CefFocusHandler* wrapped, CefRefPtr<CefBrowser> browser, FocusSource source) {
        return  wrapped->OnSetFocus(browser, source);
    }


    static void _OnGotFocus (CefFocusHandler* wrapped, CefRefPtr<CefBrowser> browser) {
         wrapped->OnGotFocus(browser);
    }

};

typedef CefFocusHandlerSafeType<> CefFocusHandlerSafe;




#pragma once

#include "include/cef_keyboard_handler.h"

template<typename TImplementation=CefKeyboardHandler>
class CefKeyboardHandlerSafeType : public CefKeyboardHandler {
public:
    CefKeyboardHandlerSafeType(const CefRefPtr<CefKeyboardHandler> wrapped) {
        this->wrapped = wrapped.get();
        this->wrapped->AddRef();
        this->appDomainId = System::AppDomain::CurrentDomain->Id;
    }

    ~CefKeyboardHandlerSafeType() {
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

    virtual bool OnPreKeyEvent (CefRefPtr<CefBrowser> browser, const CefKeyEvent & event, MSG * os_event, bool * is_keyboard_shortcut) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
            return  msclr::call_in_appdomain(appDomainId, &_OnPreKeyEvent, wrapped, browser, event, os_event, is_keyboard_shortcut);
        } else {
            return _OnPreKeyEvent(wrapped, browser, event, os_event, is_keyboard_shortcut);
        }
    }

    virtual bool OnKeyEvent (CefRefPtr<CefBrowser> browser, const CefKeyEvent & event, MSG * os_event) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
            return  msclr::call_in_appdomain(appDomainId, &_OnKeyEvent, wrapped, browser, event, os_event);
        } else {
            return _OnKeyEvent(wrapped, browser, event, os_event);
        }
    }

private:
    CefRefCount refct;
    CefKeyboardHandler* wrapped;
    int appDomainId;

    static void _Release(const CefKeyboardHandlerSafeType* _this) {
        delete _this;
    }


    static bool _OnPreKeyEvent (CefKeyboardHandler* wrapped, CefRefPtr<CefBrowser> browser, CefKeyEvent  event, MSG * os_event, bool * is_keyboard_shortcut) {
        return  wrapped->OnPreKeyEvent(browser, event, os_event, is_keyboard_shortcut);
    }


    static bool _OnKeyEvent (CefKeyboardHandler* wrapped, CefRefPtr<CefBrowser> browser, CefKeyEvent  event, MSG * os_event) {
        return  wrapped->OnKeyEvent(browser, event, os_event);
    }

};

typedef CefKeyboardHandlerSafeType<> CefKeyboardHandlerSafe;


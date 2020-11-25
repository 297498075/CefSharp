

#pragma once

#include "include/cef_geolocation_handler.h"

template<typename TImplementation=CefGeolocationHandler>
class CefGeolocationHandlerSafeType : public CefGeolocationHandler {
public:
    CefGeolocationHandlerSafeType(const CefRefPtr<CefGeolocationHandler> wrapped) {
        this->wrapped = wrapped.get();
        this->wrapped->AddRef();
        this->appDomainId = System::AppDomain::CurrentDomain->Id;
    }

    ~CefGeolocationHandlerSafeType() {
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

    virtual bool OnRequestGeolocationPermission (CefRefPtr<CefBrowser> browser, const CefString & requesting_url, int request_id, CefRefPtr<CefGeolocationCallback> callback) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
            return  msclr::call_in_appdomain(appDomainId, &_OnRequestGeolocationPermission, wrapped, browser, requesting_url, request_id, callback);
        } else {
            return _OnRequestGeolocationPermission(wrapped, browser, requesting_url, request_id, callback);
        }
    }

    virtual void OnCancelGeolocationPermission (CefRefPtr<CefBrowser> browser, int request_id) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
             msclr::call_in_appdomain(appDomainId, &_OnCancelGeolocationPermission, wrapped, browser, request_id);
        } else {
            _OnCancelGeolocationPermission(wrapped, browser, request_id);
        }
    }

private:
    CefRefCount refct;
    CefGeolocationHandler* wrapped;
    int appDomainId;

    static void _Release(const CefGeolocationHandlerSafeType* _this) {
        delete _this;
    }


    static bool _OnRequestGeolocationPermission (CefGeolocationHandler* wrapped, CefRefPtr<CefBrowser> browser, CefString  requesting_url, int request_id, CefRefPtr<CefGeolocationCallback> callback) {
        return  wrapped->OnRequestGeolocationPermission(browser, requesting_url, request_id, callback);
    }


    static void _OnCancelGeolocationPermission (CefGeolocationHandler* wrapped, CefRefPtr<CefBrowser> browser, int request_id) {
         wrapped->OnCancelGeolocationPermission(browser, request_id);
    }

};

typedef CefGeolocationHandlerSafeType<> CefGeolocationHandlerSafe;


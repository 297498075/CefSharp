

#pragma once

#include "include/cef_request_context.h"
#include "CefResourceHandlerSafe.h"

template<typename TImplementation=CefSchemeHandlerFactory>
class CefSchemeHandlerFactorySafeType : public CefSchemeHandlerFactory {
public:
    CefSchemeHandlerFactorySafeType(const CefRefPtr<CefSchemeHandlerFactory> wrapped) {
        this->wrapped = wrapped.get();
        this->wrapped->AddRef();
        this->appDomainId = System::AppDomain::CurrentDomain->Id;
    }

    ~CefSchemeHandlerFactorySafeType() {
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

    virtual CefRefPtr<CefResourceHandler> Create (CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString & scheme_name, CefRefPtr<CefRequest> request) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
            return  msclr::call_in_appdomain(appDomainId, &_Create, wrapped, browser, frame, scheme_name, request);
        } else {
            return _Create(wrapped, browser, frame, scheme_name, request);
        }
    }

private:
    CefRefCount refct;
    CefSchemeHandlerFactory* wrapped;
    int appDomainId;

    static void _Release(const CefSchemeHandlerFactorySafeType* _this) {
        delete _this;
    }


    static CefRefPtr<CefResourceHandler> _Create (CefSchemeHandlerFactory* wrapped, CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefString  scheme_name, CefRefPtr<CefRequest> request) {
        auto res = wrapped->Create(browser, frame, scheme_name, request);
        if(res) {
            res = new CefResourceHandlerSafe(res);
        }
        return res;
    }

};

typedef CefSchemeHandlerFactorySafeType<> CefSchemeHandlerFactorySafe;


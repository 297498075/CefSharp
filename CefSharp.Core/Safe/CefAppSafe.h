

#pragma once

#include "include/cef_app.h"
#include "CefResourceBundleHandlerSafe.h"
#include "CefBrowserProcessHandlerSafe.h"
#include "CefRenderProcessHandlerSafe.h"

template<typename TImplementation=CefApp>
class CefAppSafeType : public CefApp {
public:
    CefAppSafeType(const CefRefPtr<CefApp> wrapped) {
        this->wrapped = wrapped.get();
        this->wrapped->AddRef();
        this->appDomainId = System::AppDomain::CurrentDomain->Id;
    }

    ~CefAppSafeType() {
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

    virtual void OnBeforeCommandLineProcessing (const CefString & process_type, CefRefPtr<CefCommandLine> command_line) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
             msclr::call_in_appdomain(appDomainId, &_OnBeforeCommandLineProcessing, wrapped, process_type, command_line);
        } else {
            _OnBeforeCommandLineProcessing(wrapped, process_type, command_line);
        }
    }

    virtual void OnRegisterCustomSchemes (CefRefPtr<CefSchemeRegistrar> registrar) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
             msclr::call_in_appdomain(appDomainId, &_OnRegisterCustomSchemes, wrapped, registrar);
        } else {
            _OnRegisterCustomSchemes(wrapped, registrar);
        }
    }

    virtual CefRefPtr<CefResourceBundleHandler> GetResourceBundleHandler () OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
            return  msclr::call_in_appdomain(appDomainId, &_GetResourceBundleHandler, wrapped);
        } else {
            return _GetResourceBundleHandler(wrapped);
        }
    }

    virtual CefRefPtr<CefBrowserProcessHandler> GetBrowserProcessHandler () OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
            return  msclr::call_in_appdomain(appDomainId, &_GetBrowserProcessHandler, wrapped);
        } else {
            return _GetBrowserProcessHandler(wrapped);
        }
    }

    virtual CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler () OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
            return  msclr::call_in_appdomain(appDomainId, &_GetRenderProcessHandler, wrapped);
        } else {
            return _GetRenderProcessHandler(wrapped);
        }
    }

private:
    CefRefCount refct;
    CefApp* wrapped;
    int appDomainId;

    static void _Release(const CefAppSafeType* _this) {
        delete _this;
    }


    static void _OnBeforeCommandLineProcessing (CefApp* wrapped, CefString  process_type, CefRefPtr<CefCommandLine> command_line) {
         wrapped->OnBeforeCommandLineProcessing(process_type, command_line);
    }


    static void _OnRegisterCustomSchemes (CefApp* wrapped, CefRefPtr<CefSchemeRegistrar> registrar) {
         wrapped->OnRegisterCustomSchemes(registrar);
    }


    static CefRefPtr<CefResourceBundleHandler> _GetResourceBundleHandler (CefApp* wrapped) {
        auto res = wrapped->GetResourceBundleHandler();
        if(res) {
            res = new CefResourceBundleHandlerSafe(res);
        }
        return res;
    }


    static CefRefPtr<CefBrowserProcessHandler> _GetBrowserProcessHandler (CefApp* wrapped) {
        auto res = wrapped->GetBrowserProcessHandler();
        if(res) {
            res = new CefBrowserProcessHandlerSafe(res);
        }
        return res;
    }


    static CefRefPtr<CefRenderProcessHandler> _GetRenderProcessHandler (CefApp* wrapped) {
        auto res = wrapped->GetRenderProcessHandler();
        if(res) {
            res = new CefRenderProcessHandlerSafe(res);
        }
        return res;
    }

};

typedef CefAppSafeType<> CefAppSafe;


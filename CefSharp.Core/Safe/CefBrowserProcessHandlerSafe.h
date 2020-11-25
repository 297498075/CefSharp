

#pragma once

#include "include/cef_browser_process_handler.h"
#include "CefPrintHandlerSafe.h"

template<typename TImplementation=CefBrowserProcessHandler>
class CefBrowserProcessHandlerSafeType : public CefBrowserProcessHandler {
public:
    CefBrowserProcessHandlerSafeType(const CefRefPtr<CefBrowserProcessHandler> wrapped) {
        this->wrapped = wrapped.get();
        this->wrapped->AddRef();
        this->appDomainId = System::AppDomain::CurrentDomain->Id;
    }

    ~CefBrowserProcessHandlerSafeType() {
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

    virtual void OnContextInitialized () OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
             msclr::call_in_appdomain(appDomainId, &_OnContextInitialized, wrapped);
        } else {
            _OnContextInitialized(wrapped);
        }
    }

    virtual void OnBeforeChildProcessLaunch (CefRefPtr<CefCommandLine> command_line) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
             msclr::call_in_appdomain(appDomainId, &_OnBeforeChildProcessLaunch, wrapped, command_line);
        } else {
            _OnBeforeChildProcessLaunch(wrapped, command_line);
        }
    }

    virtual void OnRenderProcessThreadCreated (CefRefPtr<CefListValue> extra_info) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
             msclr::call_in_appdomain(appDomainId, &_OnRenderProcessThreadCreated, wrapped, extra_info);
        } else {
            _OnRenderProcessThreadCreated(wrapped, extra_info);
        }
    }

    virtual CefRefPtr<CefPrintHandler> GetPrintHandler () OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
            return  msclr::call_in_appdomain(appDomainId, &_GetPrintHandler, wrapped);
        } else {
            return _GetPrintHandler(wrapped);
        }
    }

private:
    CefRefCount refct;
    CefBrowserProcessHandler* wrapped;
    int appDomainId;

    static void _Release(const CefBrowserProcessHandlerSafeType* _this) {
        delete _this;
    }


    static void _OnContextInitialized (CefBrowserProcessHandler* wrapped) {
         wrapped->OnContextInitialized();
    }


    static void _OnBeforeChildProcessLaunch (CefBrowserProcessHandler* wrapped, CefRefPtr<CefCommandLine> command_line) {
         wrapped->OnBeforeChildProcessLaunch(command_line);
    }


    static void _OnRenderProcessThreadCreated (CefBrowserProcessHandler* wrapped, CefRefPtr<CefListValue> extra_info) {
         wrapped->OnRenderProcessThreadCreated(extra_info);
    }


    static CefRefPtr<CefPrintHandler> _GetPrintHandler (CefBrowserProcessHandler* wrapped) {
        auto res = wrapped->GetPrintHandler();
        if(res) {
            res = new CefPrintHandlerSafe(res);
        }
        return res;
    }

};

typedef CefBrowserProcessHandlerSafeType<> CefBrowserProcessHandlerSafe;




#pragma once

#include "include/cef_request_context_handler.h"

template<typename TImplementation=CefRequestContextHandler>
class CefRequestContextHandlerSafeType : public CefRequestContextHandler {
public:
    CefRequestContextHandlerSafeType(const CefRefPtr<CefRequestContextHandler> wrapped) {
        this->wrapped = wrapped.get();
        this->wrapped->AddRef();
        this->appDomainId = System::AppDomain::CurrentDomain->Id;
    }

    ~CefRequestContextHandlerSafeType() {
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

    virtual CefRefPtr<CefCookieManager> GetCookieManager () OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
            return  msclr::call_in_appdomain(appDomainId, &_GetCookieManager, wrapped);
        } else {
            return _GetCookieManager(wrapped);
        }
    }

    virtual bool OnBeforePluginLoad (const CefString & mime_type, const CefString & plugin_url, const CefString & top_origin_url, CefRefPtr<CefWebPluginInfo> plugin_info, PluginPolicy * plugin_policy) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
            return  msclr::call_in_appdomain(appDomainId, &_OnBeforePluginLoad, wrapped, mime_type, plugin_url, top_origin_url, plugin_info, plugin_policy);
        } else {
            return _OnBeforePluginLoad(wrapped, mime_type, plugin_url, top_origin_url, plugin_info, plugin_policy);
        }
    }

private:
    CefRefCount refct;
    CefRequestContextHandler* wrapped;
    int appDomainId;

    static void _Release(const CefRequestContextHandlerSafeType* _this) {
        delete _this;
    }


    static CefRefPtr<CefCookieManager> _GetCookieManager (CefRequestContextHandler* wrapped) {
        return  wrapped->GetCookieManager();
    }


    static bool _OnBeforePluginLoad (CefRequestContextHandler* wrapped, CefString  mime_type, CefString  plugin_url, CefString  top_origin_url, CefRefPtr<CefWebPluginInfo> plugin_info, PluginPolicy * plugin_policy) {
        return  wrapped->OnBeforePluginLoad(mime_type, plugin_url, top_origin_url, plugin_info, plugin_policy);
    }

};

typedef CefRequestContextHandlerSafeType<> CefRequestContextHandlerSafe;


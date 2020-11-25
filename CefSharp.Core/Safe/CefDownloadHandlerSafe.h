

#pragma once

#include "include/cef_download_handler.h"

template<typename TImplementation=CefDownloadHandler>
class CefDownloadHandlerSafeType : public CefDownloadHandler {
public:
    CefDownloadHandlerSafeType(const CefRefPtr<CefDownloadHandler> wrapped) {
        this->wrapped = wrapped.get();
        this->wrapped->AddRef();
        this->appDomainId = System::AppDomain::CurrentDomain->Id;
    }

    ~CefDownloadHandlerSafeType() {
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

    virtual void OnBeforeDownload (CefRefPtr<CefBrowser> browser, CefRefPtr<CefDownloadItem> download_item, const CefString & suggested_name, CefRefPtr<CefBeforeDownloadCallback> callback) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
             msclr::call_in_appdomain(appDomainId, &_OnBeforeDownload, wrapped, browser, download_item, suggested_name, callback);
        } else {
            _OnBeforeDownload(wrapped, browser, download_item, suggested_name, callback);
        }
    }

    virtual void OnDownloadUpdated (CefRefPtr<CefBrowser> browser, CefRefPtr<CefDownloadItem> download_item, CefRefPtr<CefDownloadItemCallback> callback) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
             msclr::call_in_appdomain(appDomainId, &_OnDownloadUpdated, wrapped, browser, download_item, callback);
        } else {
            _OnDownloadUpdated(wrapped, browser, download_item, callback);
        }
    }

private:
    CefRefCount refct;
    CefDownloadHandler* wrapped;
    int appDomainId;

    static void _Release(const CefDownloadHandlerSafeType* _this) {
        delete _this;
    }


    static void _OnBeforeDownload (CefDownloadHandler* wrapped, CefRefPtr<CefBrowser> browser, CefRefPtr<CefDownloadItem> download_item, CefString  suggested_name, CefRefPtr<CefBeforeDownloadCallback> callback) {
         wrapped->OnBeforeDownload(browser, download_item, suggested_name, callback);
    }


    static void _OnDownloadUpdated (CefDownloadHandler* wrapped, CefRefPtr<CefBrowser> browser, CefRefPtr<CefDownloadItem> download_item, CefRefPtr<CefDownloadItemCallback> callback) {
         wrapped->OnDownloadUpdated(browser, download_item, callback);
    }

};

typedef CefDownloadHandlerSafeType<> CefDownloadHandlerSafe;


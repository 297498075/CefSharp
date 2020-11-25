

#pragma once

#include "include/cef_load_handler.h"

template<typename TImplementation=CefLoadHandler>
class CefLoadHandlerSafeType : public CefLoadHandler {
public:
    CefLoadHandlerSafeType(const CefRefPtr<CefLoadHandler> wrapped) {
        this->wrapped = wrapped.get();
        this->wrapped->AddRef();
        this->appDomainId = System::AppDomain::CurrentDomain->Id;
    }

    ~CefLoadHandlerSafeType() {
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

    virtual void OnLoadingStateChange (CefRefPtr<CefBrowser> browser, bool isLoading, bool canGoBack, bool canGoForward) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
             msclr::call_in_appdomain(appDomainId, &_OnLoadingStateChange, wrapped, browser, isLoading, canGoBack, canGoForward);
        } else {
            _OnLoadingStateChange(wrapped, browser, isLoading, canGoBack, canGoForward);
        }
    }

    virtual void OnLoadStart (CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
             msclr::call_in_appdomain(appDomainId, &_OnLoadStart, wrapped, browser, frame);
        } else {
            _OnLoadStart(wrapped, browser, frame);
        }
    }

    virtual void OnLoadEnd (CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int httpStatusCode) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
             msclr::call_in_appdomain(appDomainId, &_OnLoadEnd, wrapped, browser, frame, httpStatusCode);
        } else {
            _OnLoadEnd(wrapped, browser, frame, httpStatusCode);
        }
    }

    virtual void OnLoadError (CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, ErrorCode errorCode, const CefString & errorText, const CefString & failedUrl) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
             msclr::call_in_appdomain(appDomainId, &_OnLoadError, wrapped, browser, frame, errorCode, errorText, failedUrl);
        } else {
            _OnLoadError(wrapped, browser, frame, errorCode, errorText, failedUrl);
        }
    }

private:
    CefRefCount refct;
    CefLoadHandler* wrapped;
    int appDomainId;

    static void _Release(const CefLoadHandlerSafeType* _this) {
        delete _this;
    }


    static void _OnLoadingStateChange (CefLoadHandler* wrapped, CefRefPtr<CefBrowser> browser, bool isLoading, bool canGoBack, bool canGoForward) {
         wrapped->OnLoadingStateChange(browser, isLoading, canGoBack, canGoForward);
    }


    static void _OnLoadStart (CefLoadHandler* wrapped, CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame) {
         wrapped->OnLoadStart(browser, frame);
    }


    static void _OnLoadEnd (CefLoadHandler* wrapped, CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int httpStatusCode) {
         wrapped->OnLoadEnd(browser, frame, httpStatusCode);
    }


    static void _OnLoadError (CefLoadHandler* wrapped, CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, ErrorCode errorCode, CefString  errorText, CefString  failedUrl) {
         wrapped->OnLoadError(browser, frame, errorCode, errorText, failedUrl);
    }

};

typedef CefLoadHandlerSafeType<> CefLoadHandlerSafe;


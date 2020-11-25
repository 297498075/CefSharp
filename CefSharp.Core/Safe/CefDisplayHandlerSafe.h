

#pragma once

#include "include/cef_display_handler.h"

template<typename TImplementation=CefDisplayHandler>
class CefDisplayHandlerSafeType : public CefDisplayHandler {
public:
    CefDisplayHandlerSafeType(const CefRefPtr<CefDisplayHandler> wrapped) {
        this->wrapped = wrapped.get();
        this->wrapped->AddRef();
        this->appDomainId = System::AppDomain::CurrentDomain->Id;
    }

    ~CefDisplayHandlerSafeType() {
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

    virtual void OnAddressChange (CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString & url) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
             msclr::call_in_appdomain(appDomainId, &_OnAddressChange, wrapped, browser, frame, url);
        } else {
            _OnAddressChange(wrapped, browser, frame, url);
        }
    }

    virtual void OnTitleChange (CefRefPtr<CefBrowser> browser, const CefString & title) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
             msclr::call_in_appdomain(appDomainId, &_OnTitleChange, wrapped, browser, title);
        } else {
            _OnTitleChange(wrapped, browser, title);
        }
    }

    virtual void OnFaviconURLChange (CefRefPtr<CefBrowser> browser, const int & icon_urls) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
             msclr::call_in_appdomain(appDomainId, &_OnFaviconURLChange, wrapped, browser, icon_urls);
        } else {
            _OnFaviconURLChange(wrapped, browser, icon_urls);
        }
    }

    virtual void OnFullscreenModeChange (CefRefPtr<CefBrowser> browser, bool fullscreen) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
             msclr::call_in_appdomain(appDomainId, &_OnFullscreenModeChange, wrapped, browser, fullscreen);
        } else {
            _OnFullscreenModeChange(wrapped, browser, fullscreen);
        }
    }

    virtual bool OnTooltip (CefRefPtr<CefBrowser> browser, CefString & text) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
            return  msclr::call_in_appdomain(appDomainId, &_OnTooltip, wrapped, browser, &text);
        } else {
            return _OnTooltip(wrapped, browser, &text);
        }
    }

    virtual void OnStatusMessage (CefRefPtr<CefBrowser> browser, const CefString & value) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
             msclr::call_in_appdomain(appDomainId, &_OnStatusMessage, wrapped, browser, value);
        } else {
            _OnStatusMessage(wrapped, browser, value);
        }
    }

    virtual bool OnConsoleMessage (CefRefPtr<CefBrowser> browser, const CefString & message, const CefString & source, int line) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
            return  msclr::call_in_appdomain(appDomainId, &_OnConsoleMessage, wrapped, browser, message, source, line);
        } else {
            return _OnConsoleMessage(wrapped, browser, message, source, line);
        }
    }

private:
    CefRefCount refct;
    CefDisplayHandler* wrapped;
    int appDomainId;

    static void _Release(const CefDisplayHandlerSafeType* _this) {
        delete _this;
    }


    static void _OnAddressChange (CefDisplayHandler* wrapped, CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefString  url) {
         wrapped->OnAddressChange(browser, frame, url);
    }


    static void _OnTitleChange (CefDisplayHandler* wrapped, CefRefPtr<CefBrowser> browser, CefString  title) {
         wrapped->OnTitleChange(browser, title);
    }


    static void _OnFaviconURLChange (CefDisplayHandler* wrapped, CefRefPtr<CefBrowser> browser, int  icon_urls) {
         wrapped->OnFaviconURLChange(browser, icon_urls);
    }


    static void _OnFullscreenModeChange (CefDisplayHandler* wrapped, CefRefPtr<CefBrowser> browser, bool fullscreen) {
         wrapped->OnFullscreenModeChange(browser, fullscreen);
    }


    static bool _OnTooltip (CefDisplayHandler* wrapped, CefRefPtr<CefBrowser> browser, CefString * text) {
        return  wrapped->OnTooltip(browser, *text);
    }


    static void _OnStatusMessage (CefDisplayHandler* wrapped, CefRefPtr<CefBrowser> browser, CefString  value) {
         wrapped->OnStatusMessage(browser, value);
    }


    static bool _OnConsoleMessage (CefDisplayHandler* wrapped, CefRefPtr<CefBrowser> browser, CefString  message, CefString  source, int line) {
        return  wrapped->OnConsoleMessage(browser, message, source, line);
    }

};

typedef CefDisplayHandlerSafeType<> CefDisplayHandlerSafe;


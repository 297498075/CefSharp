

#pragma once

#include "include/cef_life_span_handler.h"

template<typename TImplementation=CefLifeSpanHandler>
class CefLifeSpanHandlerSafeType : public CefLifeSpanHandler {
public:
    CefLifeSpanHandlerSafeType(const CefRefPtr<CefLifeSpanHandler> wrapped) {
        this->wrapped = wrapped.get();
        this->wrapped->AddRef();
        this->appDomainId = System::AppDomain::CurrentDomain->Id;
    }

    ~CefLifeSpanHandlerSafeType() {
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

    virtual bool OnBeforePopup (CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString & target_url, const CefString & target_frame_name, WindowOpenDisposition target_disposition, bool user_gesture, const CefPopupFeatures & popupFeatures, CefWindowInfo & windowInfo, CefRefPtr<CefClient> & client, CefBrowserSettings & settings, bool * no_javascript_access) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
            return  msclr::call_in_appdomain(appDomainId, &_OnBeforePopup, wrapped, browser, frame, target_url, target_frame_name, target_disposition, user_gesture, popupFeatures, &windowInfo, &client, &settings, no_javascript_access);
        } else {
            return _OnBeforePopup(wrapped, browser, frame, target_url, target_frame_name, target_disposition, user_gesture, popupFeatures, &windowInfo, &client, &settings, no_javascript_access);
        }
    }

    virtual void OnAfterCreated (CefRefPtr<CefBrowser> browser) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
             msclr::call_in_appdomain(appDomainId, &_OnAfterCreated, wrapped, browser);
        } else {
            _OnAfterCreated(wrapped, browser);
        }
    }

    virtual bool RunModal (CefRefPtr<CefBrowser> browser) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
            return  msclr::call_in_appdomain(appDomainId, &_RunModal, wrapped, browser);
        } else {
            return _RunModal(wrapped, browser);
        }
    }

    virtual bool DoClose (CefRefPtr<CefBrowser> browser) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
            return  msclr::call_in_appdomain(appDomainId, &_DoClose, wrapped, browser);
        } else {
            return _DoClose(wrapped, browser);
        }
    }

    virtual void OnBeforeClose (CefRefPtr<CefBrowser> browser) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
             msclr::call_in_appdomain(appDomainId, &_OnBeforeClose, wrapped, browser);
        } else {
            _OnBeforeClose(wrapped, browser);
        }
    }

private:
    CefRefCount refct;
    CefLifeSpanHandler* wrapped;
    int appDomainId;

    static void _Release(const CefLifeSpanHandlerSafeType* _this) {
        delete _this;
    }


    static bool _OnBeforePopup (CefLifeSpanHandler* wrapped, CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefString  target_url, CefString  target_frame_name, WindowOpenDisposition target_disposition, bool user_gesture, CefPopupFeatures  popupFeatures, CefWindowInfo * windowInfo, CefRefPtr<CefClient> * client, CefBrowserSettings * settings, bool * no_javascript_access) {
        return  wrapped->OnBeforePopup(browser, frame, target_url, target_frame_name, target_disposition, user_gesture, popupFeatures, *windowInfo, *client, *settings, no_javascript_access);
    }


    static void _OnAfterCreated (CefLifeSpanHandler* wrapped, CefRefPtr<CefBrowser> browser) {
         wrapped->OnAfterCreated(browser);
    }


    static bool _RunModal (CefLifeSpanHandler* wrapped, CefRefPtr<CefBrowser> browser) {
        return  wrapped->RunModal(browser);
    }


    static bool _DoClose (CefLifeSpanHandler* wrapped, CefRefPtr<CefBrowser> browser) {
        return  wrapped->DoClose(browser);
    }


    static void _OnBeforeClose (CefLifeSpanHandler* wrapped, CefRefPtr<CefBrowser> browser) {
         wrapped->OnBeforeClose(browser);
    }

};

typedef CefLifeSpanHandlerSafeType<> CefLifeSpanHandlerSafe;




#pragma once

#include "include/cef_jsdialog_handler.h"

template<typename TImplementation=CefJSDialogHandler>
class CefJSDialogHandlerSafeType : public CefJSDialogHandler {
public:
    CefJSDialogHandlerSafeType(const CefRefPtr<CefJSDialogHandler> wrapped) {
        this->wrapped = wrapped.get();
        this->wrapped->AddRef();
        this->appDomainId = System::AppDomain::CurrentDomain->Id;
    }

    ~CefJSDialogHandlerSafeType() {
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

    virtual bool OnJSDialog (CefRefPtr<CefBrowser> browser, const CefString & origin_url, const CefString & accept_lang, JSDialogType dialog_type, const CefString & message_text, const CefString & default_prompt_text, CefRefPtr<CefJSDialogCallback> callback, bool & suppress_message) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
            return  msclr::call_in_appdomain(appDomainId, &_OnJSDialog, wrapped, browser, origin_url, accept_lang, dialog_type, message_text, default_prompt_text, callback, &suppress_message);
        } else {
            return _OnJSDialog(wrapped, browser, origin_url, accept_lang, dialog_type, message_text, default_prompt_text, callback, &suppress_message);
        }
    }

    virtual bool OnBeforeUnloadDialog (CefRefPtr<CefBrowser> browser, const CefString & message_text, bool is_reload, CefRefPtr<CefJSDialogCallback> callback) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
            return  msclr::call_in_appdomain(appDomainId, &_OnBeforeUnloadDialog, wrapped, browser, message_text, is_reload, callback);
        } else {
            return _OnBeforeUnloadDialog(wrapped, browser, message_text, is_reload, callback);
        }
    }

    virtual void OnResetDialogState (CefRefPtr<CefBrowser> browser) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
             msclr::call_in_appdomain(appDomainId, &_OnResetDialogState, wrapped, browser);
        } else {
            _OnResetDialogState(wrapped, browser);
        }
    }

    virtual void OnDialogClosed (CefRefPtr<CefBrowser> browser) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
             msclr::call_in_appdomain(appDomainId, &_OnDialogClosed, wrapped, browser);
        } else {
            _OnDialogClosed(wrapped, browser);
        }
    }

private:
    CefRefCount refct;
    CefJSDialogHandler* wrapped;
    int appDomainId;

    static void _Release(const CefJSDialogHandlerSafeType* _this) {
        delete _this;
    }


    static bool _OnJSDialog (CefJSDialogHandler* wrapped, CefRefPtr<CefBrowser> browser, CefString  origin_url, CefString  accept_lang, JSDialogType dialog_type, CefString  message_text, CefString  default_prompt_text, CefRefPtr<CefJSDialogCallback> callback, bool * suppress_message) {
        return  wrapped->OnJSDialog(browser, origin_url, accept_lang, dialog_type, message_text, default_prompt_text, callback, *suppress_message);
    }


    static bool _OnBeforeUnloadDialog (CefJSDialogHandler* wrapped, CefRefPtr<CefBrowser> browser, CefString  message_text, bool is_reload, CefRefPtr<CefJSDialogCallback> callback) {
        return  wrapped->OnBeforeUnloadDialog(browser, message_text, is_reload, callback);
    }


    static void _OnResetDialogState (CefJSDialogHandler* wrapped, CefRefPtr<CefBrowser> browser) {
         wrapped->OnResetDialogState(browser);
    }


    static void _OnDialogClosed (CefJSDialogHandler* wrapped, CefRefPtr<CefBrowser> browser) {
         wrapped->OnDialogClosed(browser);
    }

};

typedef CefJSDialogHandlerSafeType<> CefJSDialogHandlerSafe;


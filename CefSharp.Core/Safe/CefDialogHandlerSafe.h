

#pragma once

#include "include/cef_dialog_handler.h"

template<typename TImplementation=CefDialogHandler>
class CefDialogHandlerSafeType : public CefDialogHandler {
public:
    CefDialogHandlerSafeType(const CefRefPtr<CefDialogHandler> wrapped) {
        this->wrapped = wrapped.get();
        this->wrapped->AddRef();
        this->appDomainId = System::AppDomain::CurrentDomain->Id;
    }

    ~CefDialogHandlerSafeType() {
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

private:
    CefRefCount refct;
    CefDialogHandler* wrapped;
    int appDomainId;

    static void _Release(const CefDialogHandlerSafeType* _this) {
        delete _this;
    }


    static bool _OnFileDialog (CefDialogHandler* wrapped, CefRefPtr<CefBrowser> browser, FileDialogMode mode, CefString  title, CefString  default_file_path, int  accept_filters, int selected_accept_filter, CefRefPtr<CefFileDialogCallback> callback) {
        return  wrapped->OnFileDialog(browser, mode, title, default_file_path, accept_filters, selected_accept_filter, callback);
    }

};

typedef CefDialogHandlerSafeType<> CefDialogHandlerSafe;


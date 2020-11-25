

#pragma once

#include "include/cef_print_handler.h"

template<typename TImplementation=CefPrintHandler>
class CefPrintHandlerSafeType : public CefPrintHandler {
public:
    CefPrintHandlerSafeType(const CefRefPtr<CefPrintHandler> wrapped) {
        this->wrapped = wrapped.get();
        this->wrapped->AddRef();
        this->appDomainId = System::AppDomain::CurrentDomain->Id;
    }

    ~CefPrintHandlerSafeType() {
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

    virtual void OnPrintStart (CefRefPtr<CefBrowser> browser) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
             msclr::call_in_appdomain(appDomainId, &_OnPrintStart, wrapped, browser);
        } else {
            _OnPrintStart(wrapped, browser);
        }
    }

    virtual void OnPrintSettings (CefRefPtr<CefPrintSettings> settings, bool get_defaults) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
             msclr::call_in_appdomain(appDomainId, &_OnPrintSettings, wrapped, settings, get_defaults);
        } else {
            _OnPrintSettings(wrapped, settings, get_defaults);
        }
    }

    virtual bool OnPrintDialog (bool has_selection, CefRefPtr<CefPrintDialogCallback> callback) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
            return  msclr::call_in_appdomain(appDomainId, &_OnPrintDialog, wrapped, has_selection, callback);
        } else {
            return _OnPrintDialog(wrapped, has_selection, callback);
        }
    }

    virtual bool OnPrintJob (const CefString & document_name, const CefString & pdf_file_path, CefRefPtr<CefPrintJobCallback> callback) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
            return  msclr::call_in_appdomain(appDomainId, &_OnPrintJob, wrapped, document_name, pdf_file_path, callback);
        } else {
            return _OnPrintJob(wrapped, document_name, pdf_file_path, callback);
        }
    }

    virtual void OnPrintReset () OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
             msclr::call_in_appdomain(appDomainId, &_OnPrintReset, wrapped);
        } else {
            _OnPrintReset(wrapped);
        }
    }

    virtual CefSize GetPdfPaperSize (int device_units_per_inch) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
            return  msclr::call_in_appdomain(appDomainId, &_GetPdfPaperSize, wrapped, device_units_per_inch);
        } else {
            return _GetPdfPaperSize(wrapped, device_units_per_inch);
        }
    }

private:
    CefRefCount refct;
    CefPrintHandler* wrapped;
    int appDomainId;

    static void _Release(const CefPrintHandlerSafeType* _this) {
        delete _this;
    }


    static void _OnPrintStart (CefPrintHandler* wrapped, CefRefPtr<CefBrowser> browser) {
         wrapped->OnPrintStart(browser);
    }


    static void _OnPrintSettings (CefPrintHandler* wrapped, CefRefPtr<CefPrintSettings> settings, bool get_defaults) {
         wrapped->OnPrintSettings(settings, get_defaults);
    }


    static bool _OnPrintDialog (CefPrintHandler* wrapped, bool has_selection, CefRefPtr<CefPrintDialogCallback> callback) {
        return  wrapped->OnPrintDialog(has_selection, callback);
    }


    static bool _OnPrintJob (CefPrintHandler* wrapped, CefString  document_name, CefString  pdf_file_path, CefRefPtr<CefPrintJobCallback> callback) {
        return  wrapped->OnPrintJob(document_name, pdf_file_path, callback);
    }


    static void _OnPrintReset (CefPrintHandler* wrapped) {
         wrapped->OnPrintReset();
    }


    static CefSize _GetPdfPaperSize (CefPrintHandler* wrapped, int device_units_per_inch) {
        return  wrapped->GetPdfPaperSize(device_units_per_inch);
    }

};

typedef CefPrintHandlerSafeType<> CefPrintHandlerSafe;


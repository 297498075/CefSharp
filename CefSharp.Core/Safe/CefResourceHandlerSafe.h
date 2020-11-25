

#pragma once

#include "include/cef_resource_handler.h"

template<typename TImplementation=CefResourceHandler>
class CefResourceHandlerSafeType : public CefResourceHandler {
public:
    CefResourceHandlerSafeType(const CefRefPtr<CefResourceHandler> wrapped) {
        this->wrapped = wrapped.get();
        this->wrapped->AddRef();
        this->appDomainId = System::AppDomain::CurrentDomain->Id;
    }

    ~CefResourceHandlerSafeType() {
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

    virtual bool ProcessRequest (CefRefPtr<CefRequest> request, CefRefPtr<CefCallback> callback) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
            return  msclr::call_in_appdomain(appDomainId, &_ProcessRequest, wrapped, request, callback);
        } else {
            return _ProcessRequest(wrapped, request, callback);
        }
    }

    virtual void GetResponseHeaders (CefRefPtr<CefResponse> response, int64 & response_length, CefString & redirectUrl) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
             msclr::call_in_appdomain(appDomainId, &_GetResponseHeaders, wrapped, response, &response_length, &redirectUrl);
        } else {
            _GetResponseHeaders(wrapped, response, &response_length, &redirectUrl);
        }
    }

    virtual bool ReadResponse (void * data_out, int bytes_to_read, int & bytes_read, CefRefPtr<CefCallback> callback) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
            return  msclr::call_in_appdomain(appDomainId, &_ReadResponse, wrapped, data_out, bytes_to_read, &bytes_read, callback);
        } else {
            return _ReadResponse(wrapped, data_out, bytes_to_read, &bytes_read, callback);
        }
    }

    virtual bool CanGetCookie (const CefCookie & cookie) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
            return  msclr::call_in_appdomain(appDomainId, &_CanGetCookie, wrapped, cookie);
        } else {
            return _CanGetCookie(wrapped, cookie);
        }
    }

    virtual bool CanSetCookie (const CefCookie & cookie) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
            return  msclr::call_in_appdomain(appDomainId, &_CanSetCookie, wrapped, cookie);
        } else {
            return _CanSetCookie(wrapped, cookie);
        }
    }

    virtual void Cancel () OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
             msclr::call_in_appdomain(appDomainId, &_Cancel, wrapped);
        } else {
            _Cancel(wrapped);
        }
    }

private:
    CefRefCount refct;
    CefResourceHandler* wrapped;
    int appDomainId;

    static void _Release(const CefResourceHandlerSafeType* _this) {
        delete _this;
    }


    static bool _ProcessRequest (CefResourceHandler* wrapped, CefRefPtr<CefRequest> request, CefRefPtr<CefCallback> callback) {
        return  wrapped->ProcessRequest(request, callback);
    }


    static void _GetResponseHeaders (CefResourceHandler* wrapped, CefRefPtr<CefResponse> response, int64 * response_length, CefString * redirectUrl) {
         wrapped->GetResponseHeaders(response, *response_length, *redirectUrl);
    }


    static bool _ReadResponse (CefResourceHandler* wrapped, void * data_out, int bytes_to_read, int * bytes_read, CefRefPtr<CefCallback> callback) {
        return  wrapped->ReadResponse(data_out, bytes_to_read, *bytes_read, callback);
    }


    static bool _CanGetCookie (CefResourceHandler* wrapped, CefCookie  cookie) {
        return  wrapped->CanGetCookie(cookie);
    }


    static bool _CanSetCookie (CefResourceHandler* wrapped, CefCookie  cookie) {
        return  wrapped->CanSetCookie(cookie);
    }


    static void _Cancel (CefResourceHandler* wrapped) {
         wrapped->Cancel();
    }

};

typedef CefResourceHandlerSafeType<> CefResourceHandlerSafe;


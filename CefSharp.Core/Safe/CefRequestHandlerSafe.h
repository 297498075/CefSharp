

#pragma once

#include "include/cef_request_handler.h"
#include "CefResourceHandlerSafe.h"

template<typename TImplementation=CefRequestHandler>
class CefRequestHandlerSafeType : public CefRequestHandler {
public:
    CefRequestHandlerSafeType(const CefRefPtr<CefRequestHandler> wrapped) {
        this->wrapped = wrapped.get();
        this->wrapped->AddRef();
        this->appDomainId = System::AppDomain::CurrentDomain->Id;
    }

    ~CefRequestHandlerSafeType() {
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

    virtual bool OnBeforeBrowse (CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request, bool is_redirect) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
            return  msclr::call_in_appdomain(appDomainId, &_OnBeforeBrowse, wrapped, browser, frame, request, is_redirect);
        } else {
            return _OnBeforeBrowse(wrapped, browser, frame, request, is_redirect);
        }
    }

    virtual bool OnOpenURLFromTab (CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, const CefString & target_url, WindowOpenDisposition target_disposition, bool user_gesture) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
            return  msclr::call_in_appdomain(appDomainId, &_OnOpenURLFromTab, wrapped, browser, frame, target_url, target_disposition, user_gesture);
        } else {
            return _OnOpenURLFromTab(wrapped, browser, frame, target_url, target_disposition, user_gesture);
        }
    }

    virtual ReturnValue OnBeforeResourceLoad (CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request, CefRefPtr<CefRequestCallback> callback) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
            return  msclr::call_in_appdomain(appDomainId, &_OnBeforeResourceLoad, wrapped, browser, frame, request, callback);
        } else {
            return _OnBeforeResourceLoad(wrapped, browser, frame, request, callback);
        }
    }

    virtual CefRefPtr<CefResourceHandler> GetResourceHandler (CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
            return  msclr::call_in_appdomain(appDomainId, &_GetResourceHandler, wrapped, browser, frame, request);
        } else {
            return _GetResourceHandler(wrapped, browser, frame, request);
        }
    }

    virtual void OnResourceRedirect (CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request, CefString & new_url) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
             msclr::call_in_appdomain(appDomainId, &_OnResourceRedirect, wrapped, browser, frame, request, &new_url);
        } else {
            _OnResourceRedirect(wrapped, browser, frame, request, &new_url);
        }
    }

    virtual bool OnResourceResponse (CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request, CefRefPtr<CefResponse> response) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
            return  msclr::call_in_appdomain(appDomainId, &_OnResourceResponse, wrapped, browser, frame, request, response);
        } else {
            return _OnResourceResponse(wrapped, browser, frame, request, response);
        }
    }

    virtual CefRefPtr<CefResponseFilter> GetResourceResponseFilter (CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request, CefRefPtr<CefResponse> response) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
            return  msclr::call_in_appdomain(appDomainId, &_GetResourceResponseFilter, wrapped, browser, frame, request, response);
        } else {
            return _GetResourceResponseFilter(wrapped, browser, frame, request, response);
        }
    }

    virtual void OnResourceLoadComplete (CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request, CefRefPtr<CefResponse> response, URLRequestStatus status, int64 received_content_length) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
             msclr::call_in_appdomain(appDomainId, &_OnResourceLoadComplete, wrapped, browser, frame, request, response, status, received_content_length);
        } else {
            _OnResourceLoadComplete(wrapped, browser, frame, request, response, status, received_content_length);
        }
    }

    virtual bool GetAuthCredentials (CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, bool isProxy, const CefString & host, int port, const CefString & realm, const CefString & scheme, CefRefPtr<CefAuthCallback> callback) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
            return  msclr::call_in_appdomain(appDomainId, &_GetAuthCredentials, wrapped, browser, frame, isProxy, host, port, realm, scheme, callback);
        } else {
            return _GetAuthCredentials(wrapped, browser, frame, isProxy, host, port, realm, scheme, callback);
        }
    }

    virtual bool OnQuotaRequest (CefRefPtr<CefBrowser> browser, const CefString & origin_url, int64 new_size, CefRefPtr<CefRequestCallback> callback) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
            return  msclr::call_in_appdomain(appDomainId, &_OnQuotaRequest, wrapped, browser, origin_url, new_size, callback);
        } else {
            return _OnQuotaRequest(wrapped, browser, origin_url, new_size, callback);
        }
    }

    virtual void OnProtocolExecution (CefRefPtr<CefBrowser> browser, const CefString & url, bool & allow_os_execution) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
             msclr::call_in_appdomain(appDomainId, &_OnProtocolExecution, wrapped, browser, url, &allow_os_execution);
        } else {
            _OnProtocolExecution(wrapped, browser, url, &allow_os_execution);
        }
    }

    virtual bool OnCertificateError (CefRefPtr<CefBrowser> browser, cef_errorcode_t cert_error, const CefString & request_url, CefRefPtr<CefSSLInfo> ssl_info, CefRefPtr<CefRequestCallback> callback) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
            return  msclr::call_in_appdomain(appDomainId, &_OnCertificateError, wrapped, browser, cert_error, request_url, ssl_info, callback);
        } else {
            return _OnCertificateError(wrapped, browser, cert_error, request_url, ssl_info, callback);
        }
    }

    virtual void OnPluginCrashed (CefRefPtr<CefBrowser> browser, const CefString & plugin_path) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
             msclr::call_in_appdomain(appDomainId, &_OnPluginCrashed, wrapped, browser, plugin_path);
        } else {
            _OnPluginCrashed(wrapped, browser, plugin_path);
        }
    }

    virtual void OnRenderViewReady (CefRefPtr<CefBrowser> browser) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
             msclr::call_in_appdomain(appDomainId, &_OnRenderViewReady, wrapped, browser);
        } else {
            _OnRenderViewReady(wrapped, browser);
        }
    }

    virtual void OnRenderProcessTerminated (CefRefPtr<CefBrowser> browser, TerminationStatus status) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
             msclr::call_in_appdomain(appDomainId, &_OnRenderProcessTerminated, wrapped, browser, status);
        } else {
            _OnRenderProcessTerminated(wrapped, browser, status);
        }
    }

private:
    CefRefCount refct;
    CefRequestHandler* wrapped;
    int appDomainId;

    static void _Release(const CefRequestHandlerSafeType* _this) {
        delete _this;
    }


    static bool _OnBeforeBrowse (CefRequestHandler* wrapped, CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request, bool is_redirect) {
        return  wrapped->OnBeforeBrowse(browser, frame, request, is_redirect);
    }


    static bool _OnOpenURLFromTab (CefRequestHandler* wrapped, CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefString  target_url, WindowOpenDisposition target_disposition, bool user_gesture) {
        return  wrapped->OnOpenURLFromTab(browser, frame, target_url, target_disposition, user_gesture);
    }


    static ReturnValue _OnBeforeResourceLoad (CefRequestHandler* wrapped, CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request, CefRefPtr<CefRequestCallback> callback) {
        return  wrapped->OnBeforeResourceLoad(browser, frame, request, callback);
    }


    static CefRefPtr<CefResourceHandler> _GetResourceHandler (CefRequestHandler* wrapped, CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request) {
        auto res = wrapped->GetResourceHandler(browser, frame, request);
        if(res) {
            res = new CefResourceHandlerSafe(res);
        }
        return res;
    }


    static void _OnResourceRedirect (CefRequestHandler* wrapped, CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request, CefString * new_url) {
         wrapped->OnResourceRedirect(browser, frame, request, *new_url);
    }


    static bool _OnResourceResponse (CefRequestHandler* wrapped, CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request, CefRefPtr<CefResponse> response) {
        return  wrapped->OnResourceResponse(browser, frame, request, response);
    }


    static CefRefPtr<CefResponseFilter> _GetResourceResponseFilter (CefRequestHandler* wrapped, CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request, CefRefPtr<CefResponse> response) {
        return  wrapped->GetResourceResponseFilter(browser, frame, request, response);
    }


    static void _OnResourceLoadComplete (CefRequestHandler* wrapped, CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request, CefRefPtr<CefResponse> response, URLRequestStatus status, int64 received_content_length) {
         wrapped->OnResourceLoadComplete(browser, frame, request, response, status, received_content_length);
    }


    static bool _GetAuthCredentials (CefRequestHandler* wrapped, CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, bool isProxy, CefString  host, int port, CefString  realm, CefString  scheme, CefRefPtr<CefAuthCallback> callback) {
        return  wrapped->GetAuthCredentials(browser, frame, isProxy, host, port, realm, scheme, callback);
    }


    static bool _OnQuotaRequest (CefRequestHandler* wrapped, CefRefPtr<CefBrowser> browser, CefString  origin_url, int64 new_size, CefRefPtr<CefRequestCallback> callback) {
        return  wrapped->OnQuotaRequest(browser, origin_url, new_size, callback);
    }


    static void _OnProtocolExecution (CefRequestHandler* wrapped, CefRefPtr<CefBrowser> browser, CefString  url, bool * allow_os_execution) {
         wrapped->OnProtocolExecution(browser, url, *allow_os_execution);
    }


    static bool _OnCertificateError (CefRequestHandler* wrapped, CefRefPtr<CefBrowser> browser, cef_errorcode_t cert_error, CefString  request_url, CefRefPtr<CefSSLInfo> ssl_info, CefRefPtr<CefRequestCallback> callback) {
        return  wrapped->OnCertificateError(browser, cert_error, request_url, ssl_info, callback);
    }


    static void _OnPluginCrashed (CefRequestHandler* wrapped, CefRefPtr<CefBrowser> browser, CefString  plugin_path) {
         wrapped->OnPluginCrashed(browser, plugin_path);
    }


    static void _OnRenderViewReady (CefRequestHandler* wrapped, CefRefPtr<CefBrowser> browser) {
         wrapped->OnRenderViewReady(browser);
    }


    static void _OnRenderProcessTerminated (CefRequestHandler* wrapped, CefRefPtr<CefBrowser> browser, TerminationStatus status) {
         wrapped->OnRenderProcessTerminated(browser, status);
    }

};

typedef CefRequestHandlerSafeType<> CefRequestHandlerSafe;


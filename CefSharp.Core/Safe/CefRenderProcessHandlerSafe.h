

#pragma once

#include "include/cef_render_process_handler.h"
#include "CefLoadHandlerSafe.h"

template<typename TImplementation=CefRenderProcessHandler>
class CefRenderProcessHandlerSafeType : public CefRenderProcessHandler {
public:
    CefRenderProcessHandlerSafeType(const CefRefPtr<CefRenderProcessHandler> wrapped) {
        this->wrapped = wrapped.get();
        this->wrapped->AddRef();
        this->appDomainId = System::AppDomain::CurrentDomain->Id;
    }

    ~CefRenderProcessHandlerSafeType() {
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

    virtual void OnRenderThreadCreated (CefRefPtr<CefListValue> extra_info) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
             msclr::call_in_appdomain(appDomainId, &_OnRenderThreadCreated, wrapped, extra_info);
        } else {
            _OnRenderThreadCreated(wrapped, extra_info);
        }
    }

    virtual void OnWebKitInitialized () OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
             msclr::call_in_appdomain(appDomainId, &_OnWebKitInitialized, wrapped);
        } else {
            _OnWebKitInitialized(wrapped);
        }
    }

    virtual void OnBrowserCreated (CefRefPtr<CefBrowser> browser) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
             msclr::call_in_appdomain(appDomainId, &_OnBrowserCreated, wrapped, browser);
        } else {
            _OnBrowserCreated(wrapped, browser);
        }
    }

    virtual void OnBrowserDestroyed (CefRefPtr<CefBrowser> browser) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
             msclr::call_in_appdomain(appDomainId, &_OnBrowserDestroyed, wrapped, browser);
        } else {
            _OnBrowserDestroyed(wrapped, browser);
        }
    }

    virtual CefRefPtr<CefLoadHandler> GetLoadHandler () OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
            return  msclr::call_in_appdomain(appDomainId, &_GetLoadHandler, wrapped);
        } else {
            return _GetLoadHandler(wrapped);
        }
    }

    virtual bool OnBeforeNavigation (CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request, NavigationType navigation_type, bool is_redirect) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
            return  msclr::call_in_appdomain(appDomainId, &_OnBeforeNavigation, wrapped, browser, frame, request, navigation_type, is_redirect);
        } else {
            return _OnBeforeNavigation(wrapped, browser, frame, request, navigation_type, is_redirect);
        }
    }

    virtual void OnContextCreated (CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
             msclr::call_in_appdomain(appDomainId, &_OnContextCreated, wrapped, browser, frame, context);
        } else {
            _OnContextCreated(wrapped, browser, frame, context);
        }
    }

    virtual void OnContextReleased (CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
             msclr::call_in_appdomain(appDomainId, &_OnContextReleased, wrapped, browser, frame, context);
        } else {
            _OnContextReleased(wrapped, browser, frame, context);
        }
    }

    virtual void OnUncaughtException (CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context, CefRefPtr<CefV8Exception> exception, CefRefPtr<CefV8StackTrace> stackTrace) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
             msclr::call_in_appdomain(appDomainId, &_OnUncaughtException, wrapped, browser, frame, context, exception, stackTrace);
        } else {
            _OnUncaughtException(wrapped, browser, frame, context, exception, stackTrace);
        }
    }

    virtual void OnFocusedNodeChanged (CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefDOMNode> node) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
             msclr::call_in_appdomain(appDomainId, &_OnFocusedNodeChanged, wrapped, browser, frame, node);
        } else {
            _OnFocusedNodeChanged(wrapped, browser, frame, node);
        }
    }

    virtual bool OnProcessMessageReceived (CefRefPtr<CefBrowser> browser, CefProcessId source_process, CefRefPtr<CefProcessMessage> message) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
            return  msclr::call_in_appdomain(appDomainId, &_OnProcessMessageReceived, wrapped, browser, source_process, message);
        } else {
            return _OnProcessMessageReceived(wrapped, browser, source_process, message);
        }
    }

private:
    CefRefCount refct;
    CefRenderProcessHandler* wrapped;
    int appDomainId;

    static void _Release(const CefRenderProcessHandlerSafeType* _this) {
        delete _this;
    }


    static void _OnRenderThreadCreated (CefRenderProcessHandler* wrapped, CefRefPtr<CefListValue> extra_info) {
         wrapped->OnRenderThreadCreated(extra_info);
    }


    static void _OnWebKitInitialized (CefRenderProcessHandler* wrapped) {
         wrapped->OnWebKitInitialized();
    }


    static void _OnBrowserCreated (CefRenderProcessHandler* wrapped, CefRefPtr<CefBrowser> browser) {
         wrapped->OnBrowserCreated(browser);
    }


    static void _OnBrowserDestroyed (CefRenderProcessHandler* wrapped, CefRefPtr<CefBrowser> browser) {
         wrapped->OnBrowserDestroyed(browser);
    }


    static CefRefPtr<CefLoadHandler> _GetLoadHandler (CefRenderProcessHandler* wrapped) {
        auto res = wrapped->GetLoadHandler();
        if(res) {
            res = new CefLoadHandlerSafe(res);
        }
        return res;
    }


    static bool _OnBeforeNavigation (CefRenderProcessHandler* wrapped, CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefRequest> request, NavigationType navigation_type, bool is_redirect) {
        return  wrapped->OnBeforeNavigation(browser, frame, request, navigation_type, is_redirect);
    }


    static void _OnContextCreated (CefRenderProcessHandler* wrapped, CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context) {
         wrapped->OnContextCreated(browser, frame, context);
    }


    static void _OnContextReleased (CefRenderProcessHandler* wrapped, CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context) {
         wrapped->OnContextReleased(browser, frame, context);
    }


    static void _OnUncaughtException (CefRenderProcessHandler* wrapped, CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context, CefRefPtr<CefV8Exception> exception, CefRefPtr<CefV8StackTrace> stackTrace) {
         wrapped->OnUncaughtException(browser, frame, context, exception, stackTrace);
    }


    static void _OnFocusedNodeChanged (CefRenderProcessHandler* wrapped, CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefDOMNode> node) {
         wrapped->OnFocusedNodeChanged(browser, frame, node);
    }


    static bool _OnProcessMessageReceived (CefRenderProcessHandler* wrapped, CefRefPtr<CefBrowser> browser, CefProcessId source_process, CefRefPtr<CefProcessMessage> message) {
        return  wrapped->OnProcessMessageReceived(browser, source_process, message);
    }

};

typedef CefRenderProcessHandlerSafeType<> CefRenderProcessHandlerSafe;


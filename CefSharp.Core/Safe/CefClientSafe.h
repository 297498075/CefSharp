

#pragma once

#include "include/cef_browser.h"
#include "CefContextMenuHandlerSafe.h"
#include "CefDialogHandlerSafe.h"
#include "CefDisplayHandlerSafe.h"
#include "CefDownloadHandlerSafe.h"
#include "CefDragHandlerSafe.h"
#include "CefFindHandlerSafe.h"
#include "CefFocusHandlerSafe.h"
#include "CefGeolocationHandlerSafe.h"
#include "CefJSDialogHandlerSafe.h"
#include "CefKeyboardHandlerSafe.h"
#include "CefLifeSpanHandlerSafe.h"
#include "CefLoadHandlerSafe.h"
#include "CefRenderHandlerSafe.h"
#include "CefRequestHandlerSafe.h"

template<typename TImplementation=CefClient>
class CefClientSafeType : public CefClient {
public:
    CefClientSafeType(const CefRefPtr<CefClient> wrapped) {
        this->wrapped = wrapped.get();
        this->wrapped->AddRef();
        this->appDomainId = System::AppDomain::CurrentDomain->Id;
    }

    ~CefClientSafeType() {
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

    virtual CefRefPtr<CefContextMenuHandler> GetContextMenuHandler () OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
            return  msclr::call_in_appdomain(appDomainId, &_GetContextMenuHandler, wrapped);
        } else {
            return _GetContextMenuHandler(wrapped);
        }
    }

    virtual CefRefPtr<CefDialogHandler> GetDialogHandler () OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
            return  msclr::call_in_appdomain(appDomainId, &_GetDialogHandler, wrapped);
        } else {
            return _GetDialogHandler(wrapped);
        }
    }

    virtual CefRefPtr<CefDisplayHandler> GetDisplayHandler () OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
            return  msclr::call_in_appdomain(appDomainId, &_GetDisplayHandler, wrapped);
        } else {
            return _GetDisplayHandler(wrapped);
        }
    }

    virtual CefRefPtr<CefDownloadHandler> GetDownloadHandler () OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
            return  msclr::call_in_appdomain(appDomainId, &_GetDownloadHandler, wrapped);
        } else {
            return _GetDownloadHandler(wrapped);
        }
    }

    virtual CefRefPtr<CefDragHandler> GetDragHandler () OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
            return  msclr::call_in_appdomain(appDomainId, &_GetDragHandler, wrapped);
        } else {
            return _GetDragHandler(wrapped);
        }
    }

    virtual CefRefPtr<CefFindHandler> GetFindHandler () OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
            return  msclr::call_in_appdomain(appDomainId, &_GetFindHandler, wrapped);
        } else {
            return _GetFindHandler(wrapped);
        }
    }

    virtual CefRefPtr<CefFocusHandler> GetFocusHandler () OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
            return  msclr::call_in_appdomain(appDomainId, &_GetFocusHandler, wrapped);
        } else {
            return _GetFocusHandler(wrapped);
        }
    }

    virtual CefRefPtr<CefGeolocationHandler> GetGeolocationHandler () OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
            return  msclr::call_in_appdomain(appDomainId, &_GetGeolocationHandler, wrapped);
        } else {
            return _GetGeolocationHandler(wrapped);
        }
    }

    virtual CefRefPtr<CefJSDialogHandler> GetJSDialogHandler () OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
            return  msclr::call_in_appdomain(appDomainId, &_GetJSDialogHandler, wrapped);
        } else {
            return _GetJSDialogHandler(wrapped);
        }
    }

    virtual CefRefPtr<CefKeyboardHandler> GetKeyboardHandler () OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
            return  msclr::call_in_appdomain(appDomainId, &_GetKeyboardHandler, wrapped);
        } else {
            return _GetKeyboardHandler(wrapped);
        }
    }

    virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler () OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
            return  msclr::call_in_appdomain(appDomainId, &_GetLifeSpanHandler, wrapped);
        } else {
            return _GetLifeSpanHandler(wrapped);
        }
    }

    virtual CefRefPtr<CefLoadHandler> GetLoadHandler () OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
            return  msclr::call_in_appdomain(appDomainId, &_GetLoadHandler, wrapped);
        } else {
            return _GetLoadHandler(wrapped);
        }
    }

    virtual CefRefPtr<CefRenderHandler> GetRenderHandler () OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
            return  msclr::call_in_appdomain(appDomainId, &_GetRenderHandler, wrapped);
        } else {
            return _GetRenderHandler(wrapped);
        }
    }

    virtual CefRefPtr<CefRequestHandler> GetRequestHandler () OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
            return  msclr::call_in_appdomain(appDomainId, &_GetRequestHandler, wrapped);
        } else {
            return _GetRequestHandler(wrapped);
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
    CefClient* wrapped;
    int appDomainId;

    static void _Release(const CefClientSafeType* _this) {
        delete _this;
    }


    static CefRefPtr<CefContextMenuHandler> _GetContextMenuHandler (CefClient* wrapped) {
        auto res = wrapped->GetContextMenuHandler();
        if(res) {
            res = new CefContextMenuHandlerSafe(res);
        }
        return res;
    }


    static CefRefPtr<CefDialogHandler> _GetDialogHandler (CefClient* wrapped) {
        auto res = wrapped->GetDialogHandler();
        if(res) {
            res = new CefDialogHandlerSafe(res);
        }
        return res;
    }


    static CefRefPtr<CefDisplayHandler> _GetDisplayHandler (CefClient* wrapped) {
        auto res = wrapped->GetDisplayHandler();
        if(res) {
            res = new CefDisplayHandlerSafe(res);
        }
        return res;
    }


    static CefRefPtr<CefDownloadHandler> _GetDownloadHandler (CefClient* wrapped) {
        auto res = wrapped->GetDownloadHandler();
        if(res) {
            res = new CefDownloadHandlerSafe(res);
        }
        return res;
    }


    static CefRefPtr<CefDragHandler> _GetDragHandler (CefClient* wrapped) {
        auto res = wrapped->GetDragHandler();
        if(res) {
            res = new CefDragHandlerSafe(res);
        }
        return res;
    }


    static CefRefPtr<CefFindHandler> _GetFindHandler (CefClient* wrapped) {
        auto res = wrapped->GetFindHandler();
        if(res) {
            res = new CefFindHandlerSafe(res);
        }
        return res;
    }


    static CefRefPtr<CefFocusHandler> _GetFocusHandler (CefClient* wrapped) {
        auto res = wrapped->GetFocusHandler();
        if(res) {
            res = new CefFocusHandlerSafe(res);
        }
        return res;
    }


    static CefRefPtr<CefGeolocationHandler> _GetGeolocationHandler (CefClient* wrapped) {
        auto res = wrapped->GetGeolocationHandler();
        if(res) {
            res = new CefGeolocationHandlerSafe(res);
        }
        return res;
    }


    static CefRefPtr<CefJSDialogHandler> _GetJSDialogHandler (CefClient* wrapped) {
        auto res = wrapped->GetJSDialogHandler();
        if(res) {
            res = new CefJSDialogHandlerSafe(res);
        }
        return res;
    }


    static CefRefPtr<CefKeyboardHandler> _GetKeyboardHandler (CefClient* wrapped) {
        auto res = wrapped->GetKeyboardHandler();
        if(res) {
            res = new CefKeyboardHandlerSafe(res);
        }
        return res;
    }


    static CefRefPtr<CefLifeSpanHandler> _GetLifeSpanHandler (CefClient* wrapped) {
        auto res = wrapped->GetLifeSpanHandler();
        if(res) {
            res = new CefLifeSpanHandlerSafe(res);
        }
        return res;
    }


    static CefRefPtr<CefLoadHandler> _GetLoadHandler (CefClient* wrapped) {
        auto res = wrapped->GetLoadHandler();
        if(res) {
            res = new CefLoadHandlerSafe(res);
        }
        return res;
    }


    static CefRefPtr<CefRenderHandler> _GetRenderHandler (CefClient* wrapped) {
        auto res = wrapped->GetRenderHandler();
        if(res) {
            res = new CefRenderHandlerSafe(res);
        }
        return res;
    }


    static CefRefPtr<CefRequestHandler> _GetRequestHandler (CefClient* wrapped) {
        auto res = wrapped->GetRequestHandler();
        if(res) {
            res = new CefRequestHandlerSafe(res);
        }
        return res;
    }


    static bool _OnProcessMessageReceived (CefClient* wrapped, CefRefPtr<CefBrowser> browser, CefProcessId source_process, CefRefPtr<CefProcessMessage> message) {
        return  wrapped->OnProcessMessageReceived(browser, source_process, message);
    }

};

typedef CefClientSafeType<> CefClientSafe;


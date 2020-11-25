

#pragma once

#include "include/cef_find_handler.h"

template<typename TImplementation=CefFindHandler>
class CefFindHandlerSafeType : public CefFindHandler {
public:
    CefFindHandlerSafeType(const CefRefPtr<CefFindHandler> wrapped) {
        this->wrapped = wrapped.get();
        this->wrapped->AddRef();
        this->appDomainId = System::AppDomain::CurrentDomain->Id;
    }

    ~CefFindHandlerSafeType() {
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

    virtual void OnFindResult (CefRefPtr<CefBrowser> browser, int identifier, int count, const CefRect & selectionRect, int activeMatchOrdinal, bool finalUpdate) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
             msclr::call_in_appdomain(appDomainId, &_OnFindResult, wrapped, browser, identifier, count, selectionRect, activeMatchOrdinal, finalUpdate);
        } else {
            _OnFindResult(wrapped, browser, identifier, count, selectionRect, activeMatchOrdinal, finalUpdate);
        }
    }

private:
    CefRefCount refct;
    CefFindHandler* wrapped;
    int appDomainId;

    static void _Release(const CefFindHandlerSafeType* _this) {
        delete _this;
    }


    static void _OnFindResult (CefFindHandler* wrapped, CefRefPtr<CefBrowser> browser, int identifier, int count, CefRect  selectionRect, int activeMatchOrdinal, bool finalUpdate) {
         wrapped->OnFindResult(browser, identifier, count, selectionRect, activeMatchOrdinal, finalUpdate);
    }

};

typedef CefFindHandlerSafeType<> CefFindHandlerSafe;


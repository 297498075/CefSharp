

#pragma once

#include "include/cef_web_plugin.h"

template<typename TImplementation=CefWebPluginInfoVisitor>
class CefWebPluginInfoVisitorSafeType : public CefWebPluginInfoVisitor {
public:
    CefWebPluginInfoVisitorSafeType(const CefRefPtr<CefWebPluginInfoVisitor> wrapped) {
        this->wrapped = wrapped.get();
        this->wrapped->AddRef();
        this->appDomainId = System::AppDomain::CurrentDomain->Id;
    }

    ~CefWebPluginInfoVisitorSafeType() {
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

    virtual bool Visit (CefRefPtr<CefWebPluginInfo> info, int count, int total) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
            return  msclr::call_in_appdomain(appDomainId, &_Visit, wrapped, info, count, total);
        } else {
            return _Visit(wrapped, info, count, total);
        }
    }

private:
    CefRefCount refct;
    CefWebPluginInfoVisitor* wrapped;
    int appDomainId;

    static void _Release(const CefWebPluginInfoVisitorSafeType* _this) {
        delete _this;
    }


    static bool _Visit (CefWebPluginInfoVisitor* wrapped, CefRefPtr<CefWebPluginInfo> info, int count, int total) {
        return  wrapped->Visit(info, count, total);
    }

};

typedef CefWebPluginInfoVisitorSafeType<> CefWebPluginInfoVisitorSafe;


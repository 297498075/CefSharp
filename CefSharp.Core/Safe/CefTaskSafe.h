

#pragma once

#include "include/cef_task.h"

template<typename TImplementation=CefTask>
class CefTaskSafeType : public CefTask {
public:
    CefTaskSafeType(const CefRefPtr<CefTask> wrapped) {
        this->wrapped = wrapped.get();
        this->wrapped->AddRef();
        this->appDomainId = System::AppDomain::CurrentDomain->Id;
    }

    ~CefTaskSafeType() {
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

    virtual void Execute () OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
             msclr::call_in_appdomain(appDomainId, &_Execute, wrapped);
        } else {
            _Execute(wrapped);
        }
    }

private:
    CefRefCount refct;
    CefTask* wrapped;
    int appDomainId;

    static void _Release(const CefTaskSafeType* _this) {
        delete _this;
    }


    static void _Execute (CefTask* wrapped) {
         wrapped->Execute();
    }

};

typedef CefTaskSafeType<> CefTaskSafe;


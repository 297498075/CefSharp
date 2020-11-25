

#pragma once

#include "include/cef_resource_bundle_handler.h"

template<typename TImplementation=CefResourceBundleHandler>
class CefResourceBundleHandlerSafeType : public CefResourceBundleHandler {
public:
    CefResourceBundleHandlerSafeType(const CefRefPtr<CefResourceBundleHandler> wrapped) {
        this->wrapped = wrapped.get();
        this->wrapped->AddRef();
        this->appDomainId = System::AppDomain::CurrentDomain->Id;
    }

    ~CefResourceBundleHandlerSafeType() {
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

    virtual bool GetLocalizedString (int string_id, CefString & string) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
            return  msclr::call_in_appdomain(appDomainId, &_GetLocalizedString, wrapped, string_id, &string);
        } else {
            return _GetLocalizedString(wrapped, string_id, &string);
        }
    }

    virtual bool GetDataResource (int resource_id, void *& data, size_t & data_size) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
            return  msclr::call_in_appdomain(appDomainId, &_GetDataResource, wrapped, resource_id, &data, &data_size);
        } else {
            return _GetDataResource(wrapped, resource_id, &data, &data_size);
        }
    }

    virtual bool GetDataResourceForScale (int resource_id, ScaleFactor scale_factor, void *& data, size_t & data_size) OVERRIDE {
        if (this->appDomainId != System::AppDomain::CurrentDomain->Id) {
            return  msclr::call_in_appdomain(appDomainId, &_GetDataResourceForScale, wrapped, resource_id, scale_factor, &data, &data_size);
        } else {
            return _GetDataResourceForScale(wrapped, resource_id, scale_factor, &data, &data_size);
        }
    }

private:
    CefRefCount refct;
    CefResourceBundleHandler* wrapped;
    int appDomainId;

    static void _Release(const CefResourceBundleHandlerSafeType* _this) {
        delete _this;
    }


    static bool _GetLocalizedString (CefResourceBundleHandler* wrapped, int string_id, CefString * string) {
        return  wrapped->GetLocalizedString(string_id, *string);
    }


    static bool _GetDataResource (CefResourceBundleHandler* wrapped, int resource_id, void ** data, size_t * data_size) {
        return  wrapped->GetDataResource(resource_id, *data, *data_size);
    }


    static bool _GetDataResourceForScale (CefResourceBundleHandler* wrapped, int resource_id, ScaleFactor scale_factor, void ** data, size_t * data_size) {
        return  wrapped->GetDataResourceForScale(resource_id, scale_factor, *data, *data_size);
    }

};

typedef CefResourceBundleHandlerSafeType<> CefResourceBundleHandlerSafe;


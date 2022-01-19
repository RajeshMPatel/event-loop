#include "stdafx.h"
#include "LibMsgNetworkChange.h"
//#include "SystemConfiguration.h"
#include <SystemConfiguration/SystemConfiguration.h>
#include <dispatch/dispatch.h>
#include <boost/make_shared.hpp>

typedef struct {
  SCDynamicStoreRef storeRef;
  dispatch_queue_t  storeQueue;
} NetworkChangeContextT;

static void s_networkChanged(SCDynamicStoreRef store, CFArrayRef changedKeys, void *info) {
  LibMsgNetworkChange *_this = (LibMsgNetworkChange *)info;
  //SCDynamicStoreContext context = {0, _this, nullptr, nullptr, nullptr};
  const boost::system::error_code ec; // ignored for mac
  _this->HandleEvent(ec);
}

LibMsgNetworkChange::LibMsgNetworkChange(LibMsgThread* pMsgThread, const int nMsgId)
  :
  LibMsgEvent(pMsgThread, nMsgId),
  m_bEnabled(false) {
}

LibMsgNetworkChange::~LibMsgNetworkChange() {
}

// We do not need these on Mac
bool LibMsgNetworkChange::EnableNotification() {
  return true;
}

void LibMsgNetworkChange::ResetEvents() {
}

void LibMsgNetworkChange::UnRegisterNotification() {
  NetworkChangeContextT *pContext = (NetworkChangeContextT *)m_Socket;
  SCDynamicStoreSetDispatchQueue(pContext->storeRef, nullptr);

  dispatch_release(pContext->storeQueue);
  CFRelease(pContext->storeRef);
  delete pContext;
  m_Socket = 0;
  pContext = nullptr;
  m_bEnabled = false;
}

bool LibMsgNetworkChange::RegisterNotification() {
  if (m_bEnabled)
    return true;
  
  NetworkChangeContextT *pContext = new NetworkChangeContextT;
  m_Socket = (LM_SOCKET)pContext;
  
  CFTypeRef info = CFRetain((CFTypeRef)this);
  SCDynamicStoreContext context = {0, (void*)info, NULL, NULL, NULL};
  pContext->storeRef = SCDynamicStoreCreate(NULL, CFSTR("LibMsgNetworkChange"), s_networkChanged, &context);
  
  CFStringRef ipKey = SCDynamicStoreKeyCreateNetworkGlobalEntity( kCFAllocatorDefault, kSCDynamicStoreDomainState, kSCEntNetIPv4 );
  CFArrayRef keyArray = CFArrayCreate(NULL, (const void **)(&ipKey), 1, &kCFTypeArrayCallBacks);
  SCDynamicStoreSetNotificationKeys( pContext->storeRef, keyArray, NULL );
  CFRelease(keyArray);
  CFRelease(ipKey);
  
  pContext->storeQueue = dispatch_queue_create( NULL, NULL);
  bool bb = SCDynamicStoreSetDispatchQueue( pContext->storeRef, pContext->storeQueue);

  m_bEnabled = true;
  return true;
}

void LibMsgNetworkChange::HandleEvent(const boost::system::error_code& error) {
  if (m_pMsgThread && (m_nMsgId != 0))
    m_pMsgThread->PostMessage(m_nMsgId, 0, 0);
}

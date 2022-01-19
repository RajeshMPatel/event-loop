#include "stdafx.h"
#include "LibMsgEvent.h"

LibMsgEvent::LibMsgEvent(LibMsgThread* pMsgThread, const int nMsgId)
  :
#ifdef _WIN32
  m_pObj(NULL),
#endif
  m_pMsgThread(pMsgThread),
  m_nMsgId(nMsgId)
{
}

// abstract class with virtual destructor
LibMsgEvent::~LibMsgEvent() {
}


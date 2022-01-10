//
//  LibMsgHandler.cpp
//  LibMsg
//
//  Created by Rajesh Patel on 8/9/18.
//

#include "stdafx.h"
#include "LibMsgHandler.h"
#include <assert.h>

const LibMsgMap LibMsgHandler::m_LibMsgMap = {
  nullptr,
  &LibMsgHandler::m_LibMsgItemList[0]
};

const LibMsgItem LibMsgHandler::m_LibMsgItemList[] = {
  { 0, (HLIBMSG) 0 }
};

LibMsgHandler::LibMsgHandler() {
}

LibMsgHandler::~LibMsgHandler() {
}

const LibMsgMap* LibMsgHandler::GetLibMsgMap() const {
  return &LibMsgHandler::m_LibMsgMap;
}

const LibMsgMap* LibMsgHandler::GetLibMsgMapLocal() {
  return &LibMsgHandler::m_LibMsgMap;
}

// Just a dump lookup. Since we have defined the macro as an array
// We have to do a linear lookup. We could do binary search here
const LibMsgItem* LibMsgHandler::FindLibMsgItem(const unsigned int nMessage, const LibMsgItem* lpItem) {
  while (lpItem->pFn)
  {
    if (nMessage == lpItem->nMessage)
      return lpItem;
    lpItem++;
  }
  return nullptr;
}

const LibMsgItem* LibMsgHandler::FindLibMsgItemInMap(const unsigned int nMessage) {
  const LibMsgMap* pLibMsgMap = GetLibMsgMap();
  const LibMsgItem* pLibMsgItem = nullptr;

  for (; pLibMsgMap != nullptr; pLibMsgMap = pLibMsgMap->pBaseMap)
  {
    assert( pLibMsgMap != pLibMsgMap->pBaseMap );
    pLibMsgItem = FindLibMsgItem(nMessage, pLibMsgMap->lpItems);
    if (pLibMsgItem)
      break;
  }
  return pLibMsgItem;
}

bool LibMsgHandler::OnMessage(const unsigned int nMessage, const LM_WPARAM wParam, const LM_LPARAM lParam) {
  const LibMsgItem* pLibMsgItem = FindLibMsgItemInMap(nMessage);
  if (!pLibMsgItem || !pLibMsgItem->pFn)
    return false;

  union LibMsgMapFunctions mmf;
  mmf.pFn = pLibMsgItem->pFn;

  (this->*mmf.pFn_LIBMSG)(wParam, lParam);

  return true;
}

bool LibMsgHandler::IsHandled( const unsigned int  nMessage) {
  const LibMsgMap* pLibMsgMap = GetLibMsgMap();
  const LibMsgItem* pLibMsgItem = nullptr;

  for (; pLibMsgMap != nullptr; pLibMsgMap = pLibMsgMap->pBaseMap)
  {
    assert( pLibMsgMap != pLibMsgMap->pBaseMap );
    pLibMsgItem = FindLibMsgItem(nMessage, pLibMsgMap->lpItems);
    if (pLibMsgItem)
      break;
  }

  return (nullptr != pLibMsgItem);
}

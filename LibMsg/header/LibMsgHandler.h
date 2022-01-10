//
//  LibMsgHandler.h
//  LibMsg
//
//  Created by Rajesh Patel on 8/9/18.
//

#ifndef Libmsghandler_h
#define Libmsghandler_h

#include "LibMsgDllExport.h"

#ifdef DEBUG
  #define LIBMSG_MESSAGE_DEBUG(X) X
#else
  #define LIBMSG_MESSAGE_DEBUG(X)
#endif 

typedef ptrdiff_t LM_WPARAM;
typedef ptrdiff_t LM_LPARAM;

class LibMsgHandler;
typedef void (LibMsgHandler::*HLIBMSG)(LM_WPARAM, LM_LPARAM);

template<class R, class P0, class P1>
size_t hashTypes()
{
  static const size_t hash = typeid(R(P0, P1)).hash_code();
  return hash;
}

template<class R, class C, class P0, class P1>
size_t hashMessageTypes(R (C::*)(P0, P1))
{
  return hashTypes<R, P0, P1>();
}

template<typename To, typename From>
To LibMsgCast(From fn)
{
  union {
    To to;
    From from;
  } mmf = {0};
  mmf.from = fn;
      
  return mmf.to;
}

struct LibMsgItem
{
  unsigned int  nMessage;
  HLIBMSG       pFn;
  LIBMSG_MESSAGE_DEBUG(size_t hash);
};

struct LibMsgMap
{
  const LibMsgMap*    pBaseMap;
  const LibMsgItem*   lpItems;
};

#define DECLARE_LIBMSG_MAP() \
private: \
  static const LibMsgItem m_LibMsgItemList[]; \
protected: \
  static const LibMsgMap m_LibMsgMap; \
  virtual const LibMsgMap* GetLibMsgMap() const; \
public: \
  static const LibMsgMap* GetLibMsgMapLocal(); \


#define DECLARE_LIBMSG_MAP_EXPORT() \
private: \
  static const LibMsgItem m_LibMsgItemList[]; \
protected: \
  static const LibMsgMap m_LibMsgMap; \
  virtual const LibMsgMap* GetLibMsgMap() const; \
public: \
  LibMsgDllExport static const LibMsgMap* GetLibMsgMapLocal(); \


#define BEGIN_LIBMSG_MAP(theClass, baseClass) \
  const LibMsgMap* theClass::GetLibMsgMap() const \
    { return &theClass::m_LibMsgMap; } \
  const LibMsgMap* theClass::GetLibMsgMapLocal() \
    { return &theClass::m_LibMsgMap; } \
  const LibMsgMap theClass::m_LibMsgMap = \
  { baseClass::GetLibMsgMapLocal(), &theClass::m_LibMsgItemList[0] }; \
  const LibMsgItem theClass::m_LibMsgItemList[] = \
  { \


#define END_LIBMSG_MAP() \
  {0, (HLIBMSG) 0 } \
}; \

#define ON_MESSAGE(nMessage, memberFn) \
  { nMessage, LibMsgCast<HLIBMSG>(&memberFn), LIBMSG_MESSAGE_DEBUG(hashMessageTypes(&memberFn)) },

class LibMsgHandler
{
public:

  LibMsgHandler();
  virtual ~LibMsgHandler();

  bool IsHandled(const unsigned int nMessage);
  bool OnMessage(const unsigned int nMessage, const LM_WPARAM wParam, const LM_LPARAM lParam);

  template<typename T0, typename T1>
  bool OnMessageT(const unsigned int nMessage, T0 t0, T1 t1);

  DECLARE_LIBMSG_MAP();

private:

  const LibMsgItem*  FindLibMsgItem(const unsigned int nMessage, const LibMsgItem* lpItem);

  const LibMsgItem* FindLibMsgItemInMap(const unsigned int nMessage);
};

union LibMsgMapFunctions
{
  HLIBMSG pFn;
  void (LibMsgHandler::*pFn_LIBMSG)(LM_WPARAM, LM_LPARAM);
};

template<typename T0, typename T1>
bool LibMsgHandler::OnMessageT(const unsigned int nMessage, T0 t0, T1 t1)
{
  const LibMsgItem* pLibMsgItem = FindLibMsgItemInMap(nMessage);
  if (!pLibMsgItem || !pLibMsgItem->pFn)
    return false;

  typedef void (LibMsgHandler::* tPMFN)(T0, T1);
  tPMFN pFn = LibMsgCast<tPMFN>(pLibMsgItem->pFn);

  // compute the hash of the function to verify that we are actually calling
  // the correct function
#ifdef DEBUG
  assert((hashTypes<void, T0, T1>() == pLibMsgItem->hash));
#endif
  
  // Call the right function. We already looked it up
  (this->*pFn)(t0, t1);
  return true;
}

#endif /* Libmsghandler_h */

//----------------------------------*-C++-*----------------------------------//
/*!
 * \file   c4/C4_Req.hh
 * \author Thomas M. Evans, Geoffrey Furnish
 * \date   Thu Jun  2 09:54:02 2005
 * \brief  C4_Req class definition.
 * \note   Copyright (C) 2016-2018 Los Alamos National Security, LLC.
 *         All rights reserved. */
//---------------------------------------------------------------------------//

#ifndef c4_C4_Req_hh
#define c4_C4_Req_hh

// C4 package configure
#include "C4_Status.hh"
#include "C4_Traits.hh"
#include "c4/config.h"
#include "ds++/Assert.hh"

#ifdef C4_MPI
#include "c4_mpi.h"
#endif

namespace rtt_c4 {
//===========================================================================//
/*!
 * \class C4_ReqRefRep
 * \brief Handle for non-blocking message requests.
 *
 * This class provides an encapsulator for the message requests (MPI) which are
 * produced by non blocking calls.  This class automatically waits for the
 * message to complete when the containing object goes out of scope, thus
 * plugging one of the easiest types of programming errors with non blocking
 * messaging.  Reference counting is used so that these may be passed by value
 * without accidentally triggering a program stall.
 *
 * This is a "work" class. The user interface for requests is provided by
 * rtt_c4::C4_Req.
 */
//===========================================================================//

class C4_ReqRefRep {
  friend class C4_Req;

  // number of ref counts
  int n;

  // if true, we hold a request
  bool assigned;

#ifdef C4_MPI
  MPI_Request r;
#endif

private:
  // Disallowed methods
  C4_ReqRefRep(const C4_ReqRefRep &rep) = delete;
  C4_ReqRefRep &operator=(const C4_ReqRefRep &rep) = delete;

  // Private default ctor and dtor for access from C4_Req only.
  C4_ReqRefRep();
  ~C4_ReqRefRep();

public:
  DLL_PUBLIC_c4 void wait(C4_Status *status = nullptr);
  DLL_PUBLIC_c4 bool complete(C4_Status *status = nullptr);
  DLL_PUBLIC_c4 void free();

  bool inuse() const {
#ifdef C4_MPI
    if (assigned) {
      Check(r != MPI_REQUEST_NULL);
    }
#endif
    return assigned;
  }

private:
  void set() { assigned = true; }
  void clear() { assigned = false; }
};

//===========================================================================//
/*!
 * \class C4_Req
 * \brief Non-blocking communication request class.
 *
 * This class provides an encapsulator for the message requests (MPI) which
 * are produced by non blocking calls.  This class automatically waits for the
 * message to complete when the containing object goes out of scope, thus
 * plugging one of the easiest types of programming errors with non blocking
 * messaging.  Reference counting is used so that these may be passed by value
 * without accidentally triggering a program stall.
 *
 * This class provides an interface for non-blocking request handles that
 * should be used by users.
 */
//===========================================================================//

class C4_Req {
  //! Request handle.
  C4_ReqRefRep *p;

public:
  DLL_PUBLIC_c4 C4_Req();
  DLL_PUBLIC_c4 C4_Req(const C4_Req &req);
  DLL_PUBLIC_c4 ~C4_Req();
  DLL_PUBLIC_c4 C4_Req &operator=(const C4_Req &req);

  //! \brief Equivalence operator
  bool operator==(const C4_Req &right) { return (p == right.p); }
  bool operator!=(const C4_Req &right) { return (p != right.p); }

  void wait(C4_Status *status = nullptr) { p->wait(status); }
  bool complete(C4_Status *status = nullptr) { return p->complete(status); }
  void free() { p->free(); }
  bool inuse() const { return p->inuse(); }

private:
  void set() { p->set(); }

  // Private access to the C4_ReqRefRep internals.

#ifdef C4_MPI
  MPI_Request &r() { return p->r; }
#endif

  void free_();

  /* FRIENDSHIP
   *
   * Specific friend C4 functions that may need to manipulate the C4_ReqRefRep
   * internals. A friend function of a class is defined outside that class'
   * scope but it has the right to access all private and protected members of
   * the class. Even though the prototypes for friend functions appear in the
   * class definition, friends are not member functions.
   * \ref https://www.tutorialspoint.com/cplusplus/cpp_friend_functions.htm
   */
  template <typename T>
  friend DLL_PUBLIC_c4 C4_Req send_async(const T *buf, int nels, int dest,
                                         int tag);
  template <typename T>
  friend DLL_PUBLIC_c4 void send_async(C4_Req &r, const T *buf, int nels,
                                       int dest, int tag);
  template <typename T>
  friend DLL_PUBLIC_c4 C4_Req receive_async(T *buf, int nels, int source,
                                            int tag);
  template <typename T>
  friend DLL_PUBLIC_c4 void receive_async(C4_Req &r, T *buf, int nels,
                                          int source, int tag);
#ifdef C4_MPI
  template <typename T>
  friend void send_is_custom(C4_Req &request, T const *buffer, int size,
                             int destination, int tag);
  template <typename T>
  friend void receive_async_custom(C4_Req &request, T *buffer, int size,
                                   int source, int tag);
  template <typename T>
  friend DLL_PUBLIC_c4 void send_is(C4_Req &r, const T *buf, int nels, int dest,
                                    int tag);
  friend DLL_PUBLIC_c4 void wait_all(unsigned count, C4_Req *requests);
  friend DLL_PUBLIC_c4 unsigned wait_any(unsigned count, C4_Req *requests);
  template <typename T>
  friend DLL_PUBLIC_c4 void global_isum(T &send_buffer, T &recv_buffer,
                                        C4_Req &request);

#endif
};

} // end namespace rtt_c4

#endif // c4_C4_Req_hh

//---------------------------------------------------------------------------//
// end of c4/C4_Req.hh
//---------------------------------------------------------------------------//

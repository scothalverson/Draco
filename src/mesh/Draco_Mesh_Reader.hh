//----------------------------------*-C++-*----------------------------------//
/*!
 * \file   mesh/Draco_Mesh_Reader.hh
 * \author Ryan Wollaeger <wollaeger@lanl.gov>
 * \date   Friday, Jul 13, 2018, 13:48 pm
 * \brief  RTT_Draco_Mesh_Reader header file.
 * \note   Copyright (C) 2018 Los Alamos National Security, LLC.
 *         All rights reserved. */
//---------------------------------------------------------------------------//

#ifndef rtt_mesh_Draco_Mesh_Reader_hh
#define rtt_mesh_Draco_Mesh_Reader_hh

#include <cstddef>
#include <vector>

namespace rtt_mesh {

//===========================================================================//
/*!
 * \class Draco_Mesh_Reader
 *
 * \brief Abstract base class for all readers supported by Draco_Mesh_Builder.
 */
//===========================================================================//

class Draco_Mesh_Reader {
public:
  //! Virtual destructor
  virtual ~Draco_Mesh_Reader() {
    // this can not evidently be pure
  }

  // >>> ACCESSORS

  virtual unsigned get_numdim() const = 0;
  virtual unsigned get_numcells() const = 0;
  virtual unsigned get_numnodes() const = 0;
  virtual std::vector<double> get_nodecoord(size_t node) const = 0;
  virtual std::vector<int> get_cellnodes(size_t cell) const = 0;
  virtual unsigned get_numsides() const = 0;
  virtual unsigned get_sideflag(size_t side) const = 0;
  virtual std::vector<int> get_sidenodes(size_t side) const = 0;
  virtual unsigned get_celltype(size_t cell) const = 0;
  virtual unsigned get_sidetype(size_t side) const = 0;
};

} // namespace rtt_mesh

#endif // rtt_mesh_Draco_Mesh_Reader_hh

//---------------------------------------------------------------------------//
// end of mesh/Draco_Mesh_Reader.hh
//---------------------------------------------------------------------------//

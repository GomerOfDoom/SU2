/*!
 * \file CFEMStandardElementBase.hpp
 * \brief Base class for the FEM standard elements.
 *        The functions are in the <i>CFEMStandardElementBase.cpp</i> file.
 * \author E. van der Weide
 * \version 7.0.6 "Blackbird"
 *
 * SU2 Project Website: https://su2code.github.io
 *
 * The SU2 Project is maintained by the SU2 Foundation
 * (http://su2foundation.org)
 *
 * Copyright 2012-2020, SU2 Contributors (cf. AUTHORS.md)
 *
 * SU2 is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * SU2 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with SU2. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <iostream>
#include <vector>
#include <cstdlib>

#include "../CConfig.hpp"
#include "../toolboxes/C2DContainer.hpp"

using namespace std;

/*!
 * \class CFEMStandardElementBase
 * \brief Base class for a FEM standard element.
 * \author E. van der Weide
 * \version 7.0.6 "Blackbird"
 */
class CFEMStandardElementBase {
public:
  static const size_t vecLen = 8;   /*!< \brief Vector length must be a multiple of vecLen for good performance. */

protected:
  unsigned short VTK_Type;         /*!< \brief Element type using the VTK convention. */
  unsigned short nPoly;            /*!< \brief Polynomial order of the element. */
  unsigned short orderExact;       /*!< \brief Polynomial order that must be integrated exactly by the integration rule. */
  unsigned short nDOFs;            /*!< \brief Total number of DOFs. */
  unsigned short nDOFsPad;         /*!< \brief Padded version of nDOFs. */
  unsigned short nIntegration;     /*!< \brief Total number of points used in the numerical integration. */
  unsigned short nIntegrationPad;  /*!< \brief Padded version of nIntegration. */

  su2passivevector wIntegration;    /*!< \brief The weights of the integration points for this standard element. */

public:
  /*!
  * \brief Constructor. Nothing to be done.
  */
  CFEMStandardElementBase() = default;

  /*!
  * \brief Destructor. Nothing to be done.
  */
  virtual ~CFEMStandardElementBase() = default;

public:
  /*!
   * \brief Virtual function, that, if used, must be overwritten by the derived class.
   * \param[in]  matB    - Matrix that contains the input data.
   * \param[in]  ldb     - Leading dimension of matB (gemm convention).
   * \param[in]  ldc     - Leading dimension of matC (gemm convention).
   * \param[in]  n       - Second dimension of matB and matC (gemm convention).
   * \param[out] matC    - Result of the multiplication C = A*B.
   * \param[out] matDerC - Result of the multiplication CDer = ADer*B.
   * \param[in]  config  - Pointer to the configuration. Used for the timings.
   */
  virtual void DataIntegrationPoints(const ColMajorMatrix<su2double>    &matB,
                                     const unsigned short               ldb,
                                     const unsigned short               ldc,
                                     const unsigned short               n,
                                     ColMajorMatrix<su2double>          *matC,
                                     vector<ColMajorMatrix<su2double> > *matDerC,
                                     const CConfig                      *config) const {
    SU2_MPI::Error(string("This function must be overwritten by the derived class"),
                   CURRENT_FUNCTION);
  }

  /*!
   * \brief Virtual function, that, if used, must be overwritten by the derived class.
   * \return The number of different face types of the volume element.
   */
  virtual unsigned short GetnFaceTypes(void) const {
    SU2_MPI::Error(string("This function must be overwritten by the derived class"),
                   CURRENT_FUNCTION);
    return 0;
  }

  /*!
   * \brief Virtual function, that, if used, must be overwritten by the derived class.
   * \param[in] ind - Index of the face type for which the VTK type must be returned.
   * \return The VTK type of the given face type.
   */
  virtual unsigned short GetVTK_TypeFace(unsigned short ind) const {
    SU2_MPI::Error(string("This function must be overwritten by the derived class"),
                   CURRENT_FUNCTION);
    return 0;
  }

  /*!
   * \brief Function, which makes available the number of total DOFs of the element.
   * \return  The number of total DOFs.
   */
  inline unsigned short GetNDOFs(void) const {return nDOFs;}

  /*!
   * \brief Function, which makes available the padded number of total DOFs of the element.
   * \return  The padded number of total DOFs.
   */
  inline unsigned short GetNDOFsPad(void) const {return nDOFsPad;}

  /*!
   * \brief Function, which makes available the number of total integratin points of the element.
   * \return  The number of total integration points.
   */
  inline unsigned short GetNIntegration(void) {return nIntegration;}

  /*!
   * \brief Function, which makes available the padded number of total integratin points of the element.
   * \return  The padded number of total integration points.
   */
  inline unsigned short GetNIntegrationPad(void) {return nIntegrationPad;}

  /*!
   * \brief Function, which makes available the polynomial degree of the element.
   * \return  The polynomial degree of the element.
   */
  inline unsigned short GetPolyDegree(void) const {return nPoly;}

  /*!
   * \brief Function, which makes available the type of the element.
   * \return  The type of the element using the VTK convention.
   */
  inline unsigned short GetVTK_Type(void) const {return VTK_Type;}

  /*!
   * \brief Virtual function, that, if used, must be overwritten by the derived class.
   * \param[in]  LGLDistribution - Whether or not the LGL node distribution must be used.
   * \param[in]  matCoor         - Matrix that contains the coordinates of the grid DOFs.
   * \param[in]  ldb             - Leading dimension of matCoor (gemm convention).
   * \param[in]  ldc             - Leading dimension of matMetricTerms (gemm convention).
   * \param[out] matMetricTerms  - Vector of matrices to compute the metric terms.
   * \param[out] Jacobians       - Vector to store the Jacobians of the transformation.
   * \param[out] jacMin          - Minimum value of the Jacobian.
   * \param[out] jacMax          - Maximum value of the Jacobian.
   */
  virtual void MinMaxJacobians(const bool                         LGLDistribution,
                               const ColMajorMatrix<su2double>    &matCoor,
                               const unsigned short               ldb,
                               const unsigned short               ldc,
                               vector<ColMajorMatrix<su2double> > &matMetricTerms,
                               su2activevector                    &Jacobians,
                               su2double                          &jacMin,
                               su2double                          &jacMax) const {
    SU2_MPI::Error(string("This function must be overwritten by the derived class"),
                   CURRENT_FUNCTION);
  }

  /*!
  * \brief Function, which checks if the function arguments correspond to this standard element.
  * \param[in] val_VTK_Type - Type of the element using the VTK convention.
  * \param[in] val_nPoly    - Polynomial degree of the element.
  * \return Whether or not the function arguments correspond to this standard element.
  */
  inline bool SameStandardElement(unsigned short val_VTK_Type,
                                  unsigned short val_nPoly) {
    if(val_VTK_Type != VTK_Type) return false;
    if(val_nPoly    != nPoly)    return false;
    return true;
  }

  /*!
   * \brief Static function, which makes available the number of DOFs for an element
   *        corresponding to the arguments.
   * \param[in] VTK_Type   - Type of the element using the VTK convention.
   * \param[in] nPoly      - Polynomial degree of the element.
   * \return The number of DOFs
   */
  static unsigned short GetNDOFsStatic(unsigned short VTK_Type,
                                       unsigned short nPoly);

protected:

  /*!
   * \brief Function, which determines the integration points for a tetrahedron
   *        such that polynomials of orderExact are integrated exactly.
   * \param[out] rTet - Vector of the parametric r-coordinates of the integration points.
   * \param[out] sTet - Vector of the parametric s-coordinates of the integration points.
   * \param[out] tTet - Vector of the parametric t-coordinates of the integration points.
   * \param[out] wTet - Vector of the weights of the integration points.
   */
  void IntegrationPointsTetrahedron(vector<passivedouble> &rTet,
                                    vector<passivedouble> &sTet,
                                    vector<passivedouble> &tTet,
                                    vector<passivedouble> &wTet);

  /*!
   * \brief Function, which determines the integration points for a triangle
   *        such that polynomials of orderExact are integrated exactly.
   * \param[out] rTriangle - Vector of the parametric r-coordinates of the integration points.
   * \param[out] sTriangle - Vector of the parametric s-coordinates of the integration points.
   * \param[out] wTriangle - Vector of the weights of the integration points.
   */
  void IntegrationPointsTriangle(vector<passivedouble> &rTriangle,
                                 vector<passivedouble> &sTriangle,
                                 vector<passivedouble> &wTriangle);

  /*!
   * \brief Function, which determines the location of the 1D grid DOFs for polynomial
   *        degree nPoly when an equidistant spacing is used.
   * \param[out] r - Vector of the parametric r-coordinates of the DOFs.
   */
  void Location1DGridDOFsEquidistant(vector<passivedouble> &r);

  /*!
   * \brief Function, which determines the location of the 1D grid DOFs for polynomial
   *        degree nPoly when the LGL distribution is used.
   * \param[out] r - Vector of the parametric r-coordinates of the DOFs.
   */
  void Location1DGridDOFsLGL(vector<passivedouble> &r);

  /*!
   * \brief Function, which determines the location of the triangular grid DOFs for
   *        polynomial degree nPoly when an equidistant spacing is used.
   * \param[out] r - Vector of the parametric r-coordinates of the DOFs.
   * \param[out] s - Vector of the parametric s-coordinates of the DOFs.
   */
  void LocationTriangleGridDOFsEquidistant(vector<passivedouble> &r,
                                           vector<passivedouble> &s);

  /*!
   * \brief Function, which determines the location of the triangular grid DOFs for
   *        polynomial degree nPoly when the LGL distribution is used. The definition
   *        used is according to the book of Hesthaven and Warburton.
   * \param[out] r - Vector of the parametric r-coordinates of the DOFs.
   * \param[out] s - Vector of the parametric s-coordinates of the DOFs.
   */
  void LocationTriangleGridDOFsLGL(vector<passivedouble> &r,
                                   vector<passivedouble> &s);

private:
  /*!
   * \brief Function, which computes the value of the unscaled Legendre polynomial for the given x-coordinate.
   * \param[in] n     - Order of the Jacobi polynomial.
   * \param[in] x     - Coordinate (-1 <= x <= 1) for which the Legendre polynomial must be evaluated
   * \return            The value of the unscaled Legendre polynomial of order n for the given value of x
   */
  passivedouble Legendre(unsigned short n,
                         passivedouble  x);

  /*!
   * \brief Function, which computes the value of the Jacobi polynomial for the given x-coordinate.
   * \param[in] n     - Order of the Jacobi polynomial.
   * \param[in] alpha - Alpha coefficient of the Jacobi polynomial.
   * \param[in] beta  - Beta coefficient of the Jacobi polynomial.
   * \param[in] x     - Coordinate (-1 <= x <= 1) for which the Jacobi polynomial must be evaluated.
   * \return            The value of the normalized Jacobi polynomial of order n for the given value of x.
   */
  passivedouble NormJacobi(unsigned short n,
                           unsigned short alpha,
                           unsigned short beta,
                           passivedouble  x);

  /*!
   * \brief Function, which computes the Vandermonde matrix for a standard 1D edge.
   * \param[in]  r  - Parametric coordinates for which the Vandermonde matrix must be computed.
   * \param[out] V  - Matrix to store the Vandermonde matrix in all r-locations.
   */
  void Vandermonde1D(const vector<passivedouble>   &r,
                     ColMajorMatrix<passivedouble> &V); 

  /*!
   * \brief Function, which computes a scaled warp function for all DOFs of a standard triangle
   *        with a polynomial basis function of order nPoly. This info is used to construct
   *        the location of the DOFs inside a standard triangle.
   * \param[in]  rout - Input coordinates for the warp factor.
   * \param[out] warp - Warp factor to be computed.
   */
  void WarpFactor(const vector<passivedouble> &rout,
                  vector<passivedouble>       &warp);
};
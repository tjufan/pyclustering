/*!

@authors Andrei Novikov (pyclustering@yandex.ru)
@date 2014-2020
@copyright GNU Public License

@cond GNU_PUBLIC_LICENSE
    pyclustering is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    pyclustering is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
@endcond

*/

#pragma once


#include <memory>
#include <vector>

#include <pyclustering/cluster/cluster_data.hpp>


namespace pyclustering {

namespace clst {


using representative_sequence = std::vector< std::vector<double> >;
using representative_sequence_ptr = std::shared_ptr<representative_sequence>;


/*!

@class    bsas_data bsas_data.hpp pyclustering/cluster/bsas_data.hpp

@brief    Clustering results of BSAS algorithm that consists of information about allocated
          clusters and their representatives (points).

*/
class bsas_data : public cluster_data {
private:
    representative_sequence m_representatives = { };

public:
    /*!

    @brief    Default constructor of the clustering algorithm.

    */
    bsas_data() = default;

    /*!

    @brief    Copy constructor of the clustering algorithm.

    */
    bsas_data(const bsas_data & p_other) = default;

    /*!

    @brief    Move constructor of the clustering algorithm.

    */
    bsas_data(bsas_data && p_other) = default;

    /*!

    @brief    Default destructor of the clustering algorithm.

    */
    virtual ~bsas_data() = default;

public:
    /*!
    
    @brief    Returns representatives that corresponds to allocated clusters.
    
    */
    representative_sequence & representatives() { return m_representatives; }

    /*!
    
    @brief    Returns representatives that corresponds to allocated clusters.
    
    */
    const representative_sequence & representatives() const { return m_representatives; }
};


}

}
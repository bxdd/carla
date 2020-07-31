/****************************************************************************/
// Eclipse SUMO, Simulation of Urban MObility; see https://eclipse.org/sumo
// Copyright (C) 2005-2020 German Aerospace Center (DLR) and others.
// This program and the accompanying materials are made available under the
// terms of the Eclipse Public License 2.0 which is available at
// https://www.eclipse.org/legal/epl-2.0/
// This Source Code may also be made available under the following Secondary
// Licenses when the conditions for such availability set forth in the Eclipse
// Public License 2.0 are satisfied: GNU General Public License, version 2
// or later which is available at
// https://www.gnu.org/licenses/old-licenses/gpl-2.0-standalone.html
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
/****************************************************************************/
/// @file    PCPolyContainer.h
/// @author  Daniel Krajzewicz
/// @author  Michael Behrisch
/// @author  Jakob Erdmann
/// @date    Mon, 05 Dec 2005
///
// A storage for loaded polygons and pois
/****************************************************************************/
#pragma once
#include <config.h>

#include <string>
#include <map>
#include <vector>
#include <utils/shapes/ShapeContainer.h>


// ===========================================================================
// class declarations
// ===========================================================================
class Boundary;
class SUMOPolygon;
class PointOfInterest;
class OptionsCont;
class OutputDevice;


// ===========================================================================
// class definitions
// ===========================================================================
/**
 * @class PCPolyContainer
 * @brief A storage for loaded polygons and pois
 */
class PCPolyContainer : public ShapeContainer {
public:
    /** @brief Constructor
     * @param[in] prune Whether added polygons/pois shall be pruned
     * @param[in] pruningBoundary The pruning boundary (only valid if prune==true)
     * @param[in] removeByNames Names of objects that shall not be added
     */
    PCPolyContainer(bool prune, const Boundary& pruningBoundary,
                    const std::vector<std::string>& removeByNames);


    /// @brief Destructor
    ~PCPolyContainer();


    /** @brief Adds a polygon to the storage
     *
     * If pruning is enabled, "ignorePruning" is false and the polygon lies outside
     *  the pruning boundary, or if the polygon's name is within the names of
     *  objects to discard, the polygon is deleted and false is returned.
     *
     * Afterwards it is tested whether a polygon with the same name is already stored.
     *  If so, an error message is printed, the polygon is deleted and false is returned, otherwise true.
     *
     * @param[in] poly The polygon to add
     * @param[in] ignorePruning Whether the polygon shall be kept, even though it would be pruned
     * @return Whether the polygon could be added
     */
    bool add(SUMOPolygon* poly, bool ignorePruning = false);


    /** @brief Adds a poi to the storage
     *
     * If pruning is enabled, "ignorePruning" is false and the poi lies outside
     *  the pruning boundary, or if the poi's name is within the names of
     *  objects to discard, the poi is deleted and false is returned.
     *
     * Afterwards it is tested whether a poi with the same name is already stored.
     *  If so, an error message is printed, the poi is deleted and false is returned, otherwise true.
     *
     * @param[in] poly The poi to add
     * @param[in] ignorePruning Whether the poi shall be kept, even though it would be pruned
     * @return Whether the poi could be added
     */
    bool add(PointOfInterest* poi, bool ignorePruning = false);


    void addLanePos(const std::string& poiID, const std::string& laneID, double lanePos, double lanePosLat);

    /** @brief Saves the stored polygons and pois into the given file
     * @param[in] file The name of the file to write stored objects' definitions into
     * @param[in] useGeo Whether to write output in geo-coordinates
     * @exception IOError If the file could not be opened
     */
    void save(const std::string& file, bool useGeo);

    /** @brief Saves the stored polygons and pois into the given file in dlrTDP format
     * @param[in] prefix The prefix of the file to write stored objects' definitions into
     */
    void saveDlrTDP(const std::string& prefix);


    /** @brief Retuns a unique id for a given name
     *
     * The unique id is generated by having an internal map of ids to running numbers.
     * The first call to this method will return 0, all subsequent with the same
     *  key will return numbers increased by one at each call.
     * @param[in] key The key to get a running number for
     * @return Unique id (running number of calls that used this key)
     */
    int getEnumIDFor(const std::string& key);


private:

    struct LanePos {
        LanePos() {}
        LanePos(const std::string& _laneID, double _pos, double _posLat) :
            laneID(_laneID), pos(_pos), posLat(_posLat) {}
        std::string laneID;
        double pos;
        double posLat;
    };

    /// @brief An id to pos map for lane pos specs
    std::map<std::string, LanePos> myLanePosPois;

    /// @brief An id to int map for proper enumeration
    std::map<std::string, int> myIDEnums;

    /// @brief The boundary that described the rectangle within which an object must be in order to be kept
    Boundary myPruningBoundary;

    /// @brief Information whether the pruning boundary shall be used
    bool myDoPrune;

    /// @brief List of names of polygons/pois that shall be removed
    std::vector<std::string> myRemoveByNames;

    static void writeDlrTDPHeader(OutputDevice& device, const OptionsCont& oc);

private:
    /// @brief Invalidated copy constructor
    PCPolyContainer(const PCPolyContainer& s);

    /// @brief Invalidated assignment operator
    PCPolyContainer& operator=(const PCPolyContainer& s);


};

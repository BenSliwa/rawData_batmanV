//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "predictiveMobilityMetric.h"

namespace inet {


uint32_t Batman5Routing::getPredictiveMobilityMetric(MACAddress &_mac){

    uint32_t metric = 0;
    double maxDistance = check_and_cast<physicallayer::IMediumLimitCache*>(this->getModuleByPath("radioMedium.mediumLimitCache"))->getMaxCommunicationRange().get();
    double currentDistance = getDistanceToMAC(_mac);
    double futureDistance = getFutureDistanceToMAC(_mac);

    int currentAlpha = distance_alpha;
    int futureAlpha = distance_alpha;

    double index = max(powf((currentDistance/maxDistance),currentAlpha), powf((futureDistance/maxDistance),futureAlpha)); // 0: very close ,1: far away
//    std::cout << "index: " << index << ", currentIndex: " << powf((currentDistance/maxDistance),currentAlpha) << ", futureIndex: " << powf((futureDistance/maxDistance),futureAlpha) << std::endl;

    if(index > 1){
        // distance is longer than maxDistance
        index = 1;
    }
    else if(index < 0){
        index = 0.0;
    }

    uint32_t cakePiece = (BATADV_THROUGHPUT_MAX_VALUE-1) / ogmv2_ttl;
    metric = map(index,0,1,0,cakePiece);

//    std::cout << hostModule->getFullName() << ": currentDistance: " << currentDistance << ", futureDistance: " << futureDistance << ", index: " << index << ", metric: " << metric << " to " << _mac.str() << endl;

    return metric;
}

double Batman5Routing::getFutureDistanceToMAC(MACAddress &_mac){

    double distance = 0;
    Coord myPosition = Coord(0,0,0);
    Coord neighPosition = Coord (0,0,0);

    TraceMobility *mobility =  dynamic_cast<TraceMobility*>(this->getParentModule()->getSubmodule("mobility"));
    if(mobility){
        myPosition = mobility->getFuturePosition(secondsInTheFuture);
//        std::cout << "My future position: " << myPosition << std::endl;
    }

    neighPosition = getFuturePositionFromMAC(_mac, secondsInTheFuture);
//    std::cout << "Neighbor future position: " << neighPosition << std::endl;

    if(neighPosition == Coord(0,0,0)){
//        std::cout << "Coord is 0,0,0 " << std::endl;
        return distance;
    }

    distance = (myPosition - neighPosition).length();

//    std::cout << hostModule->getFullName() << ": future Positions: " << myPosition << " <-> " << neighPosition << " => " << distance << std::endl;

    return distance;

}

Coord Batman5Routing::getFuturePositionFromMAC(MACAddress &_mac, double futureSecs){
    Coord positionOfMAC = Coord(0,0,0);

    TraceMobility *mobility = dynamic_cast<TraceMobility*>(getMobilityFromMAC(_mac));

    if(mobility){
        positionOfMAC = mobility->getFuturePosition(futureSecs);
//        std::cout << "FuturePosition of " << _mac.str() << " is " << positionOfMAC << std::endl;
    }

    return positionOfMAC;
}



predictiveMobilityMetric::predictiveMobilityMetric() {}
predictiveMobilityMetric::~predictiveMobilityMetric() {}

} /* namespace inet */

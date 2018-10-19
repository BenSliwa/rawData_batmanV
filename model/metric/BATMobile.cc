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

#include "BATMobile.h"

namespace inet {

uint32_t Batman5Routing::getScoredMetric(MACAddress &_mac){
    uint32_t metric = 0;
    double maxDistance = check_and_cast<physicallayer::IMediumLimitCache*>(this->getModuleByPath("radioMedium.mediumLimitCache"))->getMaxCommunicationRange().get();
    double distance = getDistanceToMAC(_mac);
    double futureDistance = getFutureDistanceToMAC(_mac);

    int alpha = distance_alpha;

    if(futureDistance/maxDistance > 1){
        // in the future this host will be out of reach
        // setting futureDistance to the baddest possible Value
        futureDistance = maxDistance;
    }
    if(distance/maxDistance > 1){
        // this host is now out of reach
        // setting distance to the baddest possible Value
        distance = maxDistance;
    }

    int Np = secondsInTheFuture;// predictive Width, Skalierung
    double PtrendMax = 0.1;     // Gewichtung
    double step = 0.25;         // timeStep in seconds
    double dstep = 13 * 0.25;   // Speed * timeSteps
    double Ptrend = (futureDistance - distance)/(2 * dstep * Np) * PtrendMax;

    double link_score = min( 1 - powf(distance/maxDistance,alpha), 1 - powf(futureDistance/maxDistance,alpha )) + Ptrend; // 0->far away , 1->near

//    std::cout << "currentIndex: " << 1 - powf(distance/maxDistance,alpha) << ", futureIndex: " << 1 - powf(futureDistance/maxDistance,alpha ) <<  " -min-> " << link_score-Ptrend << ", Ptrend: " << Ptrend << std::endl;

    if(link_score > 1){
//        std::cout << "link_score to big: " << link_score - Ptrend << " + " << Ptrend << " = " << link_score  << std::endl;
        link_score = 1.0;
    }
    else if(link_score < 0){
//        std::cout << "link_score to little: " << 1 - powf(distance/maxDistance,alpha) << " " << 1 - powf(futureDistance/maxDistance,alpha ) << " + " << Ptrend << " = " << link_score << std::endl;
        link_score = 0.0;
    }

    metric = map(link_score,0,1,0,BATADV_THROUGHPUT_MAX_VALUE-1);

//    std::cout << hostModule->getFullName() << ": " << link_score << " -> " << metric << std::endl;

    return metric;
}


BATMobile::BATMobile() {}
BATMobile::~BATMobile() {}

} /* namespace inet */

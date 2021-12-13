// main.cpp
//
// ICS 46 Winter 2021
// Project #5: Rock and Roll Stops the Traffic
//
// This is the program's main() function, which is the entry point for your
// console user interface.

#include "InputReader.hpp"
#include "RoadMapReader.hpp"
#include "TripReader.hpp"
#include "Digraph.hpp"
#include "RoadMapWriter.hpp"
#include <vector>
#include <map>
#include <iostream>
#include <string>
#include <iomanip>

//Output when asking for the shortest distance.
void minDistance(Trip& trip, RoadMap& roadMap)
{
    std::cout << "Shortest distance from " + roadMap.vertexInfo(trip.startVertex) + "to " + roadMap.vertexInfo(trip.endVertex) << std::endl;
    std::cout << "  Begin at " + roadMap.vertexInfo(trip.startVertex) << std::endl;
    double sumDistance = 0.0;
    std::map<int, int> shortestPath = roadMap.findShortestPaths(trip.startVertex, [](RoadSegment edgeInfo){return edgeInfo.miles;});
    std::string s = "";
    int vertex = trip.endVertex;
    while(true)
    {
        if(vertex == trip.startVertex)
        {
            break;
        }

        float distance = (int)(roadMap.edgeInfo(shortestPath[vertex], vertex).miles * 10 + 0.5);
        distance = distance / 10;
        if(roadMap.edgeInfo(shortestPath[vertex], vertex).miles == 1)
        {
            s = "  Continue to " + roadMap.vertexInfo(shortestPath[vertex]) + " (1.0 mile)\n" + s;
        }
        else
        {
            s = "  Continue to " + roadMap.vertexInfo(shortestPath[vertex]) + 
                " (" + std::to_string(int(distance)) + "." + std::to_string(int((distance - (int)distance)*10 + 0.5)) + " miles)\n" + s;
        }
        
        sumDistance += roadMap.edgeInfo(shortestPath[vertex], vertex).miles;
        vertex = shortestPath[vertex];
    }
    std::cout << s;
    std::cout << "Total distance: " << std::setprecision(2) << sumDistance << " miles" << std::endl;
}

//Change time to a string, (hrs, mins, secs) format
std::string timeString(double time)
{
    int hr = int(time);
    int min = int((time - hr) * 60);
    double sec = time*3600 - hr*3600 - min*60;

    std::string t = "";
    if(hr != 0)
    {
        if(hr == 1)
        {
            t += "1 hr ";
        }
        else
        {
            t += std::to_string(hr) + " hrs ";
        }
        if(min == 1)
        {
            t += "1 min ";
        }
        else
        {
            t += std::to_string(min) + " mins ";
        }
        if(sec == 1)
        {
            t += "1.0 sec";
        }
        else
        {
            float second = (int)(sec * 10 + 0.5);
            second = second / 10;
            t = t + std::to_string(int(second)) + "." + std::to_string(int((second - (int)second)*10 + 0.5)) + "secs";
        }   
    }
    else
    {
        if(min == 1)
        {
            t += "1 min ";
        }
        else if(min > 1)
        {
            t += std::to_string(min) + " mins ";
        }
        if(sec == 1)
        {
            t += "1.0 sec";
        }
        else
        {
            float second = (int)(sec * 10 + 0.5);
            second = second / 10;
            t = t + std::to_string(int(second)) + "." + std::to_string(int((second - (int)second)*10 + 0.5)) + " secs";
        }
    }
    return t;
}

//Output when asking for the shortest time.
void minTime(Trip& trip, RoadMap& roadMap)
{
    std::cout << "Shortest driving time from " + roadMap.vertexInfo(trip.startVertex) + "to " + roadMap.vertexInfo(trip.endVertex) << std::endl;
    std::cout << "  Begin at " + roadMap.vertexInfo(trip.startVertex) << std::endl;
    double sumTime = 0.0;
    std::map<int, int> shortestPath = roadMap.findShortestPaths(trip.startVertex, [](RoadSegment edgeInfo){return edgeInfo.miles / edgeInfo.milesPerHour;});
    std::string s = "";
    int vertex = trip.endVertex;
    while(true)
    {
        if(vertex == trip.startVertex)
        {
            break;
        }
        float distance = (int)(roadMap.edgeInfo(shortestPath[vertex], vertex).miles * 10 + 0.5);
        distance = distance / 10;

        float speed = (int)(roadMap.edgeInfo(shortestPath[vertex], vertex).milesPerHour * 10 + 0.5);
        speed = speed / 10;

        double time = roadMap.edgeInfo(shortestPath[vertex], vertex).miles / roadMap.edgeInfo(shortestPath[vertex], vertex).milesPerHour;
        std::string t = timeString(time);

        s = "  Continue to " + roadMap.vertexInfo(shortestPath[vertex]) + 
            " (" + std::to_string(int(distance)) + "." + std::to_string(int((distance - (int)distance)*10 + 0.5)) + " miles @ " + 
            std::to_string(int(speed)) + "." + std::to_string(int((speed - (int)speed)*10 + 0.5)) + "mph = " + t +")\n" + s;

        sumTime += roadMap.edgeInfo(shortestPath[vertex], vertex).miles / roadMap.edgeInfo(shortestPath[vertex], vertex).milesPerHour;
        vertex = shortestPath[vertex];
    }
    std::cout << s;
    std::string tSum = timeString(sumTime);
    std::cout << "Total time: " << tSum << std::endl;
}



int main()
{
    InputReader reader(std::cin);
    RoadMapReader roadR;
    TripReader tripR;
    RoadMap roadMap = roadR.readRoadMap(reader);
    std::vector<Trip> tripV = tripR.readTrips(reader);
    //RoadMapWriter roadW;
    //roadW.writeRoadMap(std::cout, roadMap);
    for (auto trip: tripV)
    {
        if(trip.metric == TripMetric::Distance)
        {
            minDistance(trip, roadMap);
            std::cout << std::endl;
        }
        else
        {
            minTime(trip, roadMap);
            std::cout << std::endl;
        } 
    }
    return 0;
}


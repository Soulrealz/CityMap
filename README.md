# CityMap

Program that represents a map of a city. The information about the city is contained in a txt file. Every row in the txt file contains name of intersection and after it a pair of names of other intersections and the length of the street from the first to the second intersection

# Example
Hamilton Lookout 150 Broadway 100

Lookout Hamilton 150

Broadway James 400

...

one directional streets exist, also path from intersection to another and from that other to the first one are not guaranteed to be the same

# Tasks
1. Checks if there is a way to reach one intersection from another given intersection
2. Find 3 shortest routes between two given intersections
3. If intersections are closed off (construction or w/e) alternatively find the three shortest routes between the two given intersections
4. Check if given an intersection you can go around a part of the city and get back to where you started from
5. Check if you can go around all intersections without visiting the same intersection twice. If such a feat is possible, calculate that distance
6. Check if from one intersection you can reach all other intersections
7. Find all streets that go into a deadend intersection. Street name is formed from the intersection where it starts and the intersection where it ends. Example A -> B -> C. C is a deadend so street name would be "B and C"

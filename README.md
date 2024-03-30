# winter - project
Student1: Partner1 Name: Yihan Diao 920885648 Student2: Partner2 Name: Yifan Cui 919921542


# code references
1.https://stackoverflow.com/questions/2758080/how-to-sort-an-stl-vector(sort the value in the unorderedmap)
2.https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm (this is used for finding dijkstra algorithm)
3.https://stackoverflow.com/questions/17898342/priority-queue-in-dijkstras-algorithm(this is used for fiding the shortestpath, using
    priority queue to increase perfomance)
4.https://www.youtube.com/watch?v=V6H1qAeB-l4 (watch a video to help undersanding priority queue)


# generative AI use
## prompt1 : how to assign a hash value to an unorderedmap whose key is a pair

changes : I use the the gptgiven self defined functions to generate the hash value for assign a hash value for my unorderedmap

how to assign a hash value to an unorderedmap whose key is a pair
ChatGPT
Assigning a hash value to a key that is a pair for use in an unordered_map in C++ requires you to define a custom hash function. An unordered_map uses hashing to store its elements, and by default, C++ does not provide a hash function for user-defined types like pairs (or tuples). Therefore, you need to implement a hash function for the pair and pass it to the unordered_map as a template argument.

Here's an approach using the std::pair as a key for an unordered_map. This example defines a custom hash function by combining the hash values of the elements of the pair:

cpp
Copy code
#include <iostream>
#include <unordered_map>
#include <utility> // For std::pair
#include <functional> // For std::hash

// Define a hash function for std::pair<int, int>
struct pair_hash {
    template <class T1, class T2>
    std::size_t operator () (const std::pair<T1,T2> &pair) const {
        auto hash1 = std::hash<T1>{}(pair.first);
        auto hash2 = std::hash<T2>{}(pair.second);
        return hash1 ^ hash2; // Combine the two hash values. You might also use boost::hash_combine
    }
};

int main() {
    // Create an unordered_map with std::pair<int, int> as key, using the custom hash function
    std::unordered_map<std::pair<int, int>, std::string, pair_hash> map;

    // Add elements to the map
    map[std::make_pair(1, 2)] = "One and Two";
    map[std::make_pair(3, 4)] = "Three and Four";

    // Access elements in the map
    std::cout << "The value for key (1, 2) is " << map[std::make_pair(1, 2)] << std::endl;
    std::cout << "The value for key (3, 4) is " << map[std::make_pair(3, 4)] << std::endl;

    return 0;
}
This code defines a pair_hash structure that can be used as a custom hash function. It templates on the pair's types, allowing it to be used with pairs of different types, not just int. The hash function combines the hash values of the first and second elements of the pair. The combination method used here is a simple XOR operation, but depending on your use case, you might prefer a more sophisticated method to reduce the chance of hash collisions.

Note: If you use a more complex pair (e.g., std::pair<std::string, std::string>), ensure the hash combination method adequately distributes hash values to minimize collisions.




## prompt2 : how to check if the key is unique in an unordered map

changes : I need to check if the TNodeid is unique while addedge. I do not want duplicated Tnodeids.

Checking if a key is unique in an unordered map involves ensuring that the key does not already exist in the map before performing any insertions or updates. In C++, an unordered map is a container that stores elements formed by the combination of a key value and a mapped value, and no two elements can have equivalent keys. Here is a straightforward approach to check for key uniqueness:

Using find Method: The find method searches the container for an element with a key equivalent to k and returns an iterator to it if found, otherwise it returns an iterator to unordered_map::end.
cpp
Copy code
#include <unordered_map>
#include <iostream>

int main() {
    std::unordered_map<int, std::string> myMap;

    // Insert some elements into the unordered_map
    myMap[1] = "one";
    myMap[2] = "two";

    int keyToCheck = 3;

    // Check if the key is unique
    if(myMap.find(keyToCheck) == myMap.end()) {
        std::cout << "Key is unique. You can insert the element." << std::endl;
        // Proceed to insert the new element as the key is unique
        myMap[keyToCheck] = "three"; // Example insertion
    } else {
        std::cout << "Key already exists. Element is not unique." << std::endl;
    }

    return 0;
}
Using count Method: This method returns the number of elements matching specific key (0 or 1 for unordered_map because it does not allow duplicates). If the count is 0, the key is unique.
cpp
Copy code
if(myMap.count(keyToCheck) == 0) {
    std::cout << "Key is unique. You can insert the element." << std::endl;
    // Insert the element as the key is unique
} else {
    std::cout << "Key already exists. Element is not unique." << std::endl;
}
Both methods are effective for checking if a key is unique in an unordered_map. Use the find method if you need to do something with the iterator afterward (e.g., reading or modifying the value), or use the count method for a simple existence check.




## prompt3: how to return an infinite number in c++?

changes: I need to return a infinite number while calulating shortes distance and assign time to inf is dis is inf

In C++, you can represent an infinite number using the floating-point types float, double, or long double and the special values defined in the <limits> header or by performing operations that result in infinity. Here are several ways to return an infinite value in C++:

Using the <limits> Header:
You can use the std::numeric_limits template to get the representation of positive or negative infinity for a given floating-point type.
cpp
Copy code
#include <iostream>
#include <limits>

double positiveInfinity() {
    return std::numeric_limits<double>::infinity();
}

double negativeInfinity() {
    return -std::numeric_limits<double>::infinity();
}

int main() {
    std::cout << "Positive Infinity: " << positiveInfinity() << std::endl;
    std::cout << "Negative Infinity: " << negativeInfinity() << std::endl;
    return 0;
}
Using Division by Zero:
In C++, dividing a positive floating-point number by zero results in positive infinity, while dividing a negative floating-point number by zero results in negative infinity.
cpp
Copy code
double positiveInfinity() {
    return 1.0 / 0.0;
}

double negativeInfinity() {
    return -1.0 / 0.0;
}

// Usage similar to the first example
Using Mathematical Operations:
Certain operations that exceed the largest representable number will also result in infinity.
cpp
Copy code
#include <cmath>

double positiveInfinity() {
    return std::exp(1000); // This is likely to result in infinity
}

// Usage similar to the first example
It's important to note that using division by zero to generate infinity is generally not recommended because it relies on IEEE floating-point behavior, which is supported by most, but not all, C++ implementations. The safest and most portable method is to use std::numeric_limits<double>::infinity().

Also, operations involving infinity follow the rules of IEEE floating-point arithmetic. For example, any positive number multiplied by infinity gives positive infinity, any negative number multiplied by infinity gives negative infinity, and operations like infinity minus infinity result in NaN (not a number).









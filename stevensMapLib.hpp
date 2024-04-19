/**
 * StevensMapLib.h
 * 
 * A library used to expand the functionality of std::maps and std::unordereD_maps
*/

#include "stevensStringLib.h"

#ifndef STEVENSMAPLIB
#define STEVENSMAPLIB

namespace stevensMapLib
{
    /*** Member variables ***/

    /*** Methods ***/
    /**
     * Takes two maps as input and performs the + operation on their shared keys. The resulting pairs
     * are returned in an unordered_map of the same type.
     * 
     * Parameters:
     *  std::unordered_map<K,V> A - One of the unordered_maps we are adding together.
     *  std::unordered_map<K,V> B - The other map we are adding together.
     *  std::string addOperationTarget - The target of the + operator with this function. Possible values are:
     *                                   ///"keys" - Only perform the + operation on keys
     *                                   "values" - Only perform the + operation on the values
     *                                   "keys and values" - Perform the + operation on both keys and values 
     *  bool omitKeysNotShared - Boolean indicating whether or not we should omit the pairs from A and B that they do not share from the returned map.
     * 
     * Returns:
    */
    template<typename K, typename V> 
    std::unordered_map<K,V> addUnordered_maps(  std::unordered_map<K,V> A,
                                                std::unordered_map<K,V> B,
                                                std::string addOperationTarget = "keys and values",
                                                bool omitKeysNotShared = false )
    {
        std::unordered_map<K,V> AB = {};

        //Iterate through the pairs of map A and see if any match the keys of of map B
        for(const auto & [key,value] : A)
        {
            //When we find a match, perform the + operation on the targeted indicated by the addOperationTarget parameter
            if(B.contains(key))
            {
                //Store the matched value in map AB, and keep track of the key in map B that was found as a match
                if(addOperationTarget == "values")
                {
                    AB[key] = A[key] + B[key];
                }
                else //keys and values
                {
                    AB[key + key] = A[key] + B[key];
                }
            }
            else
            {
                if(!omitKeysNotShared)
                {
                    //If we do not find a match, check to see if we should omit the key that is not shared. If not, store it in map AB.
                    AB[key] = value;
                }
            }
        }
        //If we're not omitting shared keys, add the keys from B that were not matched
        if(!omitKeysNotShared)
        {
            for(const auto & [key,value] : B)
            {
                if(!AB.contains(key))
                {
                    AB[key] = value;
                }
            }
        }

        return AB;
    }


    /**
     * Converts an unordered_map with keys of type K and values of type V to a vector of tuples with the first index containing the key
     * and the second index containing the value.
     * 
     * Parameter:
     *  std::unordered_map<K,V> umap - The unordered map that we want to convert to a vector of tuples.
     * 
     * Returns:
     *  std::vector< tuple<K,V> > - A vector containing tuples of the key-value pairs from the passed-in unordered_map in no particular order.
     * 
    */
    template<typename K, typename V>
    std::vector< std::tuple<K,V> > unordered_map_to_vecOfTuples( std::unordered_map<K,V> umap )
    {
        std::vector< std::tuple<K,V> > returnVec = {};

        //Iterate through the unordered_map
        for(const auto & [key,value] : umap)
        {
            //Construct a tuple containing the key and the value and push it to the returnVec
            returnVec.push_back( std::make_tuple(key,value) );
        }

        return returnVec;
    }


    /**
     * @brief Gets a random key from a maplike object
     * 
     * @tparam M The maplike type that we are getting a key from.
     * @tparam K The key type we are getting.
     * @tparam V The value of the pairs in the maplike type we are getting a key from.
     * 
     * @param map - The map object we are picking a random key from.
     * 
     * @return A random key found in map.
     */
    template <template <typename, typename, typename...> class M, typename K, typename V, typename... Args>
    K getRandomKey( const M<K,V> & map )
    {
        auto it = map.begin();
        std::advance( it, rand() % map.size() );
        return it->first;
    }


    /**
     * @brief Returns the first key in the maplike object.
     * 
     * @tparam M The maplike type that we are getting a key from.
     * @tparam K The key type we are getting.
     * @tparam V The value of the pairs in the maplike type we are getting a key from.
     * 
     * @param map - The map object we are getting the first key from.
     * 
     * @return The first key from the map.
     */
    template <template <typename, typename, typename...> class M, typename K, typename V, typename... Args>
    K getFirstKey( const M<K,V> & map)
    {
        auto it = map.begin();
        return it->first;
    }


    /**
     * @brief Gets a vector containing all of the keys from each pair within the given map.
     * 
     * @tparam M The type of the maplike object we are obtaining keys from.
     * @tparam K The type of the keys in the map.
     * @tparam V The type of the values in the map.
     * @tparam Args 
     * @param map - The maplike object we are obtaining keys from.
     * @return std::vector<K> - A vector containing all of the keys found in order from the maplike object.
     */
    template <template <typename, typename, typename...> class M, typename K, typename V, typename... Args>
    std::vector<K> getKeyVector(    const M<K,V> & map  )
    {
        //Initialize a vector that we will push keys back into
        std::vector<K> keyVector;
        for(const auto & [key,value] : map)
        {
            keyVector.push_back(key);
        }
        return keyVector;
    }


    /**
     * @brief Returns a maplike object which uses stringlike keys that begin only with a certain combination of characters.
     * 
     * Example: resultMap = stevensMapLib::getPairsWhereKeysStartWith("style:", { {"style:textColor","red"}, {"style:backgroundColor", "black"}, {"sectionName", "header"} });
     * //resultMap == { {"style:textColor", "red"}, {"style:backgroundColor", "black"}  };
     * 
     * @tparam M The type of the maplike object we are getting pairs from.
     * @tparam K The type of keys in the map. Must be stringlike. 
     * @tparam V The type of values in the map.
     * @tparam Args
     * @param str - Stringlike string of characters we want all of our keys in our resulting map to start with, which are also contained within map
     * @param map - The maplike object we are extracting key-value pairs from, which the keys must begin with str
     * 
     * @return M<K,V> A maplike object with stringlike keys which contains only keys that start with the requested string of characters.
     */
    template <template <typename, typename, typename...> class M, typename K, typename V, typename... Args>
    M<K,V> getPairsWhereKeysStartWith(  const M<K,V> & map,
                                        const K & str   )
    {
        //This is the map that we'll return
        M<K,V> returnMap = {};

        //Iterate through all the keys in the map
        for(const auto & [key,value] : map)
        {
            //Add the pair that starts with str to the map that will be returned
            if(stevensStringLib::startsWith(key, str))
            {
                returnMap.emplace(key, value);
            }
        }
        //Return the map with pairs that have only keys beginning with str
        return returnMap;
    }


    /**
     * @brief Modifies a maplike object's stringlike keys by erasing all instances of a substring from the keys.
     * 
     * Example: resultMap = stevensMapLib::eraseStringFromKeys( "style:",   {   {"style:textColor", "red"}, {"style:backgroundColor", "black"}  });
     * //resultMap == { {"textColor", "red"},   {"backgroundColor", "black"}};
     * 
     * @tparam M The type of maplike object we are modifying the keys of.
     * @tparam K The type of keys in the map.
     * @param str The string we wish to target and erase from all of map's keys.
     * @param map The maplike object with stringlike keys we are modifying the keys of.
     * @return M<K,V> A modified version of the map we passed into the function, with all of its keys having str erased from them.
     */
    template <template <typename, typename, typename...> class M, typename K, typename V, typename... Args>
    M<K,V> eraseStringFromKeys( const M<K,V> & map,
                                const K & str   )
    {
        //The map that will be returned
        M<K,V> returnMap = {};
        //Variable that will hold the modified key
        K modifiedKey;

        //Iterate through all the keys in the map
        for(const auto & [key,value] : map)
        {
            //We remove the str from the key's name and add it to our return map
            modifiedKey = stevensStringLib::replaceSubstr(key, str, "");
            returnMap.emplace(modifiedKey, value);
        }

        return returnMap;
    }


    /**
     * @brief Given a maplike object with values of a numeric tyoe, return the map with all of its elements that have 
     *        values less than zero set to zero.
     * 
     * Example:
     * setNegativeValuesTOZero( {{"firstKey", 3}, {"secondKey", -9001}, {"thirdKey", -4}} ) == { {"firstKey",3}, {"secondKey",0}, {"thirdKey",0} };
     * 
     * @param map - The maplike object with numeric values (bool, short, int, long, float, double, etc) that we want its negative values set to zero.
     * @return Void, but directly modifies the map object we pass in by reference.
     */
    template<typename M>
    void setNegativeValuesToZero( M & map )
    {
        for(auto & [key,value] : map)
        {
            if(value < 0)
            {
                map[key] = 0;
            }
        }
    }


    /**
     * @brief Given a maplike object that uses stringlike keys and a prospective key for the map, modify the key so that it
     *        it is unique from anything else in map. Useful for creating new, valid keys for maps.
     * 
     * @tparam M The type of maplike object object we are creating a unique key for.
     * @tparam K The stringlike type of key we are creating a unique instance of for the map.
     * @tparam V The type of value in the maplike object map.
     * @tparam Args
     * @param map The maplike object we are creating a unique key for.
     * @param keyString The stringlike key we are potentially modifying to be unique relative to all other keys in map.
     * @param algorithm The method by which we modify our key to make it unique.
     * 
     */
    template <template <typename, typename, typename...> class M, typename K, typename V, typename... Args>
    K createUniqueKeyString(    M<K,V> & map,
                                K keyString = "",
                                std::string & algorithm = "integer concatenation"    )
    {
        if(algorithm == "integer concatenation")
        {
            K originalKeyString = keyString;
            int integerSuffix = 0;
            while(map.contains(keyString))
            {
                keyString = originalKeyString + std::to_string(integerSuffix);
                integerSuffix++;
            }
        }

        return keyString;
    }


    /**
     * @brief Given a maplike object, return a random key-value pair from the map, deleting it from the map in the process.
     * 
     * @tparam M The type of the maplike object we are popping a pair from.
     * @tparam K The type of key in the pair we are popping from the map.
     * @tparam V The type of value in the pair we are popping from the map.
     * @tparam Args
     * @param map The maplike object we are popping a key-value pair from.
     * @return A random pair from the map
     * 
     */
    template <template <typename, typename, typename...> class M, typename K, typename V, typename... Args>
    std::pair<K,V> popRandom(   M<K,V> & map    )
    {
        //Get a random pair from the map using getRandomKey()
        K key = stevensMapLib::getRandomKey(map);
        V value = map[key];
        //Construct the pair we want to return
        std::pair<K,V> randomPair = std::pair(key, value);
        //Erase the pair from the source map
        map.erase(key);
        return randomPair;
    }


};
#endif
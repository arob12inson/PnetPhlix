#include "MovieDatabase.h"
#include "Movie.h"

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>

using namespace std;

MovieDatabase::MovieDatabase()
{
    // Replace this line with correct code.
    m_hasBeenCalledBefore = false;
}

bool MovieDatabase::load(const string& filename)
{
    std::ifstream reader(filename);
    
    if (reader && m_hasBeenCalledBefore == false){ //if valid file and hasn't been loaded before
        m_hasBeenCalledBefore = true;
        std::string movieID, movieName, movieYear, directors, actors, genres;
        float rating;
        while (getline(reader, movieID)){ //First line will always be the
            getline(reader, movieName);
            getline(reader, movieYear);
            
            getline(reader, directors);
            std::vector<string> director_vector = stringToVector(directors);
            
            getline(reader, actors);
            std::vector<string> actors_vector = stringToVector(actors);
            
            getline(reader, genres);
            std::vector<string> genres_vector = stringToVector(genres);
            
            std::string string_rating;
            getline(reader, string_rating);
            rating = std::stof(string_rating);
            
            //create a movie object
            Movie temp(movieID, movieName, movieYear, director_vector, actors_vector, genres_vector, rating);
            for (int i = 0; i < director_vector.size(); i ++){//
                string x = makeLowerCase(director_vector[i]); // Make lower case so searching isn't sensitive
                directorsMap.insert(x, temp);
            }
            for (int i = 0; i < actors_vector.size(); i++){
                string x = makeLowerCase(actors_vector[i]);
                actorsMap.insert(x, temp);
            }
            for (int i = 0; i < genres_vector.size(); i++){
                string x = makeLowerCase(genres_vector[i]);
                genreMap.insert(x, temp);
            }
            IDMap.insert(movieID, temp);
            
            reader.ignore(10000, '\n');// Consume the last line. If no last line, this will invaidate the object
        }
        return true;
    }
    else{
        return false;
    }
}

Movie* MovieDatabase::get_movie_from_id(const string& id) const
{
    TreeMultimap<string, Movie>::Iterator x = IDMap.find(id);
    if (x.is_valid()){
        return &(x.get_value());
        
    }
    else {
        return nullptr;
    }
}

vector<Movie*> MovieDatabase::get_movies_with_director(const string& director) const
{
    vector<Movie*> movies;
    TreeMultimap<string, Movie>::Iterator x = directorsMap.find(makeLowerCase(director));
    while (x.is_valid()){
        movies.push_back(&x.get_value());
        x.advance();
    }
    return movies;
}

vector<Movie*> MovieDatabase::get_movies_with_actor(const string& actor) const
{
    vector<Movie*> movies;
    TreeMultimap<string, Movie>::Iterator x = actorsMap.find(makeLowerCase(actor));
    while (x.is_valid()){
        movies.push_back(&x.get_value());
        x.advance();
    }
    return movies;
    
}

vector<Movie*> MovieDatabase::get_movies_with_genre(const string& genre) const
{
    vector<Movie*> movies;
    TreeMultimap<string, Movie>::Iterator x = genreMap.find(makeLowerCase(genre));
    while (x.is_valid()){
        movies.push_back(&x.get_value());
        x.advance();
    }
    return movies;
}


//turn an x1,x2,x3,...,xn to a vector
std::vector<std::string> MovieDatabase::stringToVector(const std::string& input) {
    std::vector<std::string> result;
    std::stringstream ss(input);
    std::string item;
    while (std::getline(ss, item, ',')) {
        result.push_back(item);
    }
    return result;
}

//Make a string lowercase
std::string MovieDatabase::makeLowerCase(const std::string& input) const{
    std::string temp = "";
    for (int i = 0; i < input.size(); i++){
        temp += tolower(input[i]);
    }
    return temp;
}

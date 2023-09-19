#ifndef MOVIEDATABASE_INCLUDED
#define MOVIEDATABASE_INCLUDED

#include <string>
#include <vector>
#include "treemm.h"

class Movie;

class MovieDatabase
{
  public:
    MovieDatabase();
    bool load(const std::string& filename);
    Movie* get_movie_from_id(const std::string& id) const;
    std::vector<Movie*> get_movies_with_director(const std::string& director) const;
    std::vector<Movie*> get_movies_with_actor(const std::string& actor) const;
    std::vector<Movie*> get_movies_with_genre(const std::string& genre) const;

  private:
    bool m_hasBeenCalledBefore;
    TreeMultimap<std::string, Movie> directorsMap;
    TreeMultimap<std::string, Movie> actorsMap;
    TreeMultimap<std::string, Movie> genreMap;
    TreeMultimap<std::string, Movie> IDMap;
    std::vector<std::string> stringToVector(const std::string& input);
    std::string makeLowerCase(const std::string& input) const;
};

#endif // MOVIEDATABASE_INCLUDED

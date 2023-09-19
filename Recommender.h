#ifndef RECOMMENDER_INCLUDED
#define RECOMMENDER_INCLUDED

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include "UserDatabase.h"
#include "MovieDatabase.h"
#include "Movie.h"

class UserDatabase;
class MovieDatabase;

struct MovieAndRank
{
    MovieAndRank(const std::string& id, int score)
     : movie_id(id), compatibility_score(score)
    {
        mdb = nullptr;
    }
    MovieAndRank()
    {
        
        compatibility_score = 0;
        mdb = nullptr;
    }
    void giveDatabase(MovieDatabase* m) {
        mdb = m;
    }
    
    bool operator< (const MovieAndRank& other) const { // Operator< is defined so that if m1 < m2, m1 will be recommended first before m2
        if (compatibility_score > other.compatibility_score){ // Based on score
            return true;
        }
        else if (compatibility_score < other.compatibility_score){
            return false;
        }
        else {
            Movie* movie1 = mdb->get_movie_from_id(movie_id);
            Movie* movie2 = mdb->get_movie_from_id(other.movie_id);
            if (movie1->get_rating() > movie2->get_rating()){//Based on rating
                return true;
            }
            else if (movie1->get_rating() < movie2->get_rating()){ //Based on alphabetical order
                return false;
            }
            else {
                return (movie1->get_title() < movie2->get_title());
            }
        }
    }
    
    std::string movie_id;
    int compatibility_score;
    const MovieDatabase* mdb; 
};

class Recommender
{
  public:
    Recommender(const UserDatabase& user_database,
                const MovieDatabase& movie_database);
    std::vector<MovieAndRank> recommend_movies(const std::string& user_email,
                                               int movie_count) const;
    ~Recommender();

  private:
    const UserDatabase* userdb;
    const MovieDatabase* moviedb;
    std::vector<MovieAndRank*> MandRtoFree; 
    void allocatePoints(int pointsToGive,
                        const std::unordered_map<std::string, Movie*>& watchList,
                        const std::vector<std::string>& pastPreferences,
                        std::unordered_map<std::string, MovieAndRank>& moviesRanking,
                        std::unordered_map<std::string,
                        MovieAndRank>::iterator& rankingsIterator) const;
};

#endif // RECOMMENDER_INCLUDED

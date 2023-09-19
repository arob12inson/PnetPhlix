#include "Recommender.h"
#include "UserDatabase.h"
#include "MovieDatabase.h"
#include "User.h"
#include "Movie.h"

#include <unordered_map>
#include <unordered_set>
#include <set>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

Recommender::Recommender(const UserDatabase& user_database,
                         const MovieDatabase& movie_database)
{
    userdb = &user_database;
    moviedb = &movie_database;
}

vector<MovieAndRank> Recommender::recommend_movies(const string& user_email, int movie_count) const
{
    User* user = userdb->get_user_from_email(user_email);
    vector<MovieAndRank> movieRecs;
    if (user == nullptr){ //Return empty vector if user is not found
        return movieRecs;
    }
    vector<string> movieList = user->get_watch_history();
    vector<string> watchedDirectors;
    vector<string> watchedActors;
    vector<string> watchedGenres;
    
    unordered_map<string, Movie*> watchList;//maps ids to movies the user has watched
    
    for (int i = 0; i < movieList.size(); i++){ // for each movie the user watched
        string id = movieList[i];
        Movie* m = moviedb->get_movie_from_id(id);
        watchList[id] = m; //Add the movie to a hash table
        
        //Add Actors, Directors, and Genres
        vector<string> moviesDirectors = m->get_directors();
        for (int i = 0; i < moviesDirectors.size(); i++){//Find all movies that share current movie's director
            watchedDirectors.push_back(moviesDirectors[i]);
        }
        vector<string> moviesActors = m->get_actors(); //Find all movies that share current movie's actor
        for (int i = 0; i < moviesActors.size(); i++){
            watchedActors.push_back(moviesActors[i]);
        }
        vector<string> moviesGenres = m ->get_genres();
        for (int i = 0; i < moviesGenres.size(); i++){//find all movies that share current movie's genre
            watchedGenres.push_back(moviesGenres[i]);
        }
    }
    
    unordered_map<string, MovieAndRank> moviesRanking;//Create a hashmap of movie rankings for instant lookups, mapping IDs to MovieAndRank pointers
    unordered_map<string, MovieAndRank>::iterator rankingsIterator;
    
    
    allocatePoints(20, watchList, watchedDirectors, moviesRanking, rankingsIterator); // Assign points to movies with the same directors
    allocatePoints(30, watchList, watchedActors, moviesRanking, rankingsIterator); //                   ""                     actors
    allocatePoints(1, watchList, watchedGenres, moviesRanking, rankingsIterator);  //                   ""                     genres

    
    for (rankingsIterator = moviesRanking.begin(); rankingsIterator != moviesRanking.end(); rankingsIterator++){ //convert he moviesRanking hashmap to a vector
        MovieAndRank m = (rankingsIterator->second);
        m.mdb = moviedb;
        movieRecs.push_back(m);
    }
    
    sort(movieRecs.begin(), movieRecs.end());
    
    vector<MovieAndRank> subvector;
    
    if (movieRecs.size() <= movie_count){ // If there are less recommended movies than asked for, return the vector
        return movieRecs;
        
    }else{
        subvector = {movieRecs.begin(), movieRecs.begin()+(movie_count)}; //create a subvector with the number of movies we asked to recommend
        return subvector;
    }

}

void Recommender::allocatePoints(int pointsToGive, const std::unordered_map<std::string, Movie*>& watchList, const std::vector<std::string>& pastPreferences, std::unordered_map<std::string, MovieAndRank>& moviesRanking, std::unordered_map<std::string, MovieAndRank>::iterator& rankingsIterator) const{
    for (int i = 0; i < pastPreferences.size(); i++) { //calculate corresponding scores for the first 3 iterators
        vector<Movie*> movies_by_preference;

        switch (pointsToGive) { // use the points to allocate to determine whether to look through director's movies, actor's movies, or movies of a genre
            case 20:
                movies_by_preference = moviedb->get_movies_with_director(pastPreferences[i]);
                break;
            case 30:
                movies_by_preference = moviedb->get_movies_with_actor(pastPreferences[i]);
                break;
            case 1:
                movies_by_preference = moviedb->get_movies_with_genre(pastPreferences[i]);
                break;
        }

        for (int i = 0; i < movies_by_preference.size(); i++){
            string directorsMoviesId = movies_by_preference[i]->get_id();
            if (watchList.find(directorsMoviesId) == watchList.end()){ // if it isn't a movie the user already watched/isn't in the hashmap
                rankingsIterator = moviesRanking.find(directorsMoviesId); //see if the movie is one that we've recommended

                if (rankingsIterator == moviesRanking.end()){ //if it is a movie not recommended yet
                    MovieAndRank newMovieAndRank(directorsMoviesId, pointsToGive); //create a new movie and rank
//                    MandRtoFree.push_back(newMovieAndRank); //put pointer into array of created MandR objects (So we don't have a memory leak)
                    moviesRanking[directorsMoviesId] = newMovieAndRank;
                }
                else {
                   rankingsIterator->second.compatibility_score += pointsToGive; // increase the points by the corresponding amount
                }
            }
        }
    }
    
}

Recommender::~Recommender(){
    for (int i = 0; i < MandRtoFree.size(); i++){
        delete MandRtoFree[i];
    }
}

#include "UserDatabase.h"
#include "User.h"

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

UserDatabase::UserDatabase()
{
    // Replace this line with correct code.
    m_hasBeenCalledBefore = false;
}

bool UserDatabase::load(const string& filename)
{
    std::ifstream reader(filename);
    
    if (reader && m_hasBeenCalledBefore == false){
        std::string name, email;
        int numMoviesWatched;
        while (getline(reader, name)){ //check if reader is in a valid state. If it is, then store the name value @ beginning of the user block
            getline(reader, email);//store the email
            reader >> numMoviesWatched;//see how many movies program must add
            reader.ignore(10000, '\n');
            vector <std::string> movieIDs;
            for (int i = 0; i < numMoviesWatched; i++){ 
                std::string movieId;
                getline(reader, movieId);
                movieIDs.push_back(movieId);
            }
            reader.ignore(10000, '\n');//clear the empty line at the end of a "user block"
            
            //create a new user with the gathered information, and put it in the tree
            User temp(name, email, movieIDs);
            userBase.insert(email, temp);
        }
        m_hasBeenCalledBefore = true;
        return true;
    }
    else {
        return false;//return false if the file given is invalid OR the userdatabase has been loaded before
    }

}

User* UserDatabase::get_user_from_email(const string& email) const
{
    TreeMultimap<std::string, User>::Iterator it = userBase.find(email);
    if (it.is_valid() == false){
        return nullptr;
    }
    else {
        return &(it.get_value());
    }
    return nullptr;  // Replace this line with correct code.
}

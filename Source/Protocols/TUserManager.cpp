#include "TUserManager.h"
#include <string.h>

TUserManager::TUser TUserManager::DefaultUser("admin", "passw0rd");

TUserManager::TUserManager()
: m_userCount(0)
{
    
}
   
int TUserManager::GetUserCount(bool acceptDefaultUser)
{
    if ((m_userCount == 0) && (acceptDefaultUser))
    {
        //there are no users in user storage,
        //therefore allows reading of the default user
        return 1;
    }
    return m_userCount;
}

TUserManager::TUser* TUserManager::GetUser (int aIndex)
{
    if ((m_userCount == 0) && (aIndex == 0))
    {        
        //manager contains no users, therefore returns 
        //default user with default password
        return &DefaultUser;
    }
    //needs to be implemented by class descendand
    return NULL;
}

bool TUserManager::CredentialsValid(const char* aUserName, const char* aPassword)
{
    int n = GetUserCount(true);
    bool userValid = false;
    for(int i = 0; i < n; i++)
    {
        TUser* user = GetUser(i);
        if ((strcmp(user->userName, aUserName)==0) && (strcmp(user->password, aPassword)==0))
        {
            userValid = true;
            break;
        }
    }
    return userValid;
}


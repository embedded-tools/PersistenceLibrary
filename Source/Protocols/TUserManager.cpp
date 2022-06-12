#include "TUserManager.h"
#include <string.h>

const char* TUserManager::DefaultUserName = "Administrator";
const char* TUserManager::DefaultPassword = "PASSw@rd";
   
int TUserManager::GetUserCount(bool acceptDefaultUser)
{
    if ((m_userCount == 0) && (acceptDefaultUser))
    {
        m_userCount = 1;
    }
    return m_userCount;
}

TUserManager::TUser TUserManager::GetUser (int aIndex)
{
    if ((m_userCount == 0) && (aIndex == 0))
    {
        return TUser(DefaultUserName, DefaultPassword);
    }
    //needs to be implemented by class descendand
    return TUser("", "");
}


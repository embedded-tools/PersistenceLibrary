#ifndef TUSER_MANAGER___H
#define TUSER_MANAGER___H

class TUserManager
{
    private:
        int m_userCount;

    public:
        static const int   USER_NAME_LENGTH = 24;
        static const int   PASSWORD_LENGTH  = 24;

        struct TUser
        {
            const char* userName;
            const char* password;

            TUser(const char* aUserName, const char* aPassword)
            {
                userName = aUserName;
                password = aPassword;
            }
        };

        TUserManager();

        static TUser DefaultUser;

        virtual int    GetUserCount(bool acceptDefaultUser = false);
        virtual TUser* GetUser (int aIndex);

        bool CredentialsValid(const char* aUserName, const char* aPassword);
};

#endif
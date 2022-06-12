#ifndef TUSER_MANAGER___H
#define TUSER_MANAGER___H

class TUserManager
{
    private:
        static const char* DefaultUserName;
        static const char* DefaultPassword;  

        int m_userCount;

    public:
        static const int   MaximumUserLength     = 24;
        static const int   MaximumPasswordLength = 24;

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

        virtual int   GetUserCount(bool acceptDefaultUser = true);
        virtual TUser GetUser (int aIndex);
};

#endif
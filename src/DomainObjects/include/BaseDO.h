#pragma once

class BaseDO
{
    protected:
        long long _id;
        BaseDO(long long id);
        void id(long long id);

    public:
        long long id() const;
};
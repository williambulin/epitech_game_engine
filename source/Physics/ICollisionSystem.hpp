template<typename T>
class IColisionSystem
{
    public:
        virtual ~IColisionSystem() {}
        virtual bool Overlap(const T&) = 0;
        virtual bool operator==(const T&) = 0;
};
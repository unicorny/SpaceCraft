#include "noisecache.h"


using namespace std;

namespace noise
{
    namespace module
    {
        bool CacheValue:: operator < (const CacheValue& b) const
        {
            if(x < b.x) 
            {
                return true;
            }
            else if(x == b.x)
            {
                if(y < b.y)
                    return true;
                else if(y == b.y && z < b.z)
                    return true;
            }
            return false;
        }
        
        
        NoiseCache::NoiseCache()
        : Module (GetSourceModuleCount ())
        {
            
        }
        
        double NoiseCache::GetValue (double x, double y, double z) const
        {
            map<CacheValue, double>::iterator it;
            it = mValues.find(CacheValue(x, y, z));
            if(it == mValues.end())
            {
                double value = m_pSourceModule[0]->GetValue (x, y, z);
                mValues.insert(CACHE_ENTRY(CacheValue(x, y, z), value));
                return value;
            }
            else
            {
                return it->second;
            }
        }
    }
}
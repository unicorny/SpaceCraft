#ifndef __SC_NOISE_CACHE__
#define __SC_NOISE_CACHE__

#include <noise/noise.h>
#include <map>

namespace noise
{
    namespace module
    {
        struct CacheValue
        {
            CacheValue(double _x, double _y, double _z): x(_x), y(_y), z(_z) {}
            bool operator < (const CacheValue& b) const;
            double x;
            double y;
            double z;
        };
        
        class NoiseCache : public Module
        {
        public:
            /// Constructor.
            NoiseCache ();

            virtual int GetSourceModuleCount () const
            {
              return 1;
            }

            virtual double GetValue (double x, double y, double z) const;

            virtual void SetSourceModule (int index, const Module& sourceModule)
            {
                Module::SetSourceModule (index, sourceModule);
                mValues.clear();
            }
            
            int getCachedValuesCount() {return mValues.size();}
            void reset() {mValues.clear();}
        protected:
            mutable std::map<CacheValue, double> mValues;
            typedef std::pair<CacheValue, double> CACHE_ENTRY;

        };
    }
}

#endif //__SC_NOISE_CACHE__
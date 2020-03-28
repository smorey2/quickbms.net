using System;

namespace QuickBms
{
    public class BmsService : IBmsService
    {
        public float AddFloat(float x, float y)
        {
            return x + y;
        }
    }
}

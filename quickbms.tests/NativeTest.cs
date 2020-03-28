using NUnit.Framework;

namespace QuickBms.Tests
{
    public class NativeTest
    {
        [Test]
        public void TestLoad()
        {
            NativeMethods.Touch();
        }
    }
}
